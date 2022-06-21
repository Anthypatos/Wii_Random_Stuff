#include <iostream>
#include <turbojpeg.h>
#include <gctypes.h>
#include <ogc/gx_struct.h>
#include "display.hpp"


void drawHorizontalLine(u32 iX1, u32 iX2, u32 iY, u32 iColor, void *xfb, GXRModeObj *rmode)
{
	if (iX1 >= 0 && iX1 < rmode->fbWidth && iX2 >= 0 && iX2 < rmode->fbWidth && 
		iY >= 0 && iY < rmode->xfbHeight)
	{
		iX1 = iX1 * (rmode->fbWidth >> 1) / rmode->viWidth;
		iX2 = iX2 * (rmode->fbWidth >> 1) / rmode->viWidth;

		for (u32 i = iX1; i <= iX2; i++) ((u32*)xfb)[iY * (rmode->fbWidth >> 1) + i] = iColor;
	}
}


void drawVerticalLine(u32 iX, u32 iY1, u32 iY2, u32 iColor, void *xfb, GXRModeObj *rmode)
{
	if (iX >= 0 && iX < rmode->fbWidth && iY1 >= 0 && iY1 < rmode->xfbHeight && 
		iY2 >= 0 && iY2 < rmode->xfbHeight)
	{
		iX = iX * (rmode->fbWidth >> 1) / rmode->viWidth;

		for (u32 i = iY1; i <= iY2; i++) ((u32*)xfb)[i * (rmode->fbWidth >> 1) + iX] = iColor;
	}
}


void drawBox(u32 iX1, u32 iY1, u32 iX2, u32 iY2, u32 iColor, void *xfb, GXRModeObj *rmode)
{
	if (iX1 >= 0 && iX1 < rmode->fbWidth && iX2 >= 0 && iX2 < rmode->fbWidth &&
		iY1 >= 0 && iY1 < rmode->xfbHeight && iY2 >= 0 && iY2 < rmode->xfbHeight)
	{
		drawHorizontalLine(iX1, iX2, iY1, iColor, xfb, rmode);
		drawHorizontalLine(iX1, iX2, iY2, iColor, xfb, rmode);
		drawVerticalLine(iX1, iY1, iY2, iColor, xfb, rmode);
		drawVerticalLine(iX2, iY1, iY2, iColor, xfb, rmode);
	}
}


void display_jpeg(const char *pcFilename, u32 iX, u32 iY, void *xfb, GXRModeObj *rmode)
{
	if (iX < 0 || iX >= rmode->fbWidth || iY < 0 ||iY >= rmode->xfbHeight) return;

	FILE *fileJpeg = NULL;
	tjhandle tjInstance = NULL;
	u8 *pJpegBuf = NULL;
	u8 *pImgBuf = NULL;
	u64 jpegSize = 0;
	s32 iInSubsamp = 0, iInColorspace = 0, iWidth = 0, iHeight = 0;

	/* Read the JPEG file into memory. */
    fileJpeg = fopen(pcFilename, "rb");
    fseek(fileJpeg, 0, SEEK_END);
    jpegSize = (u64)ftell(fileJpeg);
    fseek(fileJpeg, 0, SEEK_SET);

    pJpegBuf = (unsigned char *)tjAlloc(jpegSize);
    fread(pJpegBuf, jpegSize, 1, fileJpeg);

    fclose(fileJpeg);
	fileJpeg = NULL;

	tjInstance = tjInitDecompress();

	tjDecompressHeader3(tjInstance, pJpegBuf, jpegSize, &iWidth, &iHeight, &iInSubsamp, &iInColorspace);

	if (iX + iWidth <= rmode->fbWidth && iY + iHeight <= rmode->xfbHeight)
	{
		pImgBuf = (unsigned char *)tjAlloc(iWidth * iHeight * tjPixelSize[TJPF_RGB]);

		tjDecompress2(tjInstance, pJpegBuf, jpegSize, pImgBuf, iWidth, 0, iHeight, TJPF_RGB, 0);

		iX = iX * (rmode->fbWidth >> 1) / rmode->viWidth;

		for (u16 i = 0; i < iHeight; i++)
		{
			for (u16 j = 0; j < (iWidth >> 1); j++)
			{
				((u32*)xfb)[(i + iY) * (rmode->fbWidth >> 1) + j + iX] = 
					rgb2yuv(pImgBuf[(i * iWidth * 3) + (j * 6)], pImgBuf[(i * iWidth * 3) + (j * 6) + 1], 
						pImgBuf[(i * iWidth * 3) + (j * 6) + 2], pImgBuf[(i * iWidth * 3) + (j * 6) + 3], 
						pImgBuf[(i * iWidth * 3) + (j * 6) + 4], pImgBuf[(i * iWidth * 3) + (j * 6) + 5]);
			}
		}
		tjFree(pImgBuf);
	}

    tjFree(pJpegBuf);  
	pJpegBuf = NULL;
    tjDestroy(tjInstance);
	tjInstance = NULL;
}


//---------------------------------------------------------------------------------
//	Convert two RGB pixels to one Y1CbY2Cr
//---------------------------------------------------------------------------------
u32	rgb2yuv (u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2) 
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