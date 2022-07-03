/**
	@file		DRAW.cpp
	@author		Juan de la Cruz Caravaca Guerrero
	@date		30/06/2022
    @brief		Drawing functions
    @par		Description
                Implementation for the drawing functions
				
*/

#include <stdexcept>
#include <gctypes.h>
#include <ogc/gx_struct.h>
#include "../include/DRAW.hpp"


/**
 * @brief Draws a horizontal line
 * 
 * @param pXfb a pointer to the start of the XFB region
 * @param pRmode a rendermode object holding the rendering parameters
 * @param iOriginalWidth the width of the canvas that is being drawn
 * @param iOriginalHeight the height of the canvas that is being drawn
 * @param fX1 the left-hand coordinate X of the line
 * @param fX2 the right-hand coordinate X of the line
 * @param fY the coordinate Y of the line
 * @param iColor the color of the line
 */
void DRAW_horizontalLine(void* pXfb, const GXRModeObj* pRmode, u32 iOriginalWidth, u32 iOriginalHeight, 
	f32 fX1, f32 fX2, f32 fY, u32 iColor)
{
	if (iOriginalWidth <= 0 || iOriginalHeight <= 0) throw std::domain_error("Invalid dimensions");

	// Rule of thumb to translate coordinates to the XFB
	s32 iX1 = fX1 * (pRmode->fbWidth >> 1) / iOriginalWidth;
	s32 iX2 = fX2 * (pRmode->fbWidth >> 1) / iOriginalWidth;
	s32 iY = fY * pRmode->xfbHeight / iOriginalHeight;

	if (iY >= 0 && iY < pRmode->xfbHeight)	// Check boundaries
	{
		// Draws the points that are inside the XFB
		if (iX1 < 0) iX1 = 0;
		if (iX2 >= (pRmode->fbWidth >> 1)) iX2 = (pRmode->fbWidth >> 1) - 1;
		u32* pFrameBuffer = static_cast<u32*>(pXfb);

		for (s32 i = iX1; i <= iX2; i++) pFrameBuffer[iY * (pRmode->fbWidth >> 1) + i] = iColor;
	}
}


/**
 * @brief Draws a vertical line
 * 
 * @param pXfb a pointer to the start of the XFB region
 * @param pRmode a rendermode object holding the rendering parameters
 * @param iOriginalWidth the width of the canvas that is being drawn
 * @param iOriginalHeight the height of the canvas that is being drawn
 * @param fX the coordinate X of the line
 * @param fY1 the upper coordinate Y of the line
 * @param fY2 the lower coordinate Y of the line
 * @param iColor the color of the line
 */
void DRAW_verticalLine(void* pXfb, const GXRModeObj* pRmode, u32 iOriginalWidth, u32 iOriginalHeight,
	f32 fX, f32 fY1, f32 fY2, u32 iColor)
{
	if (iOriginalWidth <= 0 || iOriginalHeight <= 0) throw std::domain_error("Invalid dimensions");
	
	// Rule of thumb to translate coordinates to the XFB
	s32 iX = fX * (pRmode->fbWidth >> 1) / iOriginalWidth;
	s32 iY1 = fY1 * pRmode->xfbHeight / iOriginalHeight;
	s32 iY2 = fY2 * pRmode->xfbHeight / iOriginalHeight;

	if (iX >= 0 && iX < pRmode->fbWidth)	// Check boundaries
	{
		// Draws the points that are inside the XFB
		if (iY1 < 0) iY1 = 0;
		if (iY2 >= pRmode->xfbHeight) iY2 = pRmode->xfbHeight - 1;
		u32* pFrameBuffer = static_cast<u32*>(pXfb);

		for (s32 i = iY1; i <= iY2; i++) pFrameBuffer[i * (pRmode->fbWidth >> 1) + iX] = iColor;
	}
}


/**
 * @brief Draws an empty square
 * 
 * @param pXfb a pointer to the start of the XFB region
 * @param pRmode a rendermode object holding the rendering parameters
 * @param iOriginalWidth the width of the canvas that is being drawn
 * @param iOriginalHeight the height of the canvas that is being drawn
 * @param fX1 the coordinate X of the top left corner of the square
 * @param fY1 the coordinate Y of the top left corner of the square
 * @param fX2 the coordinate X of the top right corner of the square
 * @param fY2 the coordinate Y of the bottom left corner of the square
 * @param iColor the color of the square's edges
 */
void DRAW_box(void* pXfb, const GXRModeObj* pRmode, u32 iOriginalWidth, u32 iOriginalHeight,
	f32 fX1, f32 fY1, f32 fX2, f32 fY2, u32 iColor)
{
	DRAW_horizontalLine(pXfb, pRmode, iOriginalWidth, iOriginalHeight, fX1, fX2, fY1, iColor);
	DRAW_horizontalLine(pXfb, pRmode, iOriginalWidth, iOriginalHeight, fX1, fX2, fY2, iColor);
	DRAW_verticalLine(pXfb, pRmode, iOriginalWidth, iOriginalHeight, fX1, fY1, fY2, iColor);
	DRAW_verticalLine(pXfb, pRmode, iOriginalWidth, iOriginalHeight, fX2, fY1, fY2, iColor);
}


/**
 * @brief Draws a filled square
 * 
 * @param pXfb a pointer to the start of the XFB region
 * @param pRmode a rendermode object holding the rendering parameters
 * @param iOriginalWidth the width of the canvas that is being drawn
 * @param iOriginalHeight the height of the canvas that is being drawn
 * @param fX1 the coordinate X of the top left corner of the square
 * @param fY1 the coordinate Y of the top left corner of the square
 * @param fX2 the coordinate X of the top right corner of the square
 * @param fY2 the coordinate Y of the bottom left corner of the square
 * @param iColor the color of the square's edges
 */
void DRAW_dot(void* pXfb, const GXRModeObj* pRmode, u32 iOriginalWidth, u32 iOriginalHeight,
	f32 fX1, f32 fY1, f32 fX2, f32 fY2, u32 iColor)
{
	for (f32 i = fY1; i <= fY2; i++)
		DRAW_horizontalLine(pXfb, pRmode, iOriginalWidth, iOriginalHeight, fX1, fX2, i, iColor);
}
