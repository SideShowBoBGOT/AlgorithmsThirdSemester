//
// Created by choleraplague on 18.11.22.
//

#ifndef LAB6SECONDTRY_TTEXTUREMANAGER_H
#define LAB6SECONDTRY_TTEXTUREMANAGER_H

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include "../Patterns/TSingleton.h"
class TTextureManager : public TSingleton<TTextureManager> {
	public:
	TTextureManager()=default;
	virtual ~TTextureManager() override=default;
	
	bool Load(std::string fileName, std::string id, SDL_Renderer* pRenderer);
	void Render(std::string id, int sx, int sy, int dx, int dy, int width,
	 int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
	
	
	protected:
	std::map<std::string, SDL_Texture*> m_mTextureMap;
};


#endif //LAB6SECONDTRY_TTEXTUREMANAGER_H
