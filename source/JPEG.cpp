/**
	@file		JPEG.cpp
	@author		Juan de la Cruz Caravaca Guerrero
	@date		30/06/2022
    @brief		JPEG class
    @par		Description
                Implementation for the JPEG class
				
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <ios>
#include <new>
#include <turbojpeg.h>
#include <gctypes.h>
#include <ogc/gx_struct.h>
#include "../include/JPEG.hpp"

/** Strings for the different types of subsampling */
const std::string JPEG::SCasSubsampName[] = {"4:4:4", "4:2:2", "4:2:0", "Grayscale", "4:4:0", "4:1:1"};
/** Strings for the different types of colorspaces */
const std::string JPEG::SCasColorspaceName[] = {"RGB", "YCbCr", "GRAY", "CMYK", "YCCK"};


/**
 * @brief Construct a new JPEG object from a file in the filesystem
 * 
 * @param sFilePath the path to the image in the filesystem
 */
JPEG::JPEG(const std::string& sFilePath) : _iWidth{0}, _iHeight{0}, _iInSubsamp{0}, _iInColorspace{0},
	_iPosX{0}, _iPosY{0}, _pImgBuf{nullptr}
{
	u8* pyJpegBuf = nullptr;	// In-memory buffer for the compressed image
	u64 lJpegSize = 0;		// Size of the image in bytes

	/* Read the JPEG file into memory. */
	std::ifstream fileJpeg(sFilePath, std::ios_base::in | std::ios_base::binary | std::ios_base::ate);
	lJpegSize = fileJpeg.tellg();
	if (lJpegSize == 0) throw std::ios_base::failure("Input file contains no data");
	fileJpeg.seekg(0, std::ios_base::beg);

	if ((pyJpegBuf = tjAlloc(lJpegSize)) == nullptr) throw std::bad_alloc();
	fileJpeg.read((char*)pyJpegBuf, lJpegSize);

    fileJpeg.close();

	// Send the data to the other constructor
	*this = JPEG(pyJpegBuf, lJpegSize);

	// Free the temporary buffer
	tjFree(pyJpegBuf);  
	pyJpegBuf = nullptr;
}


/**
 * @brief Construct a new JPEG object from an image buffer
 * 
 * @param pyJpegBuf pointer to the buffer of the compressed image
 * @param lJpegSize the size of the image in bytes
 */
JPEG::JPEG(const u8* pyJpegBuf, u64 lJpegSize) : _iWidth{0}, _iHeight{0}, _iInSubsamp{0}, _iInColorspace{0},
	_iPosX{0}, _iPosY{0}, _pImgBuf{nullptr}
{
	tjhandle tjhandle = nullptr;	// Handle instance for the decompression
	u8* pyRgbBuf = nullptr;			// Buffer for the decompressed image

	if ((tjhandle = tjInitDecompress()) == nullptr) 
		throw std::runtime_error("Error initializing decompressor");

	if (tjDecompressHeader3(tjhandle, pyJpegBuf, lJpegSize, &_iWidth, &_iHeight, &_iInSubsamp, 
		&_iInColorspace) < 0) throw std::runtime_error("Error reading JPEG header");

	if ((pyRgbBuf = tjAlloc(_iWidth * _iHeight * tjPixelSize[TJPF_RGB])) == nullptr)
		throw std::bad_alloc();

	if (tjDecompress2(tjhandle, pyJpegBuf, lJpegSize, pyRgbBuf, _iWidth, 0, _iHeight, TJPF_RGB, 0) < 0)
		throw std::runtime_error("Error decompressing JPEG imagen");

	_pImgBuf = new u32[_iHeight * (_iWidth >> 1)];	// Allocate space for the converted Y1CbY2Cr image

	/* Translate the RGB values to Y1CbY2Cr and store them inside _pImgBuf */
	u32 iRow = 0, iColumn = 0;
	for (u16 i = 0; i < _iHeight; i++)	// For every scanline
	{
		iRow = i * _iWidth * tjPixelSize[TJPF_RGB];	// Offset to the i'th row. Rows have a number of color components in every pixel
		for (u16 j = 0; j < (_iWidth >> 1); j++)	// Every 2 pixels in the XFB share the same color values
		{
			iColumn = j * (tjPixelSize[TJPF_RGB] << 1);	// We leap through every 2 columns since we process 2 pixels every iteration
			/* Translate values and store them in the right offset inside the XFB */
			_pImgBuf[i * (_iWidth >> 1) + j] = 
				JPEG::rgb2yuv(pyRgbBuf[iRow + iColumn], pyRgbBuf[iRow + iColumn + 1],
					pyRgbBuf[iRow + iColumn + 2], pyRgbBuf[iRow + iColumn + 3], 
					pyRgbBuf[iRow + iColumn + 4], pyRgbBuf[iRow + iColumn + 5]);
		}
	}

	// Free resources
	tjFree(pyRgbBuf);
	pyRgbBuf = nullptr;
    tjDestroy(tjhandle);
	tjhandle = nullptr;
}


