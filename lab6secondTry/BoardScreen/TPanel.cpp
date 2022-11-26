//
// Created by choleraplague on 25.11.22.
//

#include "TPanel.h"
#include "../GameSingletons/TGame.h"
#include "../GameSingletons/TTextureManager.h"
#include "../States/NNFileSystem.h"

static std::string s_sBoardScreenPath = "BoardScreen/";

TPanel::TPanel() {
	auto buttonWidth = NNFileSystem::ButtonWidth();
	auto buttonHeigth = NNFileSystem::ButtonHeight();
	auto width = TGame::Get()->Width();
	auto heigth = TGame::Get()->Height();

	#define INIT_BUTTON(xx, dx, dy, func) \
    	xx = new TVisualObject();\
		xx->Dx(dx);\
		xx->Dy(dy);\
		xx->Sx(0);\
		xx->Sx(0);\
		xx->Width(buttonWidth);\
		xx->Height(buttonHeigth);\
		xx->Renderer(TGame::Get()->Renderer()); \
        TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sBoardScreenPath + #xx+"/"+"Normal").c_str(), #xx"Normal", TGame::Get()->Renderer());\
        TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sBoardScreenPath + #xx+"/"+"Over").c_str(), #xx"Over", TGame::Get()->Renderer());\
        TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sBoardScreenPath + #xx+"/"+"Selected").c_str(), #xx"Selected", TGame::Get()->Renderer());\
		xx->StateTexture(NState::Normal, #xx"Normal");\
		xx->StateTexture(NState::Over, #xx"Over");\
		xx->StateTexture(NState::Selected, #xx"Normal");\
		xx->StateTexture(NState::OverSelected, #xx"Over");\
		xx->OnLeftDown(func);
		
		INIT_BUTTON(TakeButton, 0, 0, [this](TVisualObject* obj) {});
		INIT_BUTTON(PutButton, buttonWidth, 0, [this](TVisualObject* obj) {});
		INIT_BUTTON(EndTurnButton, buttonWidth * 2, 0, [this](TVisualObject* obj) {});
		INIT_BUTTON(QuitButton, width - buttonWidth, 0, [this](TVisualObject* obj) { OnQuitButton(obj); });
	#undef INIT_BUTTON
	
	CardsSelector = new TCardsSelector();
	PlayersPanel = new TAIPlayersPanel();
}

TPanel::~TPanel() {
	delete TakeButton;
	delete PutButton;
	delete EndTurnButton;
	delete QuitButton;
	delete CardsSelector;
	delete PlayersPanel;
}

void TPanel::OnQuitButton(TVisualObject* obj) {
	TGameStateMachine::Get()->PopState();
}

void TPanel::HandleEvents() {
	TakeButton->HandleEvents();
	PutButton->HandleEvents();
	EndTurnButton->HandleEvents();
	QuitButton->HandleEvents();
	CardsSelector->HandleEvents();
	PlayersPanel->HandleEvents();
}

void TPanel::Clean() {
	TakeButton->Clean();
	PutButton->Clean();
	EndTurnButton->Clean();
	QuitButton->Clean();
	CardsSelector->Clean();
	PlayersPanel->Clean();
}

void TPanel::Draw() {
	TakeButton->Draw();
	PutButton->Draw();
	EndTurnButton->Draw();
	QuitButton->Draw();
	CardsSelector->Draw();
	PlayersPanel->Draw();
}