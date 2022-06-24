/**
	@file		DISPLAY.hpp
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


class JPEG
{
private:
    s32 _iWidth, _iHeight, _iInSubsamp, _iInColorspace;
    u32* _pImgBuf;

    static u32 rgb2yuv(u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2) noexcept;

public:
    static const char* subsampName[];
    static const char* colorspaceName[];

    const u32* getImgBuf() const noexcept;
    s32 getWidth() const noexcept;
    s32 getHeight() const noexcept;
    s32 getInSubsamp() const noexcept;
    s32 getInColorspace() const noexcept;

    explicit JPEG(const char* pcFilePath);
    explicit JPEG(const u8* pJpegBuf, u64 lJpegSize);
    JPEG(const JPEG& jpegOther);
    JPEG(JPEG&& jpegOther) noexcept;
    ~JPEG() noexcept;

    JPEG& operator =(const JPEG& jpegOtro);
    JPEG& operator =(JPEG&& jpegOtro) noexcept;

    void display(s32 iX, s32 iY, void* xfb, const GXRModeObj* rmode) const;
};
inline s32 JPEG::getWidth() const noexcept { return _iWidth; }
inline s32 JPEG::getHeight() const noexcept { return _iHeight; }
inline s32 JPEG::getInSubsamp() const noexcept { return _iInSubsamp; }
inline s32 JPEG::getInColorspace() const noexcept { return _iInColorspace; }
inline const u32* JPEG::getImgBuf() const noexcept { return _pImgBuf; }


#endif
