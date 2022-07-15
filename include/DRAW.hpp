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
	f32 fX1, f32 fX2, f32 fY, u32 iColor);

/**
 * @brief Draws a vertical line
 * 
 * @param xfb a pointer to the start of the XFB region
 * @param pGXRmode a rendermode object holding the rendering parameters
 * @param fOriginalWidth the width of the canvas that is being drawn
 * @param fOriginalHeight the height of the canvas that is being drawn
 * @param fX the coordinate X of the line
 * @param fY1 the upper coordinate Y of the line
 * @param fY2 the lower coordinate Y of the line
 * @param iColor the color of the line
 */
void DRAW_verticalLine(void* pXfb, const GXRModeObj* pGXRmode, f32 fOriginalWidth, f32 fOriginalHeight,
	s32 fX, s32 fY1, s32 fY2, u32 iColor);

/**
 * @brief Draws an empty square
 * 
 * @param xfb a pointer to the start of the XFB region
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
	f32 fX1, f32 fY1, f32 fX2, f32 fY2, u32 iColor);


/**
 * @brief Draws a filled square
 * 
 * @param xfb a pointer to the start of the XFB region
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
	f32 fX1, f32 fY1, f32 fX2, f32 fY2, u32 iColor);


#endif