/**
 * @brief Copy constructor
 * 
 * @param jpegOther the JPEG::JPEG object to be copied
 */
JPEG::JPEG(const JPEG& jpegOther) : _iWidth{jpegOther._iWidth}, _iHeight{jpegOther._iHeight},
	_iInSubsamp{jpegOther._iInSubsamp}, _iInColorspace{jpegOther._iInColorspace}, 
	_iPosX{jpegOther._iPosX}, _iPosY{jpegOther._iPosY}, _pImgBuf{new u32[(_iWidth >> 1) * _iHeight]}
{ 
	memcpy(_pImgBuf, jpegOther._pImgBuf, (_iWidth << 1) * _iHeight); 
}


/**
 * @brief Movement constructor
 * 
 * @param jpegOther the JPEG::JPEG object to be moved
 */
JPEG::JPEG(JPEG&& jpegOther) noexcept : _iWidth{jpegOther._iWidth}, _iHeight{jpegOther._iHeight},
	_iInSubsamp{jpegOther._iInSubsamp}, _iInColorspace{jpegOther._iInColorspace}, 
	_iPosX{jpegOther._iPosX}, _iPosY{jpegOther._iPosY}, _pImgBuf{jpegOther._pImgBuf}
{
	jpegOther._iWidth = 0;
	jpegOther._iHeight = 0;
	jpegOther._iInSubsamp = 0;
	jpegOther._iInColorspace = 0;
	jpegOther._iPosX = 0;
	jpegOther._iPosY = 0;
	jpegOther._pImgBuf = nullptr;
}


/**
 * @brief Assign with copy operator
 * 
 * @param jpegOther the JPEG::JPEG object to be assigned
 * @return JPEG& a copy of the JPEG::JPEG object
 */
JPEG& JPEG::operator =(const JPEG& jpegOther)
{
	if (this != &jpegOther)
	{
		delete[] _pImgBuf;

		_iWidth = jpegOther._iWidth;
		_iHeight = jpegOther._iHeight;
		_iInSubsamp = jpegOther._iInSubsamp;
		_iInColorspace = jpegOther._iInColorspace;
		_iPosX = jpegOther._iPosX;
		_iPosY = jpegOther._iPosY;
		_pImgBuf = new u32[(_iWidth >> 1) * _iHeight];

		memcpy(_pImgBuf, jpegOther._pImgBuf, (_iWidth << 1) * _iHeight);
	}

	return *this;
}


/**
 * @brief Assignment with movement operator
 * 
 * @param jpegOther the JPEG::JPEG object to be assigned
 * @return JPEG& the moved JPEG::JPEG object
 */
