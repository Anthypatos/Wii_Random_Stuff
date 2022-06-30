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
 * @param iX1 the left-hand coordinate X of the line
 * @param iX2 the right-hand coordinate X of the line
 * @param iY the coordinate Y of the line
 * @param iColor the color of the line
 * @param xfb a pointer to the start of the XFB region
 * @param rmode a rendermode object holding the rendering parameters
 */
void DRAW_horizontalLine(s32 iX1, s32 iX2, s32 iY, u32 iColor, void *xfb, const GXRModeObj* rmode)
{
	if (iY >= 0 && iY < rmode->xfbHeight)	// Check boundaries
	{
		// Rule of thumb to translate coordinates to the XFB
		iX1 = iX1 * (rmode->fbWidth >> 1) / rmode->viWidth;
		iX2 = iX2 * (rmode->fbWidth >> 1) / rmode->viWidth;
		iY = iY * rmode->xfbHeight / rmode->viHeight;

		// Draws the points that are inside the XFB
		if (iX1 < 0) iX1 = 0;
		if (iX2 >= (rmode->fbWidth >> 1)) iX2 = (rmode->fbWidth >> 1) - 1;

		for (s32 i = iX1; i <= iX2; i++) static_cast<u32*>(xfb)[iY * (rmode->fbWidth >> 1) + i] = iColor;
	}
}


/**
 * @brief Draws a vertical line
 * 
 * @param iX the coordinate X of the line
 * @param iY1 the upper coordinate Y of the line
 * @param iY2 the lower coordinate Y of the line
 * @param iColor the color of the line
 * @param xfb a pointer to the start of the XFB region
 * @param rmode a rendermode object holding the rendering parameters
 */
void DRAW_verticalLine(s32 iX, s32 iY1, s32 iY2, u32 iColor, void *xfb, const GXRModeObj* rmode)
{
	if (iX >= 0 && iX < rmode->fbWidth)		// Check boundaries
	{
		// Rule of thumb to translate coordinates to the XFB
		iX = iX * (rmode->fbWidth >> 1) / rmode->viWidth;
		iY1 = iY1 * rmode->xfbHeight / rmode->viHeight;
		iY2 = iY2 * rmode->xfbHeight / rmode->viHeight;

		// Draws the points that are inside the XFB
		if (iY1 < 0) iY1 = 0;
		if (iY2 >= rmode->xfbHeight) iY2 = rmode->xfbHeight - 1;

		for (s32 i = iY1; i <= iY2; i++) static_cast<u32*>(xfb)[i * (rmode->fbWidth >> 1) + iX] = iColor;
	}
}


/**
 * @brief Draws an empty square
 * 
 * @param iX1 the coordinate X of the top left corner of the square
 * @param iY1 the coordinate Y of the top left corner of the square
 * @param iX2 the coordinate X of the top right corner of the square
 * @param iY2 the coordinate Y of the bottom left corner of the square
 * @param iColor the color of the square's edges
 * @param xfb a pointer to the start of the XFB region
 * @param rmode a rendermode object holding the rendering parameters
 */
void DRAW_box(s32 iX1, s32 iY1, s32 iX2, s32 iY2, u32 iColor, void *xfb, const GXRModeObj* rmode)
{
	DRAW_horizontalLine(iX1, iX2, iY1, iColor, xfb, rmode);
	DRAW_horizontalLine(iX1, iX2, iY2, iColor, xfb, rmode);
	DRAW_verticalLine(iX1, iY1, iY2, iColor, xfb, rmode);
	DRAW_verticalLine(iX2, iY1, iY2, iColor, xfb, rmode);
}
