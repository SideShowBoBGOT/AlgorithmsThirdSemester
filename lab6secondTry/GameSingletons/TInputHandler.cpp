//
// Created by choleraplague on 19.11.22.
//

#include "TInputHandler.h"
#include "TGame.h"

void TInputHandler::Update() {
	auto event = SDL_Event();
	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT) {
			TGame::Get()->Quit();
		}
		if(event.type == SDL_MOUSEBUTTONDOWN) {
			if(event.button.button == SDL_BUTTON_LEFT) {
				m_vDowns[NMouseButton::Left] = true;
			}
			if(event.button.button == SDL_BUTTON_RIGHT) {
				m_vDowns[NMouseButton::Right] = true;
			}
			if(event.button.button == SDL_BUTTON_MIDDLE) {
				m_vDowns[NMouseButton::Right] = true;
			}
		} else if(event.type == SDL_MOUSEBUTTONUP) {
			if(event.button.button == SDL_BUTTON_LEFT) {
				m_vUps[0] = true;
			}
			if(event.button.button == SDL_BUTTON_RIGHT) {
				m_vUps[1] = true;
			}
			if(event.button.button == SDL_BUTTON_MIDDLE) {
				m_vUps[2] = true;
			}
		} else if(event.type == SDL_MOUSEMOTION){
			m_iX = event.motion.x;
			m_iY = event.motion.y;
		}
	}
	
}

void TInputHandler::Clean() {
	for(auto i=0;i<3;++i) {
		m_vDowns[i] = false;
		m_vUps[i] = false;
	}
}

void TInputHandler::CleanMousePosition() {
	m_iX = -1;
	m_iY = -1;
}

bool TInputHandler::Downs(TInputHandler::NMouseButton btn) { return m_vDowns[btn]; }

bool TInputHandler::Ups(TInputHandler::NMouseButton btn) { return m_vUps[btn]; }

int TInputHandler::X() { return m_iX; }

int TInputHandler::Y() { return m_iY; }


