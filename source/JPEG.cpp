#include <stdexcept>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <ios>
#include <new>
#include <turbojpeg.h>
#include <gctypes.h>
#include <ogc/gx_struct.h>
#include "../include/JPEG.hpp"


const char* JPEG::SCapcSubsampName[] = {"4:4:4", "4:2:2", "4:2:0", "Grayscale", "4:4:0", "4:1:1"};
const char* JPEG::SCapcColorspaceName[] = {"RGB", "YCbCr", "GRAY", "CMYK", "YCCK"};


//---------------------------------------------------------------------------------
//	Convert two RGB pixels to one Y1CbY2Cr
//---------------------------------------------------------------------------------
u32 JPEG::rgb2yuv (u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2) noexcept
{
//---------------------------------------------------------------------------------
	s32 y1, cb1, cr1, y2, cb2, cr2, cb, cr;

	y1 = (299 * r1 + 587 * g1 + 114 * b1) / 1000;
	cb1 = (-16874 * r1 - 33126 * g1 + 50000 * b1 + 12800000) / 100000;
	cr1 = (50000 * r1 - 41869 * g1 - 8131 * b1 + 12800000) / 100000;
	y2 = (299 * r2 + 587 * g2 + 114 * b2) / 1000;
	cb2 = (-16874 * r2 - 33126 * g2 + 50000 * b2 + 12800000) / 100000;
	cr2 = (50000 * r2 - 41869 * g2 - 8131 * b2 + 12800000) / 100000;

	cb = (cb1 + cb2) >> 1;
	cr = (cr1 + cr2) >> 1;
	return (y1 << 24) | (cb << 16) | (y2 << 8) | cr;
}


JPEG::JPEG(const char* pcFilePath)
{
	FILE* pfileJpeg = nullptr;
	u8* pJpegBuf = nullptr;
	s64 lSize = 0;
	u64 lJpegSize = 0;

	/* Read the JPEG file into memory. */
	if ((pfileJpeg = fopen(pcFilePath, "rb")) == nullptr) 
		throw std::ios_base::failure("Error opening input file");
	if (fseek(pfileJpeg, 0, SEEK_END) < 0 || ((lSize = ftell(pfileJpeg)) < 0) ||
        fseek(pfileJpeg, 0, SEEK_SET) < 0)
		throw std::ios_base::failure("Error determining input file size");
	if (lSize == 0) throw std::ios_base::failure("Input file contains no data");

	lJpegSize = static_cast<u64>(lSize);

	if ((pJpegBuf = static_cast<u8*>(tjAlloc(lJpegSize))) == nullptr) throw std::bad_alloc();
	if (fread(pJpegBuf, lJpegSize, 1, pfileJpeg) < 1) 
		throw std::ios_base::failure("Error reading input file");

    fclose(pfileJpeg);
	pfileJpeg = nullptr;

	*this = JPEG(pJpegBuf, lJpegSize);

	tjFree(pJpegBuf);  
	pJpegBuf = nullptr;
}


JPEG::JPEG(const u8* pJpegBuf, u64 lJpegSize) : _iPosX{0}, _iPosY{0}
{
	tjhandle tjhandle = nullptr;
	u8* pImgBuf = nullptr;

	if ((tjhandle = tjInitDecompress()) == nullptr) 
		throw std::runtime_error("Error initializing decompressor");

	if (tjDecompressHeader3(tjhandle, pJpegBuf, lJpegSize, &_iWidth, &_iHeight, &_iInSubsamp, 
		&_iInColorspace) < 0) throw std::runtime_error("Error reading JPEG header");

	if ((pImgBuf = static_cast<u8*>(tjAlloc(_iWidth * _iHeight * tjPixelSize[TJPF_RGB]))) == nullptr)
		throw std::bad_alloc();

	if (tjDecompress2(tjhandle, pJpegBuf, lJpegSize, pImgBuf, _iWidth, 0, _iHeight, TJPF_RGB, 0) < 0)
		throw std::runtime_error("Error decompressing JPEG imagen");

	_pImgBuf = new u32[_iHeight * (_iWidth >> 1)];

	u32 iRow = 0, iColumn = 0;
	for (u16 i = 0; i < _iHeight; i++)
	{
		iRow = i * _iWidth * tjPixelSize[TJPF_RGB];
		for (u16 j = 0; j < (_iWidth >> 1); j++)
		{
			iColumn = j * (tjPixelSize[TJPF_RGB] << 1);
			_pImgBuf[i * (_iWidth >> 1) + j] = 
				JPEG::rgb2yuv(pImgBuf[iRow + iColumn], pImgBuf[iRow + iColumn + 1],
					pImgBuf[iRow + iColumn + 2], pImgBuf[iRow + iColumn + 3], 
					pImgBuf[iRow + iColumn + 4], pImgBuf[iRow + iColumn + 5]);
		}
	}

	tjFree(pImgBuf);
	pImgBuf = nullptr;
    tjDestroy(tjhandle);
	tjhandle = nullptr;
}


