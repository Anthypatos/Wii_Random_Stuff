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
 * @param iX1 the left-hand coordinate X of the line
 * @param iX2 the right-hand coordinate X of the line
 * @param iY the coordinate Y of the line
 * @param iColor the color of the line
 * @param xfb a pointer to the start of the XFB region
 * @param rmode a rendermode object holding the rendering parameters
 */
void DRAW_horizontalLine(s32 iX1, s32 iX2, s32 iY, u32 iColor, void *xfb, const GXRModeObj* rmode);

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
void DRAW_verticalLine(s32 iX, s32 iY1, s32 iY2, u32 iColor, void *xfb, const GXRModeObj* rmode);

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
void DRAW_box(s32 iX1, s32 iY1, s32 iX2, s32 iY2, u32 iColor, void *xfb, const GXRModeObj* rmode);


#endif