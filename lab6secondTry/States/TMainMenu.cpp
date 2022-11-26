//
// Created by choleraplague on 18.11.22.
//

#include "TMainMenu.h"
#include "../GameSingletons/TGame.h"
#include "../GameSingletons/TTextureManager.h"
#include "../GameSingletons/TLogic.h"
#include "../Other/NNFileSystem.h"

#include <iostream>

static std::string s_sMainMenuPath = "MainMenu/";

TMainMenu::TMainMenu() {
	auto buttonWidth = NNFileSystem::ButtonWidth();
	auto buttonHeigth = NNFileSystem::ButtonHeight();
	auto center = TGame::Get()->ScreenWidth() / 2 - buttonWidth / 2;
	auto middle = TGame::Get()->ScreenHeight() / 2 - buttonHeigth / 2;

	#define INIT_BUTTON(xx, dx, dy, func) \
    	xx = new TVisualObject();\
		xx->Dx(dx);\
		xx->Dy(dy);\
		xx->Sx(0);\
		xx->Sx(0);\
		xx->Width(buttonWidth);\
		xx->Height(buttonHeigth);\
		xx->Renderer(TGame::Get()->Renderer()); \
        TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sMainMenuPath+#xx+"/"+"Normal").c_str(), #xx"Normal", TGame::Get()->Renderer());\
        TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sMainMenuPath+#xx+"/"+"Over").c_str(), #xx"Over", TGame::Get()->Renderer());\
		xx->StateTexture(NState::Normal, #xx"Normal");\
		xx->StateTexture(NState::Over, #xx"Over");\
		xx->StateTexture(NState::Selected, #xx"Normal");\
		xx->StateTexture(NState::OverSelected, #xx"Over");\
		xx->OnLeftDown(func);
		
		INIT_BUTTON(StartButton, center, middle, [this](TVisualObject* obj) { OnStartButton(obj); });
		INIT_BUTTON(SettingsButton, center, middle + buttonHeigth, [this](TVisualObject* obj) { OnSettigsButton(obj); });
		INIT_BUTTON(QuitButton, center, middle + 2 * buttonHeigth, [this](TVisualObject* obj) { OnQuitButton(obj); });
	#undef INIT_BUTTON
}

TMainMenu::~TMainMenu() {
	delete StartButton;
	delete SettingsButton;
	delete QuitButton;
}

void TMainMenu::HandleEvents() {
	StartButton->HandleEvents();
	SettingsButton->HandleEvents();
	QuitButton->HandleEvents();
}

void TMainMenu::Clean() {
	StartButton->Clean();
	SettingsButton->Clean();
	QuitButton->Clean();
}

void TMainMenu::Render() {
	StartButton->Render();
	SettingsButton->Render();
	QuitButton->Render();
}

void TMainMenu::OnStartButton(TVisualObject* obj) {
	auto& settings = TGame::Get()->Settings;
	auto number = settings->GetNumberOfPlayers();
	auto diff = settings->GetDifficulty();
	auto& session = TLogic::Get()->Session;
	session = std::make_shared<TSession>(diff, number);
	TGameStateMachine::Get()->PushState(TGame::Get()->BoardScreen);
}

void TMainMenu::OnSettigsButton(TVisualObject* obj) {
	TGameStateMachine::Get()->PushState(TGame::Get()->Settings);
}

void TMainMenu::OnQuitButton(TVisualObject* obj) {
	TGameStateMachine::Get()->PopState();
}


