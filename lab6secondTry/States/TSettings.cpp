//
// Created by choleraplague on 24.11.22.
//

#include "TSettings.h"
#include "../GameSingletons/TGame.h"
#include "../GameSingletons/TTextureManager.h"
#include "NNFileSystem.h"

static std::string s_sSettingsPath = "Settings/";


void TSettings::OnSelectPlayers(TVisualObject* obj) {
	for(auto& btn : {TwoPlayers, ThreePlayers, FourPlayers}) {
		btn->Selected(btn==obj);
	}
}

void TSettings::OnQuitButton(TVisualObject* obj) {
	TGameStateMachine::Get()->PopState();
}

TSettings::TSettings() {
	auto buttonWidth = NNFileSystem::ButtonWidth();
	auto buttonHeigth = NNFileSystem::ButtonHeight();
	auto center = TGame::Get()->Width() / 2 - buttonWidth / 2;
	auto middle = TGame::Get()->Height() / 2 - buttonHeigth / 2;

	#define INIT_BUTTON(xx, dx, dy, func) \
    	xx = new TVisualObject();\
		xx->Dx(dx);\
		xx->Dy(dy);\
		xx->Sx(0);\
		xx->Sx(0);\
		xx->Width(buttonWidth);\
		xx->Height(buttonHeigth);\
		xx->Renderer(TGame::Get()->Renderer()); \
        TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sSettingsPath + #xx+"/"+"Normal").c_str(), #xx"Normal", TGame::Get()->Renderer());\
        TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sSettingsPath + #xx+"/"+"Over").c_str(), #xx"Over", TGame::Get()->Renderer());\
        TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sSettingsPath + #xx+"/"+"Selected").c_str(), #xx"Selected", TGame::Get()->Renderer());\
		xx->StateTexture(NState::Normal, #xx"Normal");\
		xx->StateTexture(NState::Over, #xx"Over");\
		xx->StateTexture(NState::Selected, #xx"Selected");\
		xx->StateTexture(NState::OverSelected, #xx"Selected");\
		xx->OnLeftDown(func);
		
		INIT_BUTTON(TwoPlayers, center, middle - buttonHeigth, [this](TVisualObject* obj) { OnSelectPlayers(obj); });
		INIT_BUTTON(ThreePlayers, center, middle, [this](TVisualObject* obj) { OnSelectPlayers(obj); });
		INIT_BUTTON(FourPlayers, center, middle + buttonHeigth, [this](TVisualObject* obj) { OnSelectPlayers(obj); });
		INIT_BUTTON(QuitButton, center, middle + 2 * buttonHeigth, [this](TVisualObject* obj) { OnQuitButton(obj); });
	#undef INIT_BUTTON
	
	
	QuitButton->StateTexture(NState::Selected, "QuitButtonNormal");
	QuitButton->StateTexture(NState::OverSelected, "QuitButtonOver");
	OnSelectPlayers(TwoPlayers);
}

TSettings::~TSettings() {
	delete TwoPlayers;
	delete ThreePlayers;
	delete FourPlayers;
	delete QuitButton;
}


void TSettings::Clean() {
	TwoPlayers->Clean();
	ThreePlayers->Clean();
	FourPlayers->Clean();
	QuitButton->Clean();
}

void TSettings::HandleEvents() {
	TwoPlayers->HandleEvents();
	ThreePlayers->HandleEvents();
	FourPlayers->HandleEvents();
	QuitButton->HandleEvents();
}

void TSettings::Render() {
	TwoPlayers->Draw();
	ThreePlayers->Draw();
	FourPlayers->Draw();
	QuitButton->Draw();
}
