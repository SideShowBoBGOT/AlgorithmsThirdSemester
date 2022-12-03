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
	Dx(0);
	Dy(0);
	Width(width);
	Height(height);
	
	#define INIT(xx, parent, type, dx, dy, sx, sy, w, h, func) \
		xx = new type();\
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
		xx->OnLeftDown(func);                                     \
		parent->AddChild(xx);
	
		INIT(ButtonPanel, this, TAutoAlignArea, 0, height - s_iObjectHeight, 0, 0, width, s_iObjectHeight, [](TControl* obj) {});
		INIT(AIPanel, this, TAutoAlignArea, 0, 0, 0, 0, width, s_iObjectHeight, [](TControl* obj) {});
		INIT(LocalCards, this, TAutoAlignArea, s_iCardWidth, height - s_iObjectHeight - s_iCardHeight - 20, 0, 0, width - 2 * s_iCardWidth, height / 2, [](TControl* obj) {});
		INIT(PlayCards, this, TAutoAlignArea, s_iCardWidth, (height - s_iCardHeight) / 2, 0, 0, width - 2 * s_iCardWidth, height / 2, [](TControl* obj) {});
		
		LocalCards->Align(NAlign::Central);
		PlayCards->Align(NAlign::Central);
		
		#define INIT_BUTTON(xx) \
			INIT(xx, ButtonPanel, TControl, 0, 0, 0, 0, s_iObjectWidth, s_iObjectHeight, [this](TControl* obj) { On##xx(obj); });\
	
			INIT_BUTTON(TakeButton);
			INIT_BUTTON(PutButton);
			INIT_BUTTON(EndTurnButton);
			INIT_BUTTON(DeselectButton);
			INIT_BUTTON(QuitButton);
		#undef INIT_BUTTON
	
		#define TO_STR(xx) std::string(#xx)
		
		#define INIT_LABEL(parent, xx) \
			xx = new TAIBlock(TO_STR(xx));\
            xx->Dx(0);                     \
			xx->Dy(0);                    \
            xx->Renderer(TGame::Get()->Renderer());                     \
			parent->AddChild(xx);
			
			INIT_LABEL(AIPanel, AIOneLabel);
			INIT_LABEL(AIPanel, AITwoLabel);
			INIT_LABEL(AIPanel, AIThreeLabel);
		#undef INIT_LABEL
		#undef TO_STR
	
	#undef INIT
	
	TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sBoardScreenPath + "Cards/Normal").c_str(), s_sCardsId.c_str(), TGame::Get()->Renderer());\
}

void TBoardScreen::OnQuitButton(TControl* obj) {
	TGameStateMachine::Get()->PopState();
	for(auto p : m_vVisLogPairs) {
		delete p.first;
	}
	m_vVisLogPairs.clear();
	std::vector<TAIBlock*> ais = { AIOneLabel, AITwoLabel, AIThreeLabel };
	for(auto ai : ais) {
		ai->Cards->ObjectsPool().clear();
	}
	LocalCards->ObjectsPool().clear();
	PlayCards->ObjectsPool().clear();
}

void TBoardScreen::OnPutButton(TControl* obj) {
	auto& ss = TLogic::Get()->Session;
	auto lc = FromVisToLogCards(FilterSelectedCards(LocalCards->ObjectsPool()));
	if(ss->TryPut(lc)) UpdateVisuals();
}

void TBoardScreen::OnTakeButton(TControl* obj) {
	auto& ss = TLogic::Get()->Session;
	auto lc = FromVisToLogCards(FilterSelectedCards(LocalCards->ObjectsPool()));
	auto pc = FromVisToLogCards(FilterSelectedCards(PlayCards->ObjectsPool()));
	if(ss->TryTake(lc, pc)) UpdateVisuals();
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
	auto& lc = LocalCards->ObjectsPool();
	auto& pc = PlayCards->ObjectsPool();
	for(auto& v : lc) v->Selected(false);
	for(auto& v : pc) v->Selected(false);
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

void TBoardScreen::SetVisibleAILabels() {
	auto number = TLogic::Get()->Session->PlayersNumber() - 1;
	auto i = 0;
	for(auto& aiLabel : {AIOneLabel, AITwoLabel, AIThreeLabel}) {
		auto isHidden = i < number;
		aiLabel->Visible(isHidden);
		aiLabel->Enabled(isHidden);
		++i;
	}
	AIPanel->ALignObjects();
}

void TBoardScreen::SetVisualCards() {
	auto& cards = TLogic::Get()->Session->Cards();
	for(auto& c : cards) {
		auto row = static_cast<int>(c->Value());
		auto column = static_cast<int>(c->Type())*2;
		if(row > NCardValue::Eight) {
			++column;
			row -= static_cast<int>(NCardValue::Nine);
		}
		row *= s_iCardHeight;
		column *= s_iCardWidth;

		auto vis = new TVisualCard();
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
		m_vVisLogPairs.emplace_back(std::make_pair(vis, c));
	}
}

void TBoardScreen::StartGame() {
	SetVisibleAILabels();
	SetVisualCards();
	UpdateVisuals();
}

void TBoardScreen::UpdateVisuals() {
	auto& ss = TLogic::Get()->Session;
	auto& players = ss->Players();
	auto lpl = ss->LocalPlayer();
	std::vector<TAIBlock*> ais = { AIOneLabel, AITwoLabel, AIThreeLabel };
	UpdateCards(LocalCards, lpl->Cards());
	for(auto i=1;i<players.size();++i) {
		auto ai = ais[i - 1];
		ai->Label->Selected(players[i]->IsActive());
		UpdateCards(ais[i - 1]->Cards, players[i]->Cards());
	}
}

void TBoardScreen::UpdateCards(TAutoAlignArea*area, const std::vector<std::shared_ptr<TCard>>& cards) {
	area->ObjectsPool().clear();
	auto& vlp = m_vVisLogPairs;
	for(auto& c : cards) {
		auto vis = std::find_if(vlp.begin(), vlp.end(),
		[c](const auto& obj) {
			return obj.second == c;
		});
		area->AddChild(vis->first);
	}
}

std::vector<std::shared_ptr<TCard>> TBoardScreen::FromVisToLogCards(const std::vector<TControl*>& vis) {
	std::vector<std::shared_ptr<TCard>> cc;
	auto& vlp = m_vVisLogPairs;
	for(const auto& v : vis) {
		cc.emplace_back(
			std::find_if(vlp.begin(), vlp.end(),
				[&v](const auto& el) {
					return el.first == v;
				}
			)->second
		);
	}
	return cc;
}

std::vector<TControl*> TBoardScreen::FilterSelectedCards(const std::vector<TControl*>& vis) {
	std::vector<TControl*> cc;
	for(const auto& v : vis) {
		if(v->Selected()) {
			cc.emplace_back(v);
		}
	}
}


