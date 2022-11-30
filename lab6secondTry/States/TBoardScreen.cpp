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
static int constexpr s_iCardsPanelWidht= 200;
static int constexpr s_iShiftY = -90;
static int constexpr s_iPadding = 90;
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
		INIT(LocalCards, this, TAutoAlignArea, s_iCardWidth, height - s_iObjectHeight - s_iCardHeight - 20, 0, 0, width - 2 * s_iCardWidth, height / 2, [](TControl* obj) {});
		INIT(PlayCards, this, TAutoAlignArea, s_iCardWidth, s_iObjectHeight + 30, 0, 0, width - 2 * s_iCardWidth, height / 2, [](TControl* obj) {});
		
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
	for(auto& c : cards) /*if(c->Type()==NCardType::Hearts and c->Value() < NCardValue::Eight)*/ {
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
		vis->Content->Dx(0);
		vis->Content->Dy(0);
		vis->Content->Sx(column);
		vis->Content->Sy(row);
		vis->ShiftY(s_iShiftY);
		vis->Width(s_iCardWidth);
		vis->Content->Width(s_iCardWidth);
		vis->Content->Height(s_iCardHeight);
		vis->Height(s_iCardHeight);
		vis->Renderer(TGame::Get()->Renderer());
		vis->Content->Renderer(TGame::Get()->Renderer());
		vis->Content->StateTexture(NState::Normal, s_sCardsId);
		vis->Content->StateTexture(NState::Over, s_sCardsId);
		vis->Content->StateTexture(NState::Selected, s_sCardsId);
		vis->Content->StateTexture(NState::OverSelected, s_sCardsId);
		//vis->ControlChildState(true);
		VisualCards.emplace_back(vis);
	}
	LocalCards->Align(NAlign::Central);
	PlayCards->Align(NAlign::Central);
	LocalCards->ALignObjects();
	PlayCards->ALignObjects();
//	LocalCards->ControlChildState(true);
//	PlayCards->ControlChildState(true);
//	ButtonPanel->ControlChildState(true);
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

void TBoardScreen::NumberOfVisibleAILabels() {
	auto number = TLogic::Get()->Session->PlayersNumber() - 1;
	auto i = 0;
	for(auto& aiLabel : {AIOneLabel, AITwoLabel, AIThreeLabel}) {
		auto isHidden = i < number;
		aiLabel->Visible(isHidden);
		aiLabel->Enabled(isHidden);
		++i;
	}
	
}
