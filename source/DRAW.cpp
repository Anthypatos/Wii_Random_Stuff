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
 * @param pGXRmode a rendermode object holding the rendering parameters
 * @param fOriginalWidth the width of the canvas that is being drawn
 * @param fOriginalHeight the height of the canvas that is being drawn
 * @param fX1 the left-hand coordinate X of the line
 * @param fX2 the right-hand coordinate X of the line
 * @param fY the coordinate Y of the line
 * @param iColor the color of the line
 */
void DRAW_horizontalLine(void* pXfb, const GXRModeObj* pGXRmode, f32 fOriginalWidth, f32 fOriginalHeight, 
	f32 fX1, f32 fX2, f32 fY, u32 iColor)
{
	if (fOriginalWidth <= 0 || fOriginalHeight <= 0) throw std::domain_error("Invalid dimensions");

	// Rule of thumb to translate coordinates to the XFB
	s32 iX1 = static_cast<s32>(fX1 * (pGXRmode->fbWidth >> 1) / fOriginalWidth);
	s32 iX2 = static_cast<s32>(fX2 * (pGXRmode->fbWidth >> 1) / fOriginalWidth);
	s32 iY = static_cast<s32>(fY * pGXRmode->xfbHeight / fOriginalHeight);

	if (iY >= 0 && iY < pGXRmode->xfbHeight)	// Check boundaries
	{
		// Draws the points that are inside the XFB
		if (iX1 < 0) iX1 = 0;
		if (iX2 >= (pGXRmode->fbWidth >> 1)) iX2 = (pGXRmode->fbWidth >> 1) - 1;
		u32* pFrameBuffer = static_cast<u32*>(pXfb);

		for (s32 i = iX1; i <= iX2; i++) pFrameBuffer[iY * (pGXRmode->fbWidth >> 1) + i] = iColor;
	}
}


/**
 * @brief Draws a vertical line
 * 
 * @param pXfb a pointer to the start of the XFB region
 * @param pGXRmode a rendermode object holding the rendering parameters
 * @param fOriginalWidth the width of the canvas that is being drawn
 * @param fOriginalHeight the height of the canvas that is being drawn
 * @param fX the coordinate X of the line
 * @param fY1 the upper coordinate Y of the line
 * @param fY2 the lower coordinate Y of the line
 * @param iColor the color of the line
 */
void DRAW_verticalLine(void* pXfb, const GXRModeObj* pGXRmode, f32 fOriginalWidth, f32 fOriginalHeight,
	f32 fX, f32 fY1, f32 fY2, u32 iColor)
{
	if (fOriginalWidth <= 0 || fOriginalHeight <= 0) throw std::domain_error("Invalid dimensions");
	
	// Rule of thumb to translate coordinates to the XFB
	s32 iX = static_cast<s32>(fX * (pGXRmode->fbWidth >> 1) / fOriginalWidth);
	s32 iY1 = static_cast<s32>(fY1 * pGXRmode->xfbHeight / fOriginalHeight);
	s32 iY2 = static_cast<s32>(fY2 * pGXRmode->xfbHeight / fOriginalHeight);

	if (iX >= 0 && iX < pGXRmode->fbWidth)	// Check boundaries
	{
		// Draws the points that are inside the XFB
		if (iY1 < 0) iY1 = 0;
		if (iY2 >= pGXRmode->xfbHeight) iY2 = pGXRmode->xfbHeight - 1;
		u32* pFrameBuffer = static_cast<u32*>(pXfb);

		for (s32 i = iY1; i <= iY2; i++) pFrameBuffer[i * (pGXRmode->fbWidth >> 1) + iX] = iColor;
	}
}


/**
 * @brief Draws an empty square
 * 
 * @param pXfb a pointer to the start of the XFB region
 * @param pGXRmode a rendermode object holding the rendering parameters
 * @param fOriginalWidth the width of the canvas that is being drawn
 * @param fOriginalHeight the height of the canvas that is being drawn
 * @param fX1 the coordinate X of the top left corner of the square
 * @param fY1 the coordinate Y of the top left corner of the square
 * @param fX2 the coordinate X of the top right corner of the square
 * @param fY2 the coordinate Y of the bottom left corner of the square
 * @param iColor the color of the square's edges
 */
void DRAW_box(void* pXfb, const GXRModeObj* pGXRmode, f32 fOriginalWidth, f32 fOriginalHeight,
	f32 fX1, f32 fY1, f32 fX2, f32 fY2, u32 iColor)
{
	DRAW_horizontalLine(pXfb, pGXRmode, fOriginalWidth, fOriginalHeight, fX1, fX2, fY1, iColor);
	DRAW_horizontalLine(pXfb, pGXRmode, fOriginalWidth, fOriginalHeight, fX1, fX2, fY2, iColor);
	DRAW_verticalLine(pXfb, pGXRmode, fOriginalWidth, fOriginalHeight, fX1, fY1, fY2, iColor);
	DRAW_verticalLine(pXfb, pGXRmode, fOriginalWidth, fOriginalHeight, fX2, fY1, fY2, iColor);
}


/**
 * @brief Draws a filled square
 * 
 * @param pXfb a pointer to the start of the XFB region
 * @param pGXRmode a rendermode object holding the rendering parameters
 * @param fOriginalWidth the width of the canvas that is being drawn
 * @param fOriginalHeight the height of the canvas that is being drawn
 * @param fX1 the coordinate X of the top left corner of the square
 * @param fY1 the coordinate Y of the top left corner of the square
 * @param fX2 the coordinate X of the top right corner of the square
 * @param fY2 the coordinate Y of the bottom left corner of the square
 * @param iColor the color of the square's edges
 */
void DRAW_dot(void* pXfb, const GXRModeObj* pGXRmode, f32 fOriginalWidth, f32 fOriginalHeight,
	f32 fX1, f32 fY1, f32 fX2, f32 fY2, u32 iColor)
{
	for (f32 i = fY1; i <= fY2; i++)
		DRAW_horizontalLine(pXfb, pGXRmode, fOriginalWidth, fOriginalHeight, fX1, fX2, i, iColor);
}
