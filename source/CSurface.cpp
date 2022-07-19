#include <string>
#include <ios>
#include <stdexcept>
#include <SDL_video.h>
#include <SDL_stdinc.h>
#include "../include/CSurface.hpp"


SDL_Surface* CSurface::OnLoad(const std::string& CsFilePath) 
{
    SDL_Surface* pSdlSurfaceTemp = nullptr;
    SDL_Surface* pSdlSurfaceReturn = nullptr;
 
    if((pSdlSurfaceTemp = SDL_LoadBMP(CsFilePath.c_str())) == nullptr) 
        throw std::ios_base::failure("Loading error");
 
    pSdlSurfaceReturn = SDL_DisplayFormat(pSdlSurfaceTemp);
    SDL_FreeSurface(pSdlSurfaceTemp);

    if (pSdlSurfaceReturn == nullptr) throw std::ios_base::failure("Conversion error");
 
    return pSdlSurfaceReturn;
}


void CSurface::OnDraw(SDL_Surface* pSdlSurfaceDest, SDL_Surface* pSdlSurfaceSrc, 
    Sint32 iDestX, Sint32 iDestY)
{
    if(pSdlSurfaceDest == nullptr || pSdlSurfaceSrc == nullptr) 
        throw std::invalid_argument("Surface is null");

    // Make a temporary rectangle to hold the offsets
    SDL_Rect sdlRectDest;

    // Give the offsets to the rectangle
    sdlRectDest.x = iDestX;
    sdlRectDest.y = iDestY;

    SDL_BlitSurface(pSdlSurfaceSrc, nullptr, pSdlSurfaceDest, &sdlRectDest);
}


void CSurface::OnDraw(SDL_Surface* pSdlSurfaceDest, SDL_Surface* pSdlSurfaceSrc, 
    Sint32 iDestX, Sint32 iDestY, Sint32 iSrcX, Sint32 iSrcY, Sint32 iSrcWidth, Sint32 iSrcHeight) 
{
    if(pSdlSurfaceDest == nullptr || pSdlSurfaceSrc == nullptr) 
        throw std::invalid_argument("Surface is null");
 
    SDL_Rect sdlRectDest;
 
    sdlRectDest.x = iDestX;
    sdlRectDest.y = iDestY;
 
    SDL_Rect sdlRectSrc;
 
    sdlRectSrc.x = iSrcX;
    sdlRectSrc.y = iSrcY;
    sdlRectSrc.w = iSrcWidth;
    sdlRectSrc.h = iSrcHeight;
 
    SDL_BlitSurface(pSdlSurfaceSrc, &sdlRectSrc, pSdlSurfaceDest, &sdlRectDest);
}


void CSurface::Transparent(SDL_Surface* pSdlSurfaceDest, Sint32 iRed, Sint32 iGreen, Sint32 iBlue) 
{
    if(pSdlSurfaceDest == nullptr) throw std::invalid_argument("Surface is null");
 
    SDL_SetColorKey(pSdlSurfaceDest, SDL_SRCCOLORKEY | SDL_RLEACCEL, 
        SDL_MapRGB(pSdlSurfaceDest->format, iRed, iGreen, iBlue));
}
