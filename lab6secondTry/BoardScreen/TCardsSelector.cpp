//
// Created by choleraplague on 25.11.22.
//

#include "TCardsSelector.h"

#include "../GameSingletons/TGame.h"
#include "../GameSingletons/TTextureManager.h"
#include "../Other/NNFileSystem.h"

#include <iostream>

static std::string s_sCardsPath = "BoardScreen/Cards/";

TCardsSelector::TCardsSelector() {
	Cards.reserve(TGame::TotalCards);
	SelectedCards.reserve(TGame::TotalCards);

	auto buttonWidth = 148;
	auto buttonHeigth = 38;
	auto cardWidth = NNFileSystem::CardWidth();
	auto cardHeigth = NNFileSystem::CardHeigth();
	auto width = TGame::Get()->ScreenWidth();
	auto heigth = TGame::Get()->ScreenHeight();
	
	TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sCardsPath + "cards").c_str(), "CardsNormal", TGame::Get()->Renderer());

	for(auto i=0;i<s_iCardSlots;++i) {
		Cards.emplace_back(new TVisualCard());
        Cards[i]->DefaultX(buttonWidth + 10*(i+1) + cardWidth*i);
        Cards[i]->DefaultY(heigth - 10 - cardHeigth);
        Cards[i]->ShiftX(0);
        Cards[i]->ShiftY(-20);
        Cards[i]->Sx(cardWidth*i);
        Cards[i]->Sy(0);
        Cards[i]->Width(cardWidth);
        Cards[i]->Height(cardHeigth);
        Cards[i]->Renderer(TGame::Get()->Renderer()); \
        Cards[i]->StateTexture(NState::Normal, "CardsNormal");
        Cards[i]->StateTexture(NState::Over, "CardsNormal");
        Cards[i]->StateTexture(NState::Selected, "CardsNormal");
        Cards[i]->StateTexture(NState::OverSelected, "CardsNormal");
        Cards[i]->OnLeftDown([this](TVisualObject* obj) { OnSelectCard(obj); });
	}
	
	#define INIT_BUTTON(xx, dx, dy, func) \
    	xx = new TVisualObject();\
		xx->Dx(dx);\
		xx->Dy(dy);\
		xx->Sx(0);\
		xx->Sx(0);\
		xx->Width(buttonWidth);\
		xx->Height(buttonHeigth);\
		xx->Renderer(TGame::Get()->Renderer()); \
        TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sCardsPath + #xx+"/"+"Normal").c_str(), #xx"Normal", TGame::Get()->Renderer());\
        TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sCardsPath + #xx+"/"+"Over").c_str(), #xx"Over", TGame::Get()->Renderer());\
		xx->StateTexture(NState::Normal, #xx"Normal");\
		xx->StateTexture(NState::Over, #xx"Over");\
		xx->StateTexture(NState::Selected, #xx"Normal");\
		xx->StateTexture(NState::OverSelected, #xx"Over");\
		xx->OnLeftDown(func);
		
		INIT_BUTTON(NextPage, 0, heigth - 3 * buttonHeigth, [this](TVisualObject* obj) {});
		INIT_BUTTON(PrevPage, 0, heigth - 2 * buttonHeigth, [this](TVisualObject* obj) {});
		INIT_BUTTON(Deselect, 0, heigth - 1 * buttonHeigth, [this](TVisualObject* obj) {});
	#undef INIT_BUTTON
	
}

void TCardsSelector::OnSelectCard(TVisualObject* obj) {
	auto card = dynamic_cast<TVisualCard*>(obj);
	if(obj->Selected()) {
		SelectedCards.push_back(card);
	} else {
		SelectedCards.erase(std::remove(SelectedCards.begin(), SelectedCards.end(), card), SelectedCards.end());
	}
	//std::cout<<SelectedCards.size()<<"\n";
}

TCardsSelector::~TCardsSelector() {
	delete NextPage;
	delete PrevPage;
	delete Deselect;
	for(auto& card : Cards) {
		delete card;
	}
}

void TCardsSelector::HandleEvents() {
	NextPage->HandleEvents();
	PrevPage->HandleEvents();
	Deselect->HandleEvents();
	for(auto& card : Cards) {
		card->HandleEvents();
	}
}

void TCardsSelector::Render() {
	NextPage->Render();
	PrevPage->Render();
	Deselect->Render();
	for(auto& card : Cards) {
		card->Render();
	}
}

void TCardsSelector::Clean() {
	NextPage->Clean();
	PrevPage->Clean();
	Deselect->Clean();
	for(auto& card : Cards) {
		card->Clean();
	}
}
