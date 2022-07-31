#ifndef _CSURFACE_HPP_
#define _CSURFACE_HPP_

#include <string>
#include <cstdint>
#include <SDL.h>


class CSurface 
{
    public:
        static SDL_Surface* OnLoad(const std::string& CsFilePath);
        static void OnDraw(SDL_Surface* pSdlSurfaceDest, SDL_Surface* pSdlSurfaceSrc, int32_t iDestX, 
            int32_t iDestY);
        static void OnDraw(SDL_Surface* pSdlSurfaceDest, SDL_Surface* pSdlSurfaceSrc, int32_t iDestX, 
            int32_t iDestY, int32_t iSrcX, int32_t iSrcY, int32_t iSrcWidth, int32_t iSrcHeight);
        static void Transparent(SDL_Surface* pSdlSurfaceDest, int32_t iRed, int32_t iGreen, int32_t iBlue);

};


#endif