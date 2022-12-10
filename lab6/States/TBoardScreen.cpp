//
// Created by choleraplague on 25.11.22.
//

#include "TBoardScreen.h"
#include "../GameSingletons/TGame.h"
#include "../GameSingletons/TTextureManager.h"
#include "../GameSingletons/TLogic.h"
#include "../Other/NNFileSystem.h"
#include "../Logic/Notify/SNextTurnNotify.h"
#include "../Logic/Notify/SAIEndTurn.h"

static int constexpr s_iObjectWidth = 158;
static int constexpr s_iObjectHeight = 35;
static int constexpr s_iCardWidth = 101;
static int constexpr s_iCardHeight = 144;
static int constexpr s_iShiftY = -90;
static const std::string s_sBoardScreenPath = "BoardScreen/";
static const std::string s_sCardsId = "CardsNormal";
static int constexpr s_iClockWidth = 30;
static int constexpr s_iClockHeight = 30;
static int constexpr s_uClockFrames = 11;
static int constexpr s_uClockInterval = 100;
static int constexpr s_uLocalCardsWidth = 5*s_iCardWidth;

TBoardScreen::TBoardScreen() {
	auto width = TGame::Get()->ScreenWidth();
	auto height = TGame::Get()->ScreenHeight();
	Dx(0);
	Dy(0);
	Width(width);
	Height(height);
	
	#define INIT(xx, parent, type, dx, dy, sx, sy, w, h) \
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
		parent->AddChild(xx);
	
		auto localBlockHeight = height - s_iObjectHeight - s_iCardHeight - 20;
		INIT(ButtonPanel, this, TAutoAlignArea, 0, height - s_iObjectHeight, 0, 0, width, s_iObjectHeight);
		INIT(AIPanel, this, TAutoAlignArea, 0, 0, 0, 0, width, s_iObjectHeight);
		INIT(UnusedCards, this, TAutoAlignArea, s_iCardWidth, (height - s_iCardHeight) / 2 - s_iCardHeight + 60, 0, 0, width - 2 * s_iCardWidth, s_iCardHeight);
		INIT(PlayCards, this, TAutoAlignArea, s_iCardWidth, (height) / 2, 0, 0, width - 2 * s_iCardWidth, s_iCardHeight);
		INIT(LocalCards, this, TAutoAlignArea, (width - s_uLocalCardsWidth) / 2, localBlockHeight, 0, 0, s_uLocalCardsWidth, height / 2);
		INIT(TrumpCard, this, TAutoAlignArea, width - 2*s_iCardWidth, localBlockHeight, 0, 0, s_iCardWidth, s_iCardHeight);
		
		
		INIT(Clock, this, TAnimationControl, (width - s_iClockWidth) / 2, (height - s_iClockHeight) / 2, 0, 0, s_iClockWidth, s_iClockHeight);
		Clock->Init(s_uClockInterval, s_uClockFrames);
		
		INIT(YouWin, this, TControl, s_iCardWidth, localBlockHeight, 0, 0, s_iObjectWidth, s_iObjectHeight);
		INIT(YouLose, this, TControl, s_iCardWidth, localBlockHeight, 0, 0, s_iObjectWidth, s_iObjectHeight);
		
		LocalCards->Align(NAlign::Central);
		PlayCards->Align(NAlign::Central);
		UnusedCards->Align(NAlign::Central);
		UnusedCards->ControlChildState(true);
		UnusedCards->Enabled(true);
		TrumpCard->ControlChildState(true);
		TrumpCard->Enabled(false);
		
		#define INIT_BUTTON(xx) \
			INIT(xx, ButtonPanel, TControl, 0, 0, 0, 0, s_iObjectWidth, s_iObjectHeight);\
			xx->OnLeftDown([this](TControl* obj) { On##xx(obj); });                                     \
			
		
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
	
	TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sBoardScreenPath + "Cards/Normal"), s_sCardsId, TGame::Get()->Renderer());\
}

TBoardScreen::~TBoardScreen() {
	for(auto* p : m_vUnknownCardsCash) {
		delete p;
	}
}

bool TBoardScreen::HandleEvents() {
	ProcessNotifies();
	return TParent::HandleEvents();
}

void TBoardScreen::ProcessNotifies() {
	auto* log = TLogic::Get();
	auto& ss = log->Session;
	auto& ntfs = ss->Notifies();
	while(not ntfs.Empty()) {
		if(auto n = std::dynamic_pointer_cast<SNextTurnNotify>(ntfs.Front())) {
			LockInterface(n->IsAI);
			Clock->Visible(n->IsAI);
			Clock->Animate(n->IsAI);
			UpdateVisuals();
			ntfs.Pop();
		} else if(auto n = std::dynamic_pointer_cast<SAIEndTurn>(ntfs.Front())) {
			log->AIThread.join();
			ss->NextTurn();
			ntfs.Pop();
		}
	}
}

