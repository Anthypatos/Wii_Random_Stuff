#ifndef DRAW_HPP_
#define DRAW_HPP_

#include <gctypes.h>
#include <ogc/gx_struct.h>

void drawHorizontalLine(u32 iX1, u32 iX2, u32 iY, u32 iColor, void *xfb, const GXRModeObj& rmode);
void drawVerticalLine(u32 iX, u32 iY1, u32 iY2, u32 iColor, void *xfb, const GXRModeObj& rmode);
void drawBox(u32 iX1, u32 iY1, u32 iX2, u32 iY2, u32 iColor, void *xfb, const GXRModeObj& rmode);


#endif