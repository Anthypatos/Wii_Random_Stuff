#ifndef _CSURFACE_HPP_
#define _CSURFACE_HPP_

#include <string>
#include <SDL.h>
#include <SDL_stdinc.h>


class CSurface 
{
    public:
        CSurface();

        static SDL_Surface* OnLoad(const std::string& CsFilePath);
        static void OnDraw(SDL_Surface* pSdlSurfaceDest, SDL_Surface* pSdlSurfaceSrc, 
            Sint32 iX, Sint32 iY);
        static void OnDraw(SDL_Surface* pSdlSurfaceDest, SDL_Surface* pSdlSurfaceSrc, 
            Sint32 iX, Sint32 iY, Sint32 iX2, Sint32 iY2, Sint32 iWidth, Sint32 iHeight);
};


#endif