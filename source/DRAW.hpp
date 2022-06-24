#ifndef DRAW_HPP_
#define DRAW_HPP_

#include <gctypes.h>
#include <ogc/gx_struct.h>

void drawHorizontalLine(s32 iX1, s32 iX2, s32 iY, u32 iColor, void *xfb, const GXRModeObj* rmode);
void drawVerticalLine(s32 iX, s32 iY1, s32 iY2, u32 iColor, void *xfb, const GXRModeObj* rmode);
void drawBox(s32 iX1, s32 iY1, s32 iX2, s32 iY2, u32 iColor, void *xfb, const GXRModeObj* rmode);


#endif