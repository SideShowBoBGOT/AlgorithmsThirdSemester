//
// Created by choleraplague on 25.11.22.
//

#include "TBoardScreen.h"
#include "../GameSingletons/TGame.h"
#include "../GameSingletons/TTextureManager.h"
#include "../GameSingletons/TLogic.h"
#include "../Other/NNFileSystem.h"

static int constexpr s_iObjectWidth = 158;
static int constexpr s_iObjectHeight = 35;
static int constexpr s_iCardWidth = 101;
static int constexpr s_iCardHeight = 144;
static const std::string s_sBoardScreenPath = "BoardScreen/";
static const std::string s_sCardsId = "CardsNormal";

TBoardScreen::TBoardScreen() {
	auto width = TGame::Get()->ScreenWidth();
	auto height = TGame::Get()->ScreenHeight();
	
	
	#define INIT(xx, parent, type, dx, dy, sx, sy, w, h, func) \
		xx = parent->CreateObject<type>();\
		xx->Dx(dx);\
		xx->Dy(dy);\
		xx->Sx(sx);\
		xx->Sy(sy);\
		xx->Width(w);\
		xx->Height(h);\
		xx->Renderer(TGame::Get()->Renderer()); \
		TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sBoardScreenPath + #xx+"/"+"Normal").c_str(), #xx"Normal", TGame::Get()->Renderer());\
        TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sBoardScreenPath + #xx+"/"+"Over").c_str(), #xx"Over", TGame::Get()->Renderer());\
        TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sBoardScreenPath + #xx+"/"+"Selected").c_str(), #xx"Selected", TGame::Get()->Renderer());\
        TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sBoardScreenPath + #xx+"/"+"OverSelected").c_str(), #xx"OverSelected", TGame::Get()->Renderer());\
		xx->StateTexture(NState::Normal, #xx"Normal");\
		xx->StateTexture(NState::Over, #xx"Over");\
		xx->StateTexture(NState::Selected, #xx"Selected");\
		xx->StateTexture(NState::OverSelected, #xx"OverSelected");\
		xx->OnLeftDown(func);
	
		INIT(ButtonPanel, this, TAutoAlignArea, 0, height - s_iObjectHeight, 0, 0, width, s_iObjectHeight, [](TControl* obj) {});
		INIT(AIPanel, this, TAutoAlignArea, 0, 0, 0, 0, width, s_iObjectHeight, [](TControl* obj) {});
		INIT(LocalCards, this, TAutoAlignArea, 100, height - s_iObjectHeight - s_iCardHeight - 20, 0, 0, width - 200, height / 2, [](TControl* obj) {});
		INIT(PlayCards, this, TAutoAlignArea, 0, s_iObjectHeight + 30, 0, 0, width, height / 2, [](TControl* obj) {});
	
		LocalCards->OnChange([this](TControl* c) { OnChangeCardsArea(c); });
		PlayCards->OnChange([this](TControl* c) { OnChangeCardsArea(c); });
		
		#define INIT_BUTTON(xx) \
				INIT(xx, ButtonPanel, TControl, 0, 0, 0, 0, s_iObjectWidth, s_iObjectHeight, [this](TControl* obj) { On##xx(obj); });\
		
				INIT_BUTTON(TakeButton);
				INIT_BUTTON(PutButton);
				INIT_BUTTON(EndTurnButton);
				INIT_BUTTON(DeselectButton);
				INIT_BUTTON(QuitButton);
		#undef INIT_BUTTON
		
		#define INIT_LABEL(xx) \
			INIT(xx, AIPanel, TControl, 0, 0, 0, 0, s_iObjectWidth, s_iObjectHeight, [this](TControl* obj) {}); \
		
			INIT_LABEL(AIOneLabel);
			INIT_LABEL(AITwoLabel);
			INIT_LABEL(AIThreeLabel);
		#undef INIT_LABEL
	
	#undef INIT

	
	TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sBoardScreenPath + "Cards/Normal").c_str(), s_sCardsId.c_str(), TGame::Get()->Renderer());\
	
	auto& cards = TLogic::Get()->Cards;
	for(auto& c : cards) {
		auto row = static_cast<int>(c->Value());
		auto column = static_cast<int>(c->Type())*2;
		if(row > NCardValue::Eight) {
			++column;
			row -= static_cast<int>(NCardValue::Nine);
		}
		row *= s_iCardHeight;
		column *= s_iCardWidth;
		
		auto vis = LocalCards->CreateObject<TVisualCard>();
		vis->Dx(0);
		vis->Dy(0);
		vis->Sx(column);
		vis->Sy(row);
		vis->Width(s_iCardWidth);
		vis->Height(s_iCardHeight);
		vis->Renderer(TGame::Get()->Renderer());
		vis->StateTexture(NState::Normal, s_sCardsId);
		vis->StateTexture(NState::Over, s_sCardsId);
		vis->StateTexture(NState::Selected, s_sCardsId);
		vis->StateTexture(NState::OverSelected, s_sCardsId);
		VisualCards.emplace_back(vis);
	}
	
	ButtonPanel->ALignObjects();
	AIPanel->ALignObjects();
	LocalCards->ALignObjects();
}

void TBoardScreen::OnQuitButton(TControl* obj) {
	TGameStateMachine::Get()->PopState();
}

void TBoardScreen::OnPutButton(TControl* obj) {

}

void TBoardScreen::OnTakeButton(TControl* obj) {

}

void TBoardScreen::OnEndTurnButton(TControl* obj) {
	auto& session = TLogic::Get()->Session;
	session->NextPlayer();
	auto cur = session->CurrentPlayer();
	auto loc = session->LocalPlayer();
	if(cur==loc) {
		UnLockInterface();
	} else {
		LockInterface();
	}
	session->NextTurn();
}

void TBoardScreen::OnDeselectButton(TControl* obj) {

}

void TBoardScreen::LockInterface() {
	for(auto& obj : m_vObjectsPool) {
		obj->Enabled(false);
	}
}

void TBoardScreen::UnLockInterface() {
	for(auto& obj : m_vObjectsPool) {
		obj->Enabled(true);
	}
}

void TBoardScreen::OnChangeCardsArea(TControl* c) {
	auto area = dynamic_cast<TAutoAlignArea*>(c);
	area->ALignObjects();
}
