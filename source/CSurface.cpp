#include <string>
#include <ios>
#include <stdexcept>
#include <SDL_video.h>
#include <SDL_stdinc.h>
#include "../include/CSurface.hpp"


CSurface::CSurface() {}


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


void CSurface::OnDraw(SDL_Surface* pSdlSurfaceDest, SDL_Surface* pSdlSurfaceSrc, Sint32 iX, Sint32 iY)
{
    if(pSdlSurfaceDest == nullptr || pSdlSurfaceSrc == nullptr) 
        throw std::invalid_argument("Surface is null");

    // Make a temporary rectangle to hold the offsets
    SDL_Rect sdlRectDest;

    // Give the offsets to the rectangle
    sdlRectDest.x = iX;
    sdlRectDest.y = iY;

    SDL_BlitSurface(pSdlSurfaceSrc, nullptr, pSdlSurfaceDest, &sdlRectDest);
}


void CSurface::OnDraw(SDL_Surface* pSdlSurfaceDest, SDL_Surface* pSdlSurfaceSrc, Sint32 iX, Sint32 iY, 
    Sint32 iX2, Sint32 iY2, Sint32 iWidth, Sint32 iHeight) 
{
    if(pSdlSurfaceDest == nullptr || pSdlSurfaceSrc == nullptr) 
        throw std::invalid_argument("Surface is null");
 
    SDL_Rect sdlRectDest;
 
    sdlRectDest.x = iX;
    sdlRectDest.y = iY;
 
    SDL_Rect sdlRectSrc;
 
    sdlRectSrc.x = iX2;
    sdlRectSrc.y = iY2;
    sdlRectSrc.w = iWidth;
    sdlRectSrc.h = iHeight;
 
    SDL_BlitSurface(pSdlSurfaceSrc, &sdlRectSrc, pSdlSurfaceDest, &sdlRectDest);
}
