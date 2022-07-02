/**
	@file		DRAW.hpp
	@author		Juan de la Cruz Caravaca Guerrero
	@date		22/06/2022
    @brief		Drawing functions
    @par		Description
                Functions for drawing simple objects
				
*/

#ifndef DRAW_HPP_
#define DRAW_HPP_

#include <gctypes.h>
#include <ogc/gx_struct.h>

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
	s32 iX1, s32 iX2, s32 iY, u32 iColor);

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
	s32 iX, s32 iY1, s32 iY2, u32 iColor);

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
	s32 iX1, s32 iY1, s32 iX2, s32 iY2, u32 iColor);


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
	s32 iX1, s32 iY1, s32 iX2, s32 iY2, u32 iColor);


#endif