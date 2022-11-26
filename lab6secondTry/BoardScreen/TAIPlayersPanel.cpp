//
// Created by choleraplague on 26.11.22.
//

#include "TAIPlayersPanel.h"
#include "../GameSingletons/TGame.h"
#include "../GameSingletons/TTextureManager.h"
#include "../States/NNFileSystem.h"

#define DECL(xx, type, prefix) \
    type TAIPlayersPanel::xx() { return m_##prefix##xx; }\
    void TAIPlayersPanel::xx(type vv) { m_##prefix##xx = vv; }
	
	DECL(PlayersNumber, int, i);
#undef DECL

static std::string s_sPlayersNumberPath = "BoardScreen/AIPlayersPanel/";

TAIPlayersPanel::TAIPlayersPanel() {
	m_iPlayersNumber = TGame::Get()->Settings->GetSelectedOption();
	auto buttonWidth = NNFileSystem::ButtonWidth();
	auto buttonHeigth = NNFileSystem::ButtonHeight();
	auto width = TGame::Get()->Width();
	auto heigth = TGame::Get()->Height();
	
	
	for(auto i=0;i<m_iPlayersNumber;++i) {
		auto name = "AI" + std::to_string(i+1) + "Normal";
		auto path = NNFileSystem::AssetsImagePath(s_sPlayersNumberPath + "AI" + std::to_string(i+1)+"/" "Normal");
		TTextureManager::Get()->Load(path.c_str(), name, TGame::Get()->Renderer());
		AIPlayers[i] = new TVisualObject();
        AIPlayers[i]->Dx(width - buttonWidth);
        AIPlayers[i]->Dy(buttonHeigth*(i+1));
        AIPlayers[i]->Sx(0);
        AIPlayers[i]->Sy(0);
        AIPlayers[i]->Width(buttonWidth);
        AIPlayers[i]->Height(buttonHeigth);
        AIPlayers[i]->Renderer(TGame::Get()->Renderer()); \
        AIPlayers[i]->StateTexture(NState::Normal, name);
        AIPlayers[i]->StateTexture(NState::Over, name);
        AIPlayers[i]->StateTexture(NState::Selected, name);
        AIPlayers[i]->StateTexture(NState::OverSelected, name);
	}
}

TAIPlayersPanel::~TAIPlayersPanel() {
	for(auto& ai : AIPlayers) {
		delete ai;
	}
}

void TAIPlayersPanel::HandleEvents() {
}

void TAIPlayersPanel::Draw() {
	for(auto i=0;i<m_iPlayersNumber;++i) {
		AIPlayers[i]->Draw();
	}
}

void TAIPlayersPanel::Clean() {
	for(auto i=0;i<m_iPlayersNumber;++i) {
		AIPlayers[i]->Clean();
	}
}
