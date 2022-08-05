/**
	@file		JPEG.cpp
	@author		Juan de la Cruz Caravaca Guerrero
	@date		30/06/2022
    @brief		JPEG class
    @par		Description
                Implementation for the JPEG class
				
*/

#include <cstdint>
#include <string>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <ios>
#include <new>
#include <turbojpeg.h>
#include <ogc/gx_struct.h>
#include "../include/JPEG.hpp"


/**
 * @brief Construct a new JPEG object from a file in the filesystem
 * 
 * @param CsFilePath the path to the image in the filesystem
 */
JPEG::JPEG(const std::string& CsFilePath) : _iWidth{0}, _iHeight{0}, _iPosX{0}, _iPosY{0},
	_apiImgBuf{nullptr}
{
	uint8_t* pyJpegBuf = nullptr;	// In-memory buffer for the compressed image
	uint64_t lJpegSize = 0;			// Size of the image in bytes

	/* Read the JPEG file into memory. */
	std::ifstream fileJpeg(CsFilePath, std::ios_base::in | std::ios_base::binary | std::ios_base::ate);
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
 * @param CpyJpegBuf pointer to the buffer of the compressed image
 * @param lJpegSize the size of the image in bytes
 */
JPEG::JPEG(const uint8_t* CpyJpegBuf, uint64_t lJpegSize) : _iWidth{0}, _iHeight{0}, _iPosX{0}, _iPosY{0},
	_apiImgBuf{nullptr}
{
	tjhandle tjhandle;				// Handle instance for the decompression
	uint8_t* pyRgbBuf = nullptr;	// Buffer for the decompressed image
	int32_t iInSubsamp = 0;			// Subsample value of the input image
	int32_t iInColorspace = 0;		// Colorspace of the input image

	if ((tjhandle = tjInitDecompress()) == nullptr) 
		throw std::runtime_error("Error initializing decompressor");

	if (tjDecompressHeader3(tjhandle, CpyJpegBuf, lJpegSize, &_iWidth, &_iHeight, &iInSubsamp, 
		&iInColorspace) == -1) throw std::runtime_error(tjGetErrorStr());

	if ((pyRgbBuf = tjAlloc(_iWidth * _iHeight * tjPixelSize[TJPF::TJPF_RGB])) == nullptr)
		throw std::bad_alloc();

	if (tjDecompress2(tjhandle, CpyJpegBuf, lJpegSize, pyRgbBuf, _iWidth, 0, _iHeight, 
		TJPF::TJPF_RGB, 0) == -1) throw std::runtime_error(tjGetErrorStr());

	_apiImgBuf = new uint32_t[_iHeight * (_iWidth >> 1)];	// Allocate space for the converted Y1CbY2Cr image

	/* Translate the RGB values to Y1CbY2Cr and store them inside _apiImgBuf */
	uint32_t iRow = 0, iColumn = 0;
	for (int16_t i = 0; i < _iHeight; i++)	// For every scanline
	{
		iRow = i * _iWidth * tjPixelSize[TJPF_RGB];	// Offset to the i'th row. Rows have a number of color components in every pixel
		for (int16_t j = 0; j < (_iWidth >> 1); j++)	// Every 2 pixels in the XFB share the same color values
		{
			iColumn = j * (tjPixelSize[TJPF_RGB] << 1);	// We leap through every 2 columns since we process 2 pixels every iteration
			/* Translate values and store them in the right offset inside the XFB */
			_apiImgBuf[i * (_iWidth >> 1) + j] = 
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
 * @param CjpegOther the JPEG::JPEG object to be copied
 */
JPEG::JPEG(const JPEG& CjpegOther) : _iWidth{CjpegOther._iWidth}, _iHeight{CjpegOther._iHeight},
	_iPosX{CjpegOther._iPosX}, _iPosY{CjpegOther._iPosY}, 
	_apiImgBuf{new uint32_t[(_iWidth >> 1) * _iHeight]}
{	std::memcpy(_apiImgBuf, CjpegOther._apiImgBuf, (_iWidth << 1) * _iHeight);	}


/**
 * @brief Movement constructor
 * 
 * @param jpegOther the JPEG::JPEG object to be moved
 */
JPEG::JPEG(JPEG&& jpegOther) noexcept : _iWidth{jpegOther._iWidth}, _iHeight{jpegOther._iHeight},
	_iPosX{jpegOther._iPosX}, _iPosY{jpegOther._iPosY}, _apiImgBuf{jpegOther._apiImgBuf}
{
	jpegOther._iWidth = 0;
	jpegOther._iHeight = 0;
	jpegOther._iPosX = 0;
	jpegOther._iPosY = 0;
	jpegOther._apiImgBuf = nullptr;
}


/**
 * @brief Assign with copy operator
 * 
 * @param CjpegOther the JPEG::JPEG object to be assigned
 * @return JPEG& a copy of the JPEG::JPEG object
 */
JPEG& JPEG::operator =(const JPEG& CjpegOther)
{
	if (this != &CjpegOther)
	{
		delete[] _apiImgBuf;

		_iWidth = CjpegOther._iWidth;
		_iHeight = CjpegOther._iHeight;
		_iPosX = CjpegOther._iPosX;
		_iPosY = CjpegOther._iPosY;
		_apiImgBuf = new uint32_t[(_iWidth >> 1) * _iHeight];

		std::memcpy(_apiImgBuf, CjpegOther._apiImgBuf, (_iWidth << 1) * _iHeight);
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
		delete[] _apiImgBuf;

		_iWidth = jpegOther._iWidth;
		_iHeight = jpegOther._iHeight;
		_iPosX = jpegOther._iPosX;
		_iPosY = jpegOther._iPosY;
		_apiImgBuf = jpegOther._apiImgBuf;
		
		jpegOther._iWidth = 0;
		jpegOther._iHeight = 0;
		jpegOther._iPosX = 0;
		jpegOther._iPosY = 0;
		jpegOther._apiImgBuf = nullptr;
	}

	return *this;
}


/**
 * @brief Destroy the JPEG::JPEG object
 */
JPEG::~JPEG() noexcept
{
	delete[] _apiImgBuf;
	_apiImgBuf = nullptr;
}


/**
 * @brief Displays the JPEG on the XFB. Width or height of the image can be out
 * of the XFB depending on the given coordinates. In those cases, the image will
 * be partially displayed
 * 
 * @param pXfb a pointer to the start of the XFB region
 * @param CpGXRmode a rendermode object holding the rendering parameters
 * @param fOriginalWidth the width of the canvas that is being drawn. If unsure, set this to the framebuffer's width
 * @param fOriginalHeight the height of the canvas that is being drawn. If unsure, set this to the framebuffer's height
 * @param fX the coordinate X of the top left corner of the image on the canvas
 * @param fY the coordinate Y of the top left corner of the image on the canvas
 */
void JPEG::display(void* pXfb, const GXRModeObj* CpGXRmode, float fOriginalWidth, float fOriginalHeight, 
	float fX, float fY)
{
	if (fOriginalWidth <= 0 || fOriginalHeight <= 0) throw std::domain_error("Invalid dimensions");

	// Store the given positions
	_iPosX = fX;
	_iPosY = fY;

	// Rule of thumb to translate coordinates to the XFB
	int32_t iX = static_cast<int32_t>(fX * (CpGXRmode->fbWidth >> 1) / fOriginalWidth);
	int32_t iY = static_cast<int32_t>(fY * CpGXRmode->xfbHeight / fOriginalHeight);

	uint32_t* pFrameBuffer = static_cast<uint32_t*>(pXfb);
	bool bStop = false;	// Stop flag for the copy process

	/* Copy whatever part of the image buffer is needed to the XFB */
	for (int16_t i = 0; i < _iHeight && !bStop; i++)	// For every scanline that is needed
	{
		if (iY + i < 0) i += (-iY - 1);	// If the first scanline is before the start of the XFB, jump to the first valid scanline
		else if (iY + i >= CpGXRmode->xfbHeight) bStop = true;	// Stop if we reach the end of the XFB
		else
		{
			if (iX < 0)	// If the X coordinate is to the left of the XFB copy the relevant part into it
				std::memcpy(pFrameBuffer + (iY + i) * (CpGXRmode->fbWidth >> 1), 
					_apiImgBuf + i * (_iWidth >> 1) + (-iX), 
					std::min(((_iWidth >> 1) + iX) << 2, CpGXRmode->fbWidth << 1));
			else if (iX < CpGXRmode->fbWidth)	// If the X coordinate is inside the XFB copy the relevant part into it
				std::memcpy(pFrameBuffer + (iY + i) * (CpGXRmode->fbWidth >> 1) + iX, 
					_apiImgBuf + i * (_iWidth >> 1), 
					std::min(_iWidth << 1, ((CpGXRmode->fbWidth >> 1) - iX) << 2));
		}
	}
}


/**
 * @brief Convert two RGB pixels to one Y1CbY2Cr
 * 
 * @param yR1 the red component of the first pixel
 * @param yG1 the green component of the first pixel
 * @param yB1 the blue component of the first pixel
 * @param yR2 the red component of the second pixel
 * @param yG2 the green component of the second pixel
 * @param yB2 the blue component of the second pixel
 * @return uint32_t the converted Y1CbY2Cr pixel value
 */
uint32_t JPEG::rgb2yuv (uint8_t yR1, uint8_t yG1, uint8_t yB1, uint8_t yR2, uint8_t yG2, uint8_t yB2) noexcept
{
	int32_t iY1 = 0, iCb1 = 0, iCr1 = 0, iY2 = 0, iCb2 = 0, iCr2 = 0, iCb = 0, iCr = 0;

	iY1 = (299 * yR1 + 587 * yG1 + 114 * yB1) / 1000;
	iCb1 = (-16874 * yR1 - 33126 * yG1 + 50000 * yB1 + 12800000) / 100000;
	iCr1 = (50000 * yR1 - 41869 * yG1 - 8131 * yB1 + 12800000) / 100000;
	iY2 = (299 * yR2 + 587 * yG2 + 114 * yB2) / 1000;
	iCb2 = (-16874 * yR2 - 33126 * yG2 + 50000 * yB2 + 12800000) / 100000;
	iCr2 = (50000 * yR2 - 41869 * yG2 - 8131 * yB2 + 12800000) / 100000;

	iCb = (iCb1 + iCb2) >> 1;
	iCr = (iCr1 + iCr2) >> 1;
	return (iY1 << 24) | (iCb << 16) | (iY2 << 8) | iCr;
}
