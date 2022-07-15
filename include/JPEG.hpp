/**
	@file		JPEG.hpp
	@author		Juan de la Cruz Caravaca Guerrero
	@date		22/06/2022
    @brief		JPEG class
    @par		Description
                Class for decoding and displaying JPEGs
				
*/

#ifndef JPEG_HPP_
#define JPEG_HPP_

#include <string>
#include <gctypes.h>
#include <ogc/gx_struct.h>

/**
 * @brief Class for decoding and displaying JPEGs
 */
class JPEG
{
public:
    /** Getters */
    s32 getWidth() const noexcept;
    s32 getHeight() const noexcept;
    s32 getPosX() const noexcept;
    s32 getPosY() const noexcept;
    const u32* getImgBuf() const noexcept;

    /**
     * @brief Construct a new JPEG object from a file in the filesystem
     * @param sFilePath the path to the image in the filesystem
     */
    explicit JPEG(const std::string& sFilePath);

    /**
     * @brief Construct a new JPEG object from an image buffer
     * @param pyJpegBuf pointer to the buffer of the compressed image
     * @param lJpegSize the size of the image in bytes
     */
    explicit JPEG(const u8* pyJpegBuf, u64 lJpegSize);
    
    JPEG(const JPEG& jpegOther);        /**< Copy constructor */
    JPEG(JPEG&& jpegOther) noexcept;    /**< Movement constructor */
    ~JPEG() noexcept;                   /**< Destructor */

    JPEG& operator =(const JPEG& jpegOtro);         /**< Assign with copy operator */
    JPEG& operator =(JPEG&& jpegOtro) noexcept;     /**< Assign with movement operator */

    /**
     * @brief Displays the JPEG on the XFB. Width or height of the image can be out
     * of the XFB depending on the given coordinates. In those cases, the image will
     * be partially displayed
     * 
     * @param pXfb a pointer to the start of the XFB region
     * @param pGXRmode a rendermode object holding the rendering parameters
     * @param fOriginalWidth the width of the canvas that is being drawn
     * @param fOriginalHeight the height of the canvas that is being drawn
     * @param fX the coordinate X of the top left corner of the image on the canvas
     * @param fY the coordinate Y of the top left corner of the image on the canvas
     */
    void display(void* pXfb, const GXRModeObj* pGXRmode, f32 fOriginalWidth, f32 fOriginalHeight, 
        f32 fX, f32 fY);

private:
    s32 _iWidth;            /**< Width of the image in pixels */
    s32 _iHeight;           /**< Height of the image in pixels */
    s32 _iPosX;             /**< Coordinate X for the top left corner of the displayed image on the canvas*/
    s32 _iPosY;             /**< Coordinate Y for the top left corner of the displayed image on the canvas*/
    u32* _pImgBuf;          /**< Image buffer in Y1CbY2Cr values */

    static u32 rgb2yuv(u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2) noexcept;
};
inline s32 JPEG::getWidth() const noexcept { return _iWidth; }
inline s32 JPEG::getHeight() const noexcept { return _iHeight; }
inline s32 JPEG::getPosX() const noexcept { return _iPosX; }
inline s32 JPEG::getPosY() const noexcept { return _iPosY; }
inline const u32* JPEG::getImgBuf() const noexcept { return _pImgBuf; }


#endif
