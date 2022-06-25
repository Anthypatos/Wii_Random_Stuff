#include <gctypes.h>
#include <ogc/gx_struct.h>
#include "../include/DRAW.hpp"


void DRAW_horizontalLine(s32 iX1, s32 iX2, s32 iY, u32 iColor, void *xfb, const GXRModeObj* rmode)
{
	if (iY >= 0 && iY < rmode->xfbHeight)
	{
		iX1 = iX1 * (rmode->fbWidth >> 1) / rmode->viWidth;
		iX2 = iX2 * (rmode->fbWidth >> 1) / rmode->viWidth;

		if (iX1 < 0) iX1 = 0;
		if (iX2 >= (rmode->fbWidth >> 1)) iX2 = (rmode->fbWidth >> 1) - 1;

		for (s32 i = iX1; i <= iX2; i++) static_cast<u32*>(xfb)[iY * (rmode->fbWidth >> 1) + i] = iColor;
	}
}


void DRAW_verticalLine(s32 iX, s32 iY1, s32 iY2, u32 iColor, void *xfb, const GXRModeObj* rmode)
{
	if (iX >= 0 && iX < rmode->fbWidth)
	{
		iX = iX * (rmode->fbWidth >> 1) / rmode->viWidth;

		if (iY1 < 0) iY1 = 0;
		if (iY2 >= rmode->xfbHeight) iY2 = rmode->xfbHeight - 1;

		for (s32 i = iY1; i <= iY2; i++) static_cast<u32*>(xfb)[i * (rmode->fbWidth >> 1) + iX] = iColor;
	}
}


void DRAW_box(s32 iX1, s32 iY1, s32 iX2, s32 iY2, u32 iColor, void *xfb, const GXRModeObj* rmode)
{
	DRAW_horizontalLine(iX1, iX2, iY1, iColor, xfb, rmode);
	DRAW_horizontalLine(iX1, iX2, iY2, iColor, xfb, rmode);
	DRAW_verticalLine(iX1, iY1, iY2, iColor, xfb, rmode);
	DRAW_verticalLine(iX2, iY1, iY2, iColor, xfb, rmode);
}
