#include <iostream>
#include <stdexcept>
#include <cstring>
#include <turbojpeg.h>
#include <gctypes.h>
#include <ogc/gx_struct.h>
#include "DISPLAY.hpp"


void drawHorizontalLine(u32 iX1, u32 iX2, u32 iY, u32 iColor, void *xfb, const GXRModeObj& rmode)
{
	if (iX1 >= 0 && iX2 >= 0 && iX2 < rmode.fbWidth && iY >= 0 && iY < rmode.xfbHeight)
	{
		iX1 = iX1 * (rmode.fbWidth >> 1) / rmode.viWidth;
		iX2 = iX2 * (rmode.fbWidth >> 1) / rmode.viWidth;

		for (u32 i = iX1; i <= iX2; i++) ((u32*)xfb)[iY * (rmode.fbWidth >> 1) + i] = iColor;
	}
}


void drawVerticalLine(u32 iX, u32 iY1, u32 iY2, u32 iColor, void *xfb, const GXRModeObj& rmode)
{
	if (iX >= 0 && iX < rmode.fbWidth && iY1 >= 0 && iY2 >= 0 && iY2 < rmode.xfbHeight)
	{
		iX = iX * (rmode.fbWidth >> 1) / rmode.viWidth;

		for (u32 i = iY1; i <= iY2; i++) ((u32*)xfb)[i * (rmode.fbWidth >> 1) + iX] = iColor;
	}
}


void drawBox(u32 iX1, u32 iY1, u32 iX2, u32 iY2, u32 iColor, void *xfb, const GXRModeObj& rmode)
{
	if (iX1 >= 0 && iX2 < rmode.fbWidth && iY1 >= 0 && iY2 >= 0 && iY2 < rmode.xfbHeight)
	{
		drawHorizontalLine(iX1, iX2, iY1, iColor, xfb, rmode);
		drawHorizontalLine(iX1, iX2, iY2, iColor, xfb, rmode);
		drawVerticalLine(iX1, iY1, iY2, iColor, xfb, rmode);
		drawVerticalLine(iX2, iY1, iY2, iColor, xfb, rmode);
	}
}


Jpeg::Jpeg(const char* pcFilename)
{
	FILE* pfileJpeg = nullptr;
	tjhandle tjhandle = nullptr;
	u8* pJpegBuf = nullptr;
	u64 lJpegSize = 0;
	u8* pImgBuf;

	/* Read the JPEG file into memory. */
    pfileJpeg = fopen(pcFilename, "rb");
    fseek(pfileJpeg, 0, SEEK_END);
    lJpegSize = (u64)ftell(pfileJpeg);
    rewind(pfileJpeg);

    pJpegBuf = (u8*)tjAlloc(lJpegSize);
    fread(pJpegBuf, lJpegSize, 1, pfileJpeg);

    fclose(pfileJpeg);
	pfileJpeg = nullptr;

	tjhandle = tjInitDecompress();

	tjDecompressHeader3(tjhandle, pJpegBuf, lJpegSize, &_iWidth, &_iHeight, &_iInSubsamp, 
		&_iInColorspace);

	pImgBuf = (u8*)tjAlloc(_iWidth * _iHeight * tjPixelSize[TJPF_RGB]);

	tjDecompress2(tjhandle, pJpegBuf, lJpegSize, pImgBuf, _iWidth, 0, _iHeight, TJPF_RGB, 0);

	_pImgBuf = new u32[_iHeight * (_iWidth >> 1)];

	u32 iRow = 0, iColumn = 0;
	for (u16 i = 0; i < _iHeight; i++)
	{
		iRow = i * _iWidth * 3;
		for (u16 j = 0; j < (_iWidth >> 1); j++)
		{
			iColumn = j * 6;
			_pImgBuf[i * (_iWidth >> 1) + j] = 
				Jpeg::rgb2yuv(pImgBuf[iRow + iColumn], pImgBuf[iRow + iColumn + 1],
					pImgBuf[iRow + iColumn + 2], pImgBuf[iRow + iColumn + 3], 
					pImgBuf[iRow + iColumn + 4], pImgBuf[iRow + iColumn + 5]);
		}
	}

	tjFree(pImgBuf);
	pImgBuf = nullptr;
	tjFree(pJpegBuf);  
	pJpegBuf = nullptr;
    tjDestroy(tjhandle);
	tjhandle = nullptr;
}


Jpeg::Jpeg(const Jpeg& jpegOtro) noexcept : _iWidth{jpegOtro._iWidth}, _iHeight{jpegOtro._iHeight},
	_iInSubsamp{jpegOtro._iInSubsamp}, _iInColorspace{jpegOtro._iInColorspace},
	_pImgBuf{new u32[(_iWidth >> 1) * _iHeight]}
{ 
	memcpy(_pImgBuf, jpegOtro._pImgBuf, (_iWidth << 1) * _iHeight); 
}


Jpeg& Jpeg::operator =(const Jpeg& jpegOtro) noexcept
{
	if (this != &jpegOtro)
	{
		delete[] _pImgBuf;

		_iWidth = jpegOtro._iWidth;
		_iHeight = jpegOtro._iHeight;
		_iInSubsamp = jpegOtro._iInSubsamp;
		_iInColorspace = jpegOtro._iInColorspace;
		_pImgBuf = new u32[(_iWidth >> 1) * _iHeight];

		memcpy(_pImgBuf, jpegOtro._pImgBuf, (_iWidth << 1) * _iHeight);
	}

	return *this;
}


Jpeg::~Jpeg()
{
	delete[] _pImgBuf;
	_pImgBuf = nullptr;
}


//---------------------------------------------------------------------------------
//	Convert two RGB pixels to one Y1CbY2Cr
//---------------------------------------------------------------------------------
u32 Jpeg::rgb2yuv (u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2) noexcept
{
//---------------------------------------------------------------------------------
	int y1, cb1, cr1, y2, cb2, cr2, cb, cr;

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


void Jpeg::display(u32 iX, u32 iY, void* xfb, const GXRModeObj& rmode) const
{
	if (iX < 0 || iX >= rmode.fbWidth || iY < 0 ||iY >= rmode.xfbHeight || 
		iX + _iWidth > rmode.fbWidth || iY + _iHeight > rmode.xfbHeight) 
		throw std::out_of_range("Out of the buffer range");

	iX = iX * (rmode.fbWidth >> 1) / rmode.viWidth;

	for (u16 i = 0; i < _iHeight; i++)
	{
		memcpy((((u32*)xfb) + (i + iY) * (rmode.fbWidth >> 1) + iX), (_pImgBuf + i * (_iWidth >> 1)), 
			(_iWidth << 1));
	}
}
