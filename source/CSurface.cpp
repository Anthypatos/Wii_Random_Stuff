#include <string>
#include <ios>
#include <stdexcept>
#include <cstdint>
#include <SDL_video.h>
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
    int32_t iDestX, int32_t iDestY)
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
    int32_t iDestX, int32_t iDestY, int32_t iSrcX, int32_t iSrcY, int32_t iSrcWidth, int32_t iSrcHeight) 
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


void CSurface::Transparent(SDL_Surface* pSdlSurfaceDest, int32_t iRed, int32_t iGreen, int32_t iBlue) 
{
    if(pSdlSurfaceDest == nullptr) throw std::invalid_argument("Surface is null");
 
    SDL_SetColorKey(pSdlSurfaceDest, SDL_SRCCOLORKEY | SDL_RLEACCEL, 
        SDL_MapRGB(pSdlSurfaceDest->format, iRed, iGreen, iBlue));
}