JPEG& JPEG::operator =(JPEG&& jpegOther) noexcept
{
	if (this != &jpegOther)
	{
		delete[] _pImgBuf;

		_iWidth = jpegOther._iWidth;
		_iHeight = jpegOther._iHeight;
		_iInSubsamp = jpegOther._iInSubsamp;
		_iInColorspace = jpegOther._iInColorspace;
		_iPosX = jpegOther._iPosX;
		_iPosY = jpegOther._iPosY;
		_pImgBuf = jpegOther._pImgBuf;
		
		jpegOther._iWidth = 0;
		jpegOther._iHeight = 0;
		jpegOther._iInSubsamp = 0;
		jpegOther._iInColorspace = 0;
		jpegOther._iPosX = 0;
		jpegOther._iPosY = 0;
		jpegOther._pImgBuf = nullptr;
	}

	return *this;
}


/**
 * @brief Destroy the JPEG::JPEG object
 */
JPEG::~JPEG() noexcept
{
	delete[] _pImgBuf;
	_pImgBuf = nullptr;
}


/**
 * @brief Displays the JPEG on the XFB. Width or height of the image can be out
 * of the XFB depending on the given coordinates. In those cases, the image will
 * be partially displayed
 * 
 * @param pXfb a pointer to the start of the XFB region
 * @param pRmode a rendermode object holding the rendering parameters
 * @param iOriginalWidth the width of the canvas that is being drawn
 * @param iOriginalHeight the height of the canvas that is being drawn
 * @param fX the coordinate X of the top left corner of the image on the canvas
 * @param fY the coordinate Y of the top left corner of the image on the canvas
 */
void JPEG::display(void* pXfb, const GXRModeObj* pRmode, u32 iOriginalWidth, u32 iOriginalHeight, 
	f32 fX, f32 fY)
{
	if (iOriginalWidth <= 0 || iOriginalHeight <= 0) throw std::domain_error("Invalid dimensions");

	// Store the given positions
	_iPosX = fX;
	_iPosY = fY;

	// Rule of thumb to translate coordinates to the XFB
	s32 iX = fX * (pRmode->fbWidth >> 1) / iOriginalWidth;
	s32 iY = fY * pRmode->xfbHeight / iOriginalHeight;

	u32* pFrameBuffer = static_cast<u32*>(pXfb);
	bool bStop = false;	// Stop flag for the copy process

	/* Copy whatever part of the image buffer is needed to the XFB */
	for (u16 i = 0; i < _iHeight && !bStop; i++)	// For every scanline that is needed
	{
		if (iY + i < 0) i += (-iY - 1);	// If the first scanline is before the start of the XFB, jump to the first valid scanline
		else if (iY + i >= pRmode->xfbHeight) bStop = true;	// Stop if we reach the end of the XFB
		else
		{
			if (iX < 0)	// If the X coordinate is to the left of the XFB copy the relevant part into it
				memcpy(pFrameBuffer + (iY + i) * (pRmode->fbWidth >> 1), 
					_pImgBuf + i * (_iWidth >> 1) + (-iX), 
					std::min(((_iWidth >> 1) + iX) << 2, pRmode->fbWidth << 1));
			else if (iX < pRmode->fbWidth)	// If the X coordinate is inside the XFB copy the relevant part into it
				memcpy(pFrameBuffer + (iY + i) * (pRmode->fbWidth >> 1) + iX, 
					_pImgBuf + i * (_iWidth >> 1), 
					std::min(_iWidth << 1, ((pRmode->fbWidth >> 1) - iX) << 2));
		}
	}
}

/**
 * @brief Convert two RGB pixels to one Y1CbY2Cr
 * 
 * @param r1 the red component of the first pixel
 * @param g1 the green component of the first pixel
 * @param b1 the blue component of the first pixel
 * @param r2 the red component of the second pixel
 * @param g2 the green component of the second pixel
 * @param b2 the blue component of the second pixel
 * @return u32 the converted Y1CbY2Cr pixel value
 */
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
