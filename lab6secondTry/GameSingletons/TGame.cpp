//
// Created by choleraplague on 18.11.22.
//

#include "TGame.h"
#include "TInputHandler.h"

#define DECL(xx, type, prefix) \
    type TGame::xx() { return m_##prefix##xx; }
	
	DECL(Width, int, i);
	DECL(Height, int, i);
#undef DECL

bool TGame::Init(const char* name, int x, int y, int w, int h, int flags) {
	if(SDL_Init(SDL_INIT_EVERYTHING)!=0) return false;
	m_pWindow = SDL_CreateWindow(name, x, y, w, h, flags);
	m_iWidth = w;
	m_iHeight = h;
	if(m_pWindow==nullptr) return false;
	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
	if(m_pWindow==nullptr) return false;
	SDL_SetRenderDrawColor(m_pRenderer, 102, 102, 102, 1);
	MainMenu = new TMainMenu();
	Settings = new TSettings();
	TGameStateMachine::Get()->PushState(MainMenu);
	TInputHandler::Get();
	m_bRunning = true;
	return true;
}

SDL_Renderer* TGame::Renderer() {
	return m_pRenderer;
}

void TGame::HandleEvents() {
	TInputHandler::Get()->Update();
	TGameStateMachine::Get()->HandleEvents();
}

void TGame::Render() {
	TGameStateMachine::Get()->Render();
	SDL_RenderPresent(m_pRenderer);
}

bool TGame::IsRunning() {
	return m_bRunning;
}

void TGame::Clean() {
	TGameStateMachine::Get()->Clean();
	SDL_RenderClear(m_pRenderer);
	TInputHandler::Get()->Clean();
}

void TGame::Quit() {
	m_bRunning = false;
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}