void TBoardScreen::OnQuitButton(TControl* obj) {
	TGameStateMachine::Get()->PopState();
	for(auto p : m_vVisLogPairs) delete p.first;
	m_vVisLogPairs.clear();
	std::vector<TAIBlock*> ais = { AIOneLabel, AITwoLabel, AIThreeLabel };
	for(auto& ai : ais) ai->Cards->ObjectsPool().clear();
	LocalCards->ObjectsPool().clear();
	PlayCards->ObjectsPool().clear();
	UnusedCards->ObjectsPool().clear();
	TrumpCard->ObjectsPool().clear();
	Clock->Animate(false);
}

void TBoardScreen::OnPutButton(TControl* obj) {
	auto& ss = TLogic::Get()->Session;
	auto lc = FromVisToLogCards(FilterSelectedCards(LocalCards->ObjectsPool()));
	
	if(ss->TryPut(lc)) {
		UpdateVisuals();
	}
}

void TBoardScreen::OnTakeButton(TControl* obj) {
	auto& ss = TLogic::Get()->Session;
	auto lc = FromVisToLogCards(FilterSelectedCards(LocalCards->ObjectsPool()));
	auto pc = FromVisToLogCards(FilterSelectedCards(PlayCards->ObjectsPool()));
	if(ss->TryTake(lc, pc)) UpdateVisuals();
}

void TBoardScreen::OnEndTurnButton(TControl* obj) {
	auto& session = TLogic::Get()->Session;
	session->NextTurn();
}

void TBoardScreen::OnDeselectButton(TControl* obj) {
	auto& lc = LocalCards->ObjectsPool();
	auto& pc = PlayCards->ObjectsPool();
	for(auto& v : lc) v->State(NState::Normal);
	for(auto& v : pc) v->State(NState::Normal);
}

void TBoardScreen::LockInterface(bool isLock) {
	for(auto& obj : m_vObjectsPool) {
		obj->Enabled(!isLock);
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

TVisualCard* TBoardScreen::CreateCard(NCardType type, NCardValue val) {
	auto row = static_cast<int>(val);
	auto column = static_cast<int>(type)*2;
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
	return vis;
}

void TBoardScreen::SetVisualCards() {
	auto& cards = TLogic::Get()->Session->Cards();
	for(auto& c : cards) {
		auto vis = CreateCard(c->Type(), c->Value());
		m_vVisLogPairs.emplace_back(vis, c);
	}
}

void TBoardScreen::StartGame() {
	CreateSession();
	SetVisibleAILabels();
	SetVisualCards();
	UpdateVisuals();
	YouWin->Visible(false);
	YouLose->Visible(false);
	Clock->Visible(false);
}

void TBoardScreen::CreateSession() {
	auto& settings = TGame::Get()->Settings;
	auto number = settings->GetNumberOfPlayers();
	auto diff = settings->GetDifficulty();
	auto& session = TLogic::Get()->Session;
	session = std::make_shared<TSession>(diff, number);
}

std::shared_ptr<TCard> TBoardScreen::FindCardByVis(TControl* vis) {
	auto& vlp = m_vVisLogPairs;
	auto c = std::find_if(vlp.begin(), vlp.end(),
		[&vis](const auto& obj) {
			return obj.first == vis;
		}
	);
	return c->second;
}

TControl* TBoardScreen::FindVisByCard(std::shared_ptr<TCard> c) {
	auto& vlp = m_vVisLogPairs;
	auto vis = std::find_if(vlp.begin(), vlp.end(),
		[&c](const auto& obj) {
			return obj.second == c;
		}
	);
	return vis->first;
}

void TBoardScreen::UpdateVisuals() {
	OnDeselectButton(DeselectButton);
	TrumpCard->Enabled(false);
	auto& ss = TLogic::Get()->Session;
	auto& players = ss->Players();
	auto lpl = ss->LocalPlayer();
	std::vector<TAIBlock*> ais = { AIOneLabel, AITwoLabel, AIThreeLabel };
	UpdateCards(LocalCards, lpl->Cards());
	UpdateCards(PlayCards, ss->PlayCards());
	for(auto i=1;i<players.size();++i) {
		auto& ai = ais[i - 1];
		ai->Label->Selected(players[i]->Active());
		UpdateCards(ai->Cards, players[i]->Cards());
	}
	auto& uc = ss->UnusedCards();
	if(uc.empty()) {
		UpdateCards(TrumpCard, {});
		UpdateCards(UnusedCards, {});
		return;
	}
	UpdateCards(TrumpCard, {uc.front()});
	auto other = std::vector<std::shared_ptr<TCard>>();
	other.reserve(uc.size() - 1);
	std::copy(uc.begin() + 1, uc.end(), std::back_inserter(other));
	UpdateCards(UnusedCards, other);
}

void TBoardScreen::UpdateCards(TAutoAlignArea*area, const std::vector<std::shared_ptr<TCard>>& cards) {
	area->ObjectsPool().clear();
	for(auto& c : cards) {
		auto vis = FindVisByCard(c);
		area->AddChild(vis);
	}
}

std::vector<std::shared_ptr<TCard>> TBoardScreen::FromVisToLogCards(const std::vector<TControl*>& vis) {
	std::vector<std::shared_ptr<TCard>> cc;
	auto& vlp = m_vVisLogPairs;
	for(const auto& v : vis) {
		cc.emplace_back(FindCardByVis(v));
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
	return cc;
}