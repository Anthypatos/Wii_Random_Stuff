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
#include <cstdint>
#include <ogc/gx_struct.h>

/**
 * @brief Class for decoding and displaying JPEGs
 */
class JPEG
{
public:
    /** Getters */
    int32_t getWidth() const noexcept;
    int32_t getHeight() const noexcept;
    int32_t getPosX() const noexcept;
    int32_t getPosY() const noexcept;
    const uint32_t* getImgBuf() const noexcept;

    /**
     * @brief Construct a new JPEG object from a file in the filesystem
     * @param CsFilePath the path to the image in the filesystem
     */
    explicit JPEG(const std::string& CsFilePath);

    /**
     * @brief Construct a new JPEG object from an image buffer
     * @param CpyJpegBuf pointer to the buffer of the compressed image
     * @param lJpegSize the size of the image in bytes
     */
    explicit JPEG(const uint8_t* CpyJpegBuf, u64 lJpegSize);
    
    JPEG(const JPEG& CjpegOther);        /**< Copy constructor */
    JPEG(JPEG&& jpegOther) noexcept;    /**< Movement constructor */
    ~JPEG() noexcept;                   /**< Destructor */

    JPEG& operator =(const JPEG& CjpegOtro);         /**< Assign with copy operator */
    JPEG& operator =(JPEG&& jpegOtro) noexcept;     /**< Assign with movement operator */

    /**
     * @brief Displays the JPEG on the XFB. Width or height of the image can be out
     * of the XFB depending on the given coordinates. In those cases, the image will
     * be partially displayed
     * 
     * @param pXfb a pointer to the start of the XFB region
     * @param CpGXRmode a rendermode object holding the rendering parameters
     * @param fOriginalWidth the width of the canvas that is being drawn
     * @param fOriginalHeight the height of the canvas that is being drawn
     * @param fX the coordinate X of the top left corner of the image on the canvas
     * @param fY the coordinate Y of the top left corner of the image on the canvas
     */
    void display(void* pXfb, const GXRModeObj* CpGXRmode, f32 fOriginalWidth, f32 fOriginalHeight, 
        f32 fX, f32 fY);

private:
    int32_t _iWidth;            /**< Width of the image in pixels */
    int32_t _iHeight;           /**< Height of the image in pixels */
    int32_t _iPosX;             /**< Coordinate X for the top left corner of the displayed image on the canvas*/
    int32_t _iPosY;             /**< Coordinate Y for the top left corner of the displayed image on the canvas*/
    uint32_t* _piImgBuf;        /**< Image buffer in Y1CbY2Cr values */

    static uint32_t rgb2yuv(uint8_t yR1, uint8_t yG1, uint8_t yB1, uint8_t yR2, uint8_t yG2, uint8_t yB2) noexcept;

};


inline int32_t JPEG::getWidth() const noexcept { return _iWidth; }
inline int32_t JPEG::getHeight() const noexcept { return _iHeight; }
inline int32_t JPEG::getPosX() const noexcept { return _iPosX; }
inline int32_t JPEG::getPosY() const noexcept { return _iPosY; }
inline const uint32_t* JPEG::getImgBuf() const noexcept { return _piImgBuf; }


#endif
