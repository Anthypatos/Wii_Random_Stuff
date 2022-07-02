/**
	@file		DRAW.cpp
	@author		Juan de la Cruz Caravaca Guerrero
	@date		30/06/2022
    @brief		Drawing functions
    @par		Description
                Implementation for the drawing functions
				
*/

#include <gctypes.h>
#include <ogc/gx_struct.h>
#include "../include/DRAW.hpp"


/**
 * @brief Draws a horizontal line
 * 
 * @param xfb a pointer to the start of the XFB region
 * @param rmode a rendermode object holding the rendering parameters
 * @param iOriginalWidth the width of the canvas that is being drawn
 * @param iOriginalHeight the height of the canvas that is being drawn
 * @param iX1 the left-hand coordinate X of the line
 * @param iX2 the right-hand coordinate X of the line
 * @param iY the coordinate Y of the line
 * @param iColor the color of the line
 */
void DRAW_horizontalLine(void* xfb, const GXRModeObj* rmode, u32 iOriginalWidth, u32 iOriginalHeight, 
	s32 iX1, s32 iX2, s32 iY, u32 iColor)
{
	if (iY >= 0 && iY < rmode->xfbHeight)	// Check boundaries
	{
		// Rule of thumb to translate coordinates to the XFB
		iX1 = iX1 * (rmode->fbWidth >> 1) / iOriginalWidth;
		iX2 = iX2 * (rmode->fbWidth >> 1) / iOriginalWidth;
		iY = iY * rmode->xfbHeight / iOriginalHeight;

		// Draws the points that are inside the XFB
		if (iX1 < 0) iX1 = 0;
		if (iX2 >= (rmode->fbWidth >> 1)) iX2 = (rmode->fbWidth >> 1) - 1;

		for (s32 i = iX1; i <= iX2; i++) static_cast<u32*>(xfb)[iY * (rmode->fbWidth >> 1) + i] = iColor;
	}
}


/**
 * @brief Draws a vertical line
 * 
 * @param xfb a pointer to the start of the XFB region
 * @param rmode a rendermode object holding the rendering parameters
 * @param iOriginalWidth the width of the canvas that is being drawn
 * @param iOriginalHeight the height of the canvas that is being drawn
 * @param iX the coordinate X of the line
 * @param iY1 the upper coordinate Y of the line
 * @param iY2 the lower coordinate Y of the line
 * @param iColor the color of the line
 */
void DRAW_verticalLine(void* xfb, const GXRModeObj* rmode, u32 iOriginalWidth, u32 iOriginalHeight,
	s32 iX, s32 iY1, s32 iY2, u32 iColor)
{
	if (iX >= 0 && iX < rmode->fbWidth)		// Check boundaries
	{
		// Rule of thumb to translate coordinates to the XFB
		iX = iX * (rmode->fbWidth >> 1) / iOriginalWidth;
		iY1 = iY1 * rmode->xfbHeight / iOriginalHeight;
		iY2 = iY2 * rmode->xfbHeight / iOriginalHeight;

		// Draws the points that are inside the XFB
		if (iY1 < 0) iY1 = 0;
		if (iY2 >= rmode->xfbHeight) iY2 = rmode->xfbHeight - 1;

		for (s32 i = iY1; i <= iY2; i++) static_cast<u32*>(xfb)[i * (rmode->fbWidth >> 1) + iX] = iColor;
	}
}


/**
 * @brief Draws an empty square
 * 
 * @param xfb a pointer to the start of the XFB region
 * @param rmode a rendermode object holding the rendering parameters
 * @param iOriginalWidth the width of the canvas that is being drawn
 * @param iOriginalHeight the height of the canvas that is being drawn
 * @param iX1 the coordinate X of the top left corner of the square
 * @param iY1 the coordinate Y of the top left corner of the square
 * @param iX2 the coordinate X of the top right corner of the square
 * @param iY2 the coordinate Y of the bottom left corner of the square
 * @param iColor the color of the square's edges
 */
void DRAW_box(void* xfb, const GXRModeObj* rmode, u32 iOriginalWidth, u32 iOriginalHeight,
	s32 iX1, s32 iY1, s32 iX2, s32 iY2, u32 iColor)
{
	DRAW_horizontalLine(xfb, rmode, iOriginalWidth, iOriginalHeight, iX1, iX2, iY1, iColor);
	DRAW_horizontalLine(xfb, rmode, iOriginalWidth, iOriginalHeight, iX1, iX2, iY2, iColor);
	DRAW_verticalLine(xfb, rmode, iOriginalWidth, iOriginalHeight, iX1, iY1, iY2, iColor);
	DRAW_verticalLine(xfb, rmode, iOriginalWidth, iOriginalHeight, iX2, iY1, iY2, iColor);
}


/**
 * @brief Draws a filled square
 * 
 * @param xfb a pointer to the start of the XFB region
 * @param rmode a rendermode object holding the rendering parameters
 * @param iOriginalWidth the width of the canvas that is being drawn
 * @param iOriginalHeight the height of the canvas that is being drawn
 * @param iX1 the coordinate X of the top left corner of the square
 * @param iY1 the coordinate Y of the top left corner of the square
 * @param iX2 the coordinate X of the top right corner of the square
 * @param iY2 the coordinate Y of the bottom left corner of the square
 * @param iColor the color of the square's edges
 */
void DRAW_dot(void* xfb, const GXRModeObj* rmode, u32 iOriginalWidth, u32 iOriginalHeight,
	s32 iX1, s32 iY1, s32 iX2, s32 iY2, u32 iColor)
{
	for (s16 i = iY1; i < iY2; i++)
		DRAW_horizontalLine(xfb, rmode, iOriginalWidth, iOriginalHeight, iX1, iX2, i, iColor);
}
