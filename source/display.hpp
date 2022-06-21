#ifndef DISPLAY_HPP_
#define DISPLAY_HPP_

#include <gctypes.h>
#include <ogc/gx_struct.h>

void drawHorizontalLine(u32 iX1, u32 iX2, u32 iY, u32 iColor, void *xfb, GXRModeObj *rmode);
void drawVerticalLine(u32 iX, u32 iY1, u32 iY2, u32 iColor, void *xfb, GXRModeObj *rmode);
void drawBox(u32 iX1, u32 iY1, u32 iX2, u32 iY2, u32 iColor, void *xfb, GXRModeObj *rmode);
void display_jpeg(const char *psFilename, u32 iX, u32 iY, void *xfb, GXRModeObj *rmode);
u32	rgb2yuv (u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2);

#endif