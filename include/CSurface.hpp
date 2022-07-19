#ifndef _CSURFACE_HPP_
#define _CSURFACE_HPP_

#include <string>
#include <SDL.h>
#include <SDL_stdinc.h>


class CSurface 
{
    public:
        static SDL_Surface* OnLoad(const std::string& CsFilePath);
        static void OnDraw(SDL_Surface* pSdlSurfaceDest, SDL_Surface* pSdlSurfaceSrc, 
            Sint32 iDestX, Sint32 iDestY);
        static void OnDraw(SDL_Surface* pSdlSurfaceDest, SDL_Surface* pSdlSurfaceSrc, 
            Sint32 iDestX, Sint32 iDestY, Sint32 iSrcX, Sint32 iSrcY, Sint32 iSrcWidth, Sint32 iSrcHeight);
        static void Transparent(SDL_Surface* pSdlSurfaceDest, Sint32 iRed, Sint32 iGreen, Sint32 iBlue);

};


#endif