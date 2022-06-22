/**
	@file		PORTS.hpp
	@author		Juan de la Cruz Caravaca Guerrero
	@date		22/06/2022
    @brief		DISPLAY module
    @par		Description
                Utilities for drawing on the screen and displaying JPEGs
				
*/

#ifndef DISPLAY_HPP_
#define DISPLAY_HPP_

#include <gctypes.h>
#include <ogc/gx_struct.h>


void drawHorizontalLine(u32 iX1, u32 iX2, u32 iY, u32 iColor, void *xfb, const GXRModeObj& rmode);
void drawVerticalLine(u32 iX, u32 iY1, u32 iY2, u32 iColor, void *xfb, const GXRModeObj& rmode);
void drawBox(u32 iX1, u32 iY1, u32 iX2, u32 iY2, u32 iColor, void *xfb, const GXRModeObj& rmode);


class Jpeg
{
private:
    s32 _iWidth, _iHeight, _iInSubsamp, _iInColorspace;
    u8* _pImgBuf;

public:
    const u8* getImgBuf() const noexcept;
    s32 getWidth() const noexcept;
    s32 getHeight() const noexcept;
    s32 getInSubsamp() const noexcept;
    s32 getInColorspace() const noexcept;

    explicit Jpeg(const char *pcFilename);
    Jpeg(const Jpeg& jpegOtro) noexcept;
    ~Jpeg();

    Jpeg& operator =(const Jpeg& jpegOtro) noexcept;

    static u32 rgb2yuv(u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2) noexcept;
    void display(u32 iX, u32 iY, void* xfb, const GXRModeObj& rmode) const;
};
inline s32 Jpeg::getWidth() const noexcept { return _iWidth; }
inline s32 Jpeg::getHeight() const noexcept { return _iHeight; }
inline s32 Jpeg::getInSubsamp() const noexcept { return _iInSubsamp; }
inline s32 Jpeg::getInColorspace() const noexcept { return _iInColorspace; }
inline const u8* Jpeg::getImgBuf() const noexcept { return _pImgBuf; }


#endif
