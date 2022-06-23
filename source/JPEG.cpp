#include <iostream>
#include <stdexcept>
#include <cstring>
#include <turbojpeg.h>
#include <gctypes.h>
#include <ogc/gx_struct.h>
#include "JPEG.hpp"


const char* JPEG::subsampName[] = {"4:4:4", "4:2:2", "4:2:0", "Grayscale", "4:4:0", "4:1:1"};
const char* JPEG::colorspaceName[] = {"RGB", "YCbCr", "GRAY", "CMYK", "YCCK"};


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
	u64 lJpegSize = 0;

	/* Read the JPEG file into memory. */
    pfileJpeg = fopen(pcFilePath, "rb");
    fseek(pfileJpeg, 0, SEEK_END);
    lJpegSize = (u64)ftell(pfileJpeg);
    rewind(pfileJpeg);

    pJpegBuf = (u8*)tjAlloc(lJpegSize);
    fread(pJpegBuf, lJpegSize, 1, pfileJpeg);

    fclose(pfileJpeg);
	pfileJpeg = nullptr;

	*this = JPEG(pJpegBuf, lJpegSize);

	tjFree(pJpegBuf);  
	pJpegBuf = nullptr;
}


JPEG::JPEG(const u8* pJpegBuf, u64 lJpegSize) 
{
	tjhandle tjhandle = nullptr;
	u8* pImgBuf = nullptr;

	tjhandle = tjInitDecompress();

	tjDecompressHeader3(tjhandle, pJpegBuf, lJpegSize, &_iWidth, &_iHeight, &_iInSubsamp, 
		&_iInColorspace);

	pImgBuf = (u8*)tjAlloc(_iWidth * _iHeight * tjPixelSize[TJPF_RGB]);

	tjDecompress2(tjhandle, pJpegBuf, lJpegSize, pImgBuf, _iWidth, 0, _iHeight, TJPF_RGB, 0);

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


JPEG::JPEG(const JPEG& jpegOther) noexcept : _iWidth{jpegOther._iWidth}, _iHeight{jpegOther._iHeight},
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


JPEG& JPEG::operator =(const JPEG& jpegOther) noexcept
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


void JPEG::display(u32 iX, u32 iY, void* xfb, const GXRModeObj& rmode) const
{
	if (iX < 0 || iX >= rmode.fbWidth || iY < 0 ||iY >= rmode.xfbHeight || 
		iX + _iWidth > rmode.fbWidth || iY + _iHeight > rmode.xfbHeight) 
		throw std::out_of_range("Out of the buffer range");

	iX = iX * (rmode.fbWidth >> 1) / rmode.viWidth;

	for (u16 i = 0; i < _iHeight; i++)
		memcpy(((u32*)xfb) + (i + iY) * (rmode.fbWidth >> 1) + iX, _pImgBuf + i * (_iWidth >> 1), 
			_iWidth << 1);
}
