//
// Created by choleraplague on 18.11.22.
//

#ifndef UNTITLED1_TGAME_H
#define UNTITLED1_TGAME_H

#include <SDL2/SDL.h>
#include "TGameStateMachine.h"
#include "../States/TMainMenu.h"
#include "../States/TSettings.h"
#include "../States/TBoardScreen.h"

class TGame : public TSingleton<TGame> {
	public:
	TGame()=default;
	virtual ~TGame() override;
	
	virtual bool Init(const char* name, int x, int y, int w, int h, int flags);
	
	public:
	virtual SDL_Renderer* Renderer();
	
	public:
	virtual bool IsRunning();
	virtual void Quit();
	virtual void Clean();
	
	public:
	virtual void HandleEvents();
	virtual void Render();
	
	#define DECL(xx, type, prefix, val) \
		protected:            \
		type m_##prefix##xx = val;      \
        public:                        \
		virtual type xx();
	
		DECL(ScreenWidth, int, i, 0);
		DECL(ScreenHeight, int, i, 0);
	#undef DECL
	
	public:
	TMainMenu* MainMenu = nullptr;
	TSettings* Settings = nullptr;
	TBoardScreen* BoardScreen = nullptr;
	
	public:
	static constexpr int TotalCards = 52;
	
	protected:
	SDL_Window* m_pWindow = nullptr;
	SDL_Renderer* m_pRenderer = nullptr;
	
	protected:
	bool m_bRunning = true;
};


#endif //UNTITLED1_TGAME_H
