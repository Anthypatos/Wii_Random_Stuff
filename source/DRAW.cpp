/**
	@file		DRAW.cpp
	@author		Juan de la Cruz Caravaca Guerrero
	@date		30/06/2022
    @brief		Drawing functions
    @par		Description
                Implementation for the drawing functions
				
*/

#include <stdexcept>
#include <cstdint>
#include <ogc/gx_struct.h>
#include "../include/DRAW.hpp"


/**
 * @brief Draws a horizontal line
 * 
 * @param pXfb a pointer to the start of the XFB region
 * @param CpGXRmode a rendermode object holding the rendering parameters
 * @param fOriginalWidth the width of the canvas that is being drawn. If unsure, set this to the framebuffer's width
 * @param fOriginalHeight the height of the canvas that is being drawn. If unsure, set this to the framebuffer's height
 * @param fX1 the left-hand coordinate X of the line
 * @param fX2 the right-hand coordinate X of the line
 * @param fY the coordinate Y of the line
 * @param iColor the color of the line
 */
void DRAW_horizontalLine(void* pXfb, const GXRModeObj* CpGXRmode, 
	float fOriginalWidth, float fOriginalHeight, float fX1, float fX2, float fY, uint32_t iColor)
{
	if (fOriginalWidth <= 0 || fOriginalHeight <= 0) throw std::domain_error("Invalid dimensions");

	// Rule of thumb to translate coordinates to the XFB
	int32_t iX1 = static_cast<int32_t>(fX1 * (CpGXRmode->fbWidth >> 1) / fOriginalWidth);
	int32_t iX2 = static_cast<int32_t>(fX2 * (CpGXRmode->fbWidth >> 1) / fOriginalWidth);
	int32_t iY = static_cast<int32_t>(fY * CpGXRmode->xfbHeight / fOriginalHeight);

	if (iY >= 0 && iY < CpGXRmode->xfbHeight)	// Check boundaries
	{
		// Draws the points that are inside the XFB
		if (iX1 < 0) iX1 = 0;
		if (iX2 >= (CpGXRmode->fbWidth >> 1)) iX2 = (CpGXRmode->fbWidth >> 1) - 1;
		uint32_t* pFrameBuffer = static_cast<uint32_t*>(pXfb);

		for (int32_t i = iX1; i <= iX2; i++) pFrameBuffer[iY * (CpGXRmode->fbWidth >> 1) + i] = iColor;
	}
}


/**
 * @brief Draws a vertical line
 * 
 * @param pXfb a pointer to the start of the XFB region
 * @param CpGXRmode a rendermode object holding the rendering parameters
 * @param fOriginalWidth the width of the canvas that is being drawn. If unsure, set this to the framebuffer's width
 * @param fOriginalHeight the height of the canvas that is being drawn. If unsure, set this to the framebuffer's height
 * @param fX the coordinate X of the line
 * @param fY1 the upper coordinate Y of the line
 * @param fY2 the lower coordinate Y of the line
 * @param iColor the color of the line
 */
void DRAW_verticalLine(void* pXfb, const GXRModeObj* CpGXRmode, 
	float fOriginalWidth, float fOriginalHeight, float fX, float fY1, float fY2, uint32_t iColor)
{
	if (fOriginalWidth <= 0 || fOriginalHeight <= 0) throw std::domain_error("Invalid dimensions");
	
	// Rule of thumb to translate coordinates to the XFB
	int32_t iX = static_cast<int32_t>(fX * (CpGXRmode->fbWidth >> 1) / fOriginalWidth);
	int32_t iY1 = static_cast<int32_t>(fY1 * CpGXRmode->xfbHeight / fOriginalHeight);
	int32_t iY2 = static_cast<int32_t>(fY2 * CpGXRmode->xfbHeight / fOriginalHeight);

	if (iX >= 0 && iX < CpGXRmode->fbWidth)	// Check boundaries
	{
		// Draws the points that are inside the XFB
		if (iY1 < 0) iY1 = 0;
		if (iY2 >= CpGXRmode->xfbHeight) iY2 = CpGXRmode->xfbHeight - 1;
		uint32_t* pFrameBuffer = static_cast<uint32_t*>(pXfb);

		for (int32_t i = iY1; i <= iY2; i++) pFrameBuffer[i * (CpGXRmode->fbWidth >> 1) + iX] = iColor;
	}
}


/**
 * @brief Draws an empty square
 * 
 * @param pXfb a pointer to the start of the XFB region
 * @param CpGXRmode a rendermode object holding the rendering parameters
 * @param fOriginalWidth the width of the canvas that is being drawn. If unsure, set this to the framebuffer's width
 * @param fOriginalHeight the height of the canvas that is being drawn. If unsure, set this to the framebuffer's height
 * @param fX1 the coordinate X of the top left corner of the square
 * @param fY1 the coordinate Y of the top left corner of the square
 * @param fX2 the coordinate X of the top right corner of the square
 * @param fY2 the coordinate Y of the bottom left corner of the square
 * @param iColor the color of the square's edges
 */
void DRAW_box(void* pXfb, const GXRModeObj* CpGXRmode, float fOriginalWidth, float fOriginalHeight,
	float fX1, float fY1, float fX2, float fY2, uint32_t iColor)
{
	DRAW_horizontalLine(pXfb, CpGXRmode, fOriginalWidth, fOriginalHeight, fX1, fX2, fY1, iColor);
	DRAW_horizontalLine(pXfb, CpGXRmode, fOriginalWidth, fOriginalHeight, fX1, fX2, fY2, iColor);
	DRAW_verticalLine(pXfb, CpGXRmode, fOriginalWidth, fOriginalHeight, fX1, fY1, fY2, iColor);
	DRAW_verticalLine(pXfb, CpGXRmode, fOriginalWidth, fOriginalHeight, fX2, fY1, fY2, iColor);
}


/**
 * @brief Draws a filled square
 * 
 * @param pXfb a pointer to the start of the XFB region
 * @param CpGXRmode a rendermode object holding the rendering parameters
 * @param fOriginalWidth the width of the canvas that is being drawn. If unsure, set this to the framebuffer's width
 * @param fOriginalHeight the height of the canvas that is being drawn. If unsure, set this to the framebuffer's height
 * @param fX1 the coordinate X of the top left corner of the square
 * @param fY1 the coordinate Y of the top left corner of the square
 * @param fX2 the coordinate X of the top right corner of the square
 * @param fY2 the coordinate Y of the bottom left corner of the square
 * @param iColor the color of the square's edges
 */
void DRAW_dot(void* pXfb, const GXRModeObj* CpGXRmode, float fOriginalWidth, float fOriginalHeight,
	float fX1, float fY1, float fX2, float fY2, uint32_t iColor)
{
	float fIncrement = fOriginalHeight / CpGXRmode->xfbHeight;

	for (float i = fY1; i <= fY2; i += fIncrement)
		DRAW_horizontalLine(pXfb, CpGXRmode, fOriginalWidth, fOriginalHeight, fX1, fX2, i, iColor);
}
