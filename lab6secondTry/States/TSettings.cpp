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

void TSettings::OnSelectDifficulty(TVisualObject* obj) {
	for(auto& btn : {EasyButton, MediumButton, HardButton}) {
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
	
	#define INIT_GENERAL(xx, dx, dy) \
    	xx = new TVisualObject();\
		xx->Dx(dx);\
		xx->Dy(dy);\
		xx->Sx(0);\
		xx->Sx(0);\
		xx->Width(buttonWidth);\
		xx->Height(buttonHeigth);\
		xx->Renderer(TGame::Get()->Renderer());\
	 	TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sSettingsPath + #xx+"/"+"Normal").c_str(), #xx"Normal", TGame::Get()->Renderer());\
		xx->StateTexture(NState::Normal, #xx"Normal");
	
		INIT_GENERAL(PlayersLabel, center - buttonWidth / 2 - buttonWidth, middle);
		INIT_GENERAL(TwoPlayers, center - buttonWidth / 2, middle);
		INIT_GENERAL(ThreePlayers, center + buttonWidth / 2, middle);
		INIT_GENERAL(FourPlayers, center + buttonWidth / 2 + buttonWidth, middle);
		INIT_GENERAL(DifficultyLabel, center -  buttonWidth / 2 - buttonWidth, middle + buttonHeigth);
		INIT_GENERAL(EasyButton, center - buttonWidth / 2, middle + buttonHeigth);
		INIT_GENERAL(MediumButton, center + buttonWidth / 2, middle + buttonHeigth);
		INIT_GENERAL(HardButton, center +  buttonWidth / 2 + buttonWidth, middle + buttonHeigth);
		INIT_GENERAL(QuitButton, center, middle + 2 * buttonHeigth);
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

TSettings::~TSettings() {
	delete TwoPlayers;
	delete ThreePlayers;
	delete FourPlayers;
	
	delete EasyButton;
	delete MediumButton;
	delete HardButton;
	
	delete DifficultyLabel;
	delete PlayersLabel;
	
	delete QuitButton;
}

int TSettings::GetSelectedOption() {
	for(auto& btn : {TwoPlayers, ThreePlayers, FourPlayers}) {
		if(btn->Selected()) {
			return atoi(btn->UserData().c_str());
		}
	}
	return 2;
}


void TSettings::Clean() {
	TwoPlayers->Clean();
	ThreePlayers->Clean();
	FourPlayers->Clean();
	
	EasyButton->Clean();
	MediumButton->Clean();
	HardButton->Clean();
	
	DifficultyLabel->Clean();
	PlayersLabel->Clean();
	
	QuitButton->Clean();
}

void TSettings::HandleEvents() {
	TwoPlayers->HandleEvents();
	ThreePlayers->HandleEvents();
	FourPlayers->HandleEvents();
	
	EasyButton->HandleEvents();
	MediumButton->HandleEvents();
	HardButton->HandleEvents();
	
	DifficultyLabel->HandleEvents();
	PlayersLabel->HandleEvents();
	
	QuitButton->HandleEvents();
}

void TSettings::Render() {
	TwoPlayers->Draw();
	ThreePlayers->Draw();
	FourPlayers->Draw();
	
	EasyButton->Draw();
	MediumButton->Draw();
	HardButton->Draw();
	
	DifficultyLabel->Draw();
	PlayersLabel->Draw();
	
	QuitButton->Draw();
}
