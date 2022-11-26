//
// Created by choleraplague on 24.11.22.
//

#include "TSettings.h"
#include "../GameSingletons/TGame.h"
#include "../GameSingletons/TTextureManager.h"
#include "../Other/NNFileSystem.h"

static int constexpr s_iButtonWidth = 158;
static int constexpr s_iButtonHeigth = 35;
static std::string s_sSettingsPath = "Settings/";

TSettings::TSettings() {
	auto center = TGame::Get()->ScreenWidth() / 2 - s_iButtonWidth / 2;
	auto middle = TGame::Get()->ScreenHeight() / 2 - s_iButtonHeigth / 2;
	
	#define INIT_GENERAL(xx, dx, dy) \
    	xx = CreateObject();\
		xx->Dx(dx);\
		xx->Dy(dy);\
		xx->Sx(0);\
		xx->Sx(0);\
		xx->Width(s_iButtonWidth);\
		xx->Height(s_iButtonHeigth);\
		xx->Renderer(TGame::Get()->Renderer());\
	 	TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sSettingsPath + #xx+"/"+"Normal").c_str(), #xx"Normal", TGame::Get()->Renderer());\
		xx->StateTexture(NState::Normal, #xx"Normal");
	
		INIT_GENERAL(PlayersLabel, center - s_iButtonWidth / 2 - s_iButtonWidth, middle);
		INIT_GENERAL(TwoPlayers, center - s_iButtonWidth / 2, middle);
		INIT_GENERAL(ThreePlayers, center + s_iButtonWidth / 2, middle);
		INIT_GENERAL(FourPlayers, center + s_iButtonWidth / 2 + s_iButtonWidth, middle);
		INIT_GENERAL(DifficultyLabel, center -  s_iButtonWidth / 2 - s_iButtonWidth, middle + s_iButtonHeigth);
		INIT_GENERAL(EasyButton, center - s_iButtonWidth / 2, middle + s_iButtonHeigth);
		INIT_GENERAL(MediumButton, center + s_iButtonWidth / 2, middle + s_iButtonHeigth);
		INIT_GENERAL(HardButton, center +  s_iButtonWidth / 2 + s_iButtonWidth, middle + s_iButtonHeigth);
		INIT_GENERAL(QuitButton, center, middle + 2 * s_iButtonHeigth);
	#undef INIT_GENERAL
	
	#define INIT_BUTTON(xx, userData, func) \
		TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sSettingsPath + #xx+"/"+"Selected").c_str(), #xx"Selected", TGame::Get()->Renderer());\
		TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sSettingsPath + #xx+"/"+"Over").c_str(), #xx"Over", TGame::Get()->Renderer());\
		xx->StateTexture(NState::Over, #xx"Over");\
		xx->StateTexture(NState::Selected, #xx"Selected");\
		xx->StateTexture(NState::OverSelected, #xx"Selected");                                                                                             \
		xx->OnLeftDown(func);                  \
		xx->UserData(userData);
		
		INIT_BUTTON(TwoPlayers, "2", [this](TVisualObject* obj) { OnSelectPlayers(obj); });
		INIT_BUTTON(ThreePlayers, "3", [this](TVisualObject* obj) { OnSelectPlayers(obj); });
		INIT_BUTTON(FourPlayers, "4", [this](TVisualObject* obj) { OnSelectPlayers(obj); });
		INIT_BUTTON(EasyButton, "1", [this](TVisualObject* obj) { OnSelectDifficulty(obj); });
		INIT_BUTTON(MediumButton, "2", [this](TVisualObject* obj) { OnSelectDifficulty(obj); });
		INIT_BUTTON(HardButton, "3", [this](TVisualObject* obj) { OnSelectDifficulty(obj); });
		INIT_BUTTON(QuitButton, "", [this](TVisualObject* obj) { OnQuitButton(obj); obj->Selected(false); });
	#undef INIT_BUTTON
	
	#define INIT_LABEL(xx) \
		xx->StateTexture(NState::Over, #xx"Normal");\
		xx->StateTexture(NState::Selected, #xx"Normal");\
		xx->StateTexture(NState::OverSelected, #xx"Normal");
	
		INIT_LABEL(DifficultyLabel);
		INIT_LABEL(PlayersLabel);
	#undef INIT_LABEL
	
	OnSelectPlayers(TwoPlayers);
	OnSelectDifficulty(MediumButton);
}

int TSettings::GetNumberOfPlayers() {
	for(auto& btn : {TwoPlayers, ThreePlayers, FourPlayers}) {
		if(btn->Selected()) {
			return atoi(btn->UserData().c_str());
		}
	}
	return 2;
}

NDifficulty TSettings::GetDifficulty() {
	for(auto& btn : {EasyButton, MediumButton, HardButton}) {
		if(btn->Selected()) {
			return static_cast<NDifficulty>(atoi(btn->UserData().c_str()));
		}
	}
	return NDifficulty::Medium;
}

void TSettings::OnSelectPlayers(TVisualObject* obj) {
	for(auto& btn : {TwoPlayers, ThreePlayers, FourPlayers}) {
		btn->Selected(btn==obj);
	}
}

void TSettings::OnSelectDifficulty(TVisualObject* obj) {
	for(auto& btn : {EasyButton, MediumButton, HardButton}) {
		btn->Selected(btn==obj);
	}
}

void TSettings::OnQuitButton(TVisualObject* obj) {
	TGameStateMachine::Get()->PopState();
}