JPEG::JPEG(const JPEG& jpegOther) : _iWidth{jpegOther._iWidth}, _iHeight{jpegOther._iHeight},
	_iInSubsamp{jpegOther._iInSubsamp}, _iInColorspace{jpegOther._iInColorspace},
	_pImgBuf{new u32[(_iWidth >> 1) * _iHeight]}
{ 
	memcpy(_pImgBuf, jpegOther._pImgBuf, (_iWidth << 1) * _iHeight); 
}


JPEG::JPEG(JPEG&& jpegOther) noexcept : _iWidth{jpegOther._iWidth}, _iHeight{jpegOther._iHeight},
	_iInSubsamp{jpegOther._iInSubsamp}, _iInColorspace{jpegOther._iInColorspace}, 
	_pImgBuf{jpegOther._pImgBuf}
{
	jpegOther._iWidth = 0;
	jpegOther._iHeight = 0;
	jpegOther._iInSubsamp = 0;
	jpegOther._iInColorspace = 0;
	jpegOther._pImgBuf = nullptr;
}


JPEG& JPEG::operator =(const JPEG& jpegOther)
{
	if (this != &jpegOther)
	{
		delete[] _pImgBuf;

		_iWidth = jpegOther._iWidth;
		_iHeight = jpegOther._iHeight;
		_iInSubsamp = jpegOther._iInSubsamp;
		_iInColorspace = jpegOther._iInColorspace;
		_pImgBuf = new u32[(_iWidth >> 1) * _iHeight];

		memcpy(_pImgBuf, jpegOther._pImgBuf, (_iWidth << 1) * _iHeight);
	}

	return *this;
}


JPEG& JPEG::operator =(JPEG&& jpegOther) noexcept
{
	if (this != &jpegOther)
	{
		delete[] _pImgBuf;

		_iWidth = jpegOther._iWidth;
		_iHeight = jpegOther._iHeight;
		_iInSubsamp = jpegOther._iInSubsamp;
		_iInColorspace = jpegOther._iInColorspace;
		_pImgBuf = jpegOther._pImgBuf;
		
		jpegOther._iWidth = 0;
		jpegOther._iHeight = 0;
		jpegOther._iInSubsamp = 0;
		jpegOther._iInColorspace = 0;
		jpegOther._pImgBuf = nullptr;
	}

	return *this;
}


JPEG::~JPEG() noexcept
{
	delete[] _pImgBuf;
	_pImgBuf = nullptr;
}


void JPEG::display(s32 iX, s32 iY, void* xfb, const GXRModeObj* rmode)
{
	_iPosX = iX;
	_iPosY = iY;
	iX = iX * (rmode->fbWidth >> 1) / rmode->viWidth;
	iY = iY * rmode->xfbHeight / rmode->viHeight;

	bool bStop = false;

	for (u16 i = 0; i < _iHeight && !bStop; i++)
	{
		if (iY + i < 0) i += (-iY - 1);
		else if (iY + i >= rmode->xfbHeight) bStop = true;
		else
		{
			if (iX < 0)
				std::memcpy(static_cast<u32*>(xfb) + (iY + i) * (rmode->fbWidth >> 1), 
					_pImgBuf + i * (_iWidth >> 1) + (-iX), 
					std::min(((_iWidth >> 1) + iX) << 2, rmode->fbWidth << 1));
			else if (iX < rmode->fbWidth)
				std::memcpy(static_cast<u32*>(xfb) + (iY + i) * (rmode->fbWidth >> 1) + iX, 
					_pImgBuf + i * (_iWidth >> 1), 
					std::min(_iWidth << 1, ((rmode->fbWidth >> 1) - iX) << 2));
		}
	}
}
