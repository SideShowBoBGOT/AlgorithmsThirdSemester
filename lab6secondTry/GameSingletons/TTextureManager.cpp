//
// Created by choleraplague on 18.11.22.
//

#include "TTextureManager.h"

bool TTextureManager::Load(std::string fileName, std::string id, SDL_Renderer*pRenderer) {
	auto pTempSurface = SDL_LoadBMP(fileName.c_str());
	if(not pTempSurface) return false;
	auto pTexture = SDL_CreateTextureFromSurface(pRenderer, pTempSurface);
	SDL_FreeSurface(pTempSurface);
	if(not pTexture) return false;
	m_mTextureMap[id] = pTexture;
	return true;
}

void TTextureManager::Draw(std::string id, int sx, int sy, int dx, int dy, int width, int height,
 SDL_Renderer* pRenderer, SDL_RendererFlip flip) {
	auto srcRect = SDL_Rect();
	auto destRect = SDL_Rect();
	srcRect.x = sx;
	srcRect.y = sy;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = dx;
	destRect.y = dy;
	SDL_RenderCopyEx(pRenderer, m_mTextureMap[id], &srcRect, &destRect, NULL, NULL, flip);
}
