#include <gctypes.h>
#include <ogc/gx_struct.h>
#include "DRAW.hpp"


void drawHorizontalLine(u32 iX1, u32 iX2, u32 iY, u32 iColor, void *xfb, const GXRModeObj& rmode)
{
	if (iX1 >= 0 && iX2 < rmode.fbWidth && iY >= 0 && iY < rmode.xfbHeight)
	{
		iX1 = iX1 * (rmode.fbWidth >> 1) / rmode.viWidth;
		iX2 = iX2 * (rmode.fbWidth >> 1) / rmode.viWidth;

		for (u32 i = iX1; i <= iX2; i++) ((u32*)xfb)[iY * (rmode.fbWidth >> 1) + i] = iColor;
	}
}


void drawVerticalLine(u32 iX, u32 iY1, u32 iY2, u32 iColor, void *xfb, const GXRModeObj& rmode)
{
	if (iX >= 0 && iX < rmode.fbWidth && iY1 >= 0 && iY2 < rmode.xfbHeight)
	{
		iX = iX * (rmode.fbWidth >> 1) / rmode.viWidth;

		for (u32 i = iY1; i <= iY2; i++) ((u32*)xfb)[i * (rmode.fbWidth >> 1) + iX] = iColor;
	}
}


void drawBox(u32 iX1, u32 iY1, u32 iX2, u32 iY2, u32 iColor, void *xfb, const GXRModeObj& rmode)
{
	if (iX1 >= 0 && iX2 < rmode.fbWidth && iY1 >= 0 && iY2 < rmode.xfbHeight)
	{
		drawHorizontalLine(iX1, iX2, iY1, iColor, xfb, rmode);
		drawHorizontalLine(iX1, iX2, iY2, iColor, xfb, rmode);
		drawVerticalLine(iX1, iY1, iY2, iColor, xfb, rmode);
		drawVerticalLine(iX2, iY1, iY2, iColor, xfb, rmode);
	}
}
