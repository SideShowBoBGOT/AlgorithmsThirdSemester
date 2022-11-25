//
// Created by choleraplague on 18.11.22.
//

#include "TMainMenu.h"
#include "../GameSingletons/TGame.h"
#include "../GameSingletons/TTextureManager.h"
#include "NNFileSystem.h"

#include <iostream>

static std::string s_sMainMenuPath = "MainMenu/";

TMainMenu::TMainMenu() {
	auto buttonWidth = NNFileSystem::ButtonWidth();
	auto buttonHeigth = NNFileSystem::ButtonHeight();
	auto center = TGame::Get()->Width() / 2 - buttonWidth / 2;
	auto middle = TGame::Get()->Height() / 2 - buttonHeigth / 2;

	#define INIT_BUTTON(xx, dx, dy, func) \
    	m_p##xx = new TVisualObject();\
		m_p##xx->Dx(dx);\
		m_p##xx->Dy(dy);\
		m_p##xx->Sx(0);\
		m_p##xx->Sx(0);\
		m_p##xx->Width(buttonWidth);\
		m_p##xx->Height(buttonHeigth);\
		m_p##xx->Renderer(TGame::Get()->Renderer()); \
        TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sMainMenuPath+#xx+"/"+"Normal").c_str(), #xx"Normal", TGame::Get()->Renderer());\
        TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sMainMenuPath+#xx+"/"+"Over").c_str(), #xx"Over", TGame::Get()->Renderer());\
		m_p##xx->StateTexture(NState::Normal, #xx"Normal");\
		m_p##xx->StateTexture(NState::Over, #xx"Over");\
		m_p##xx->StateTexture(NState::Selected, #xx"Normal");\
		m_p##xx->StateTexture(NState::OverSelected, #xx"Over");\
		m_p##xx->OnLeftDown(func);
		
		INIT_BUTTON(StartButton, center, middle, [this](TVisualObject* obj) { OnStartButton(obj); });
		INIT_BUTTON(SettingsButton, center, middle + buttonHeigth, [this](TVisualObject* obj) { OnSettigsButton(obj); });
		INIT_BUTTON(QuitButton, center, middle + 2 * buttonHeigth, [this](TVisualObject* obj) { OnQuitButton(obj); });
	#undef INIT_BUTTON
}

TMainMenu::~TMainMenu() {
	delete m_pStartButton;
	delete m_pSettingsButton;
	delete m_pQuitButton;
}

void TMainMenu::HandleEvents() {
	m_pStartButton->HandleEvents();
	m_pSettingsButton->HandleEvents();
	m_pQuitButton->HandleEvents();
}

void TMainMenu::Clean() {
	m_pStartButton->Clean();
	m_pSettingsButton->Clean();
	m_pQuitButton->Clean();
}

void TMainMenu::Render() {
	m_pStartButton->Draw();
	m_pSettingsButton->Draw();
	m_pQuitButton->Draw();
}

void TMainMenu::OnStartButton(TVisualObject* obj) {
	std::cout<<"Start"<<"\n";
}

void TMainMenu::OnSettigsButton(TVisualObject* obj) {
	TGameStateMachine::Get()->PushState(TGame::Get()->Settings);
}

void TMainMenu::OnQuitButton(TVisualObject* obj) {
	TGameStateMachine::Get()->PopState();
}


