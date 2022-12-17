//
// Created by choleraplague on 26.11.22.
//
#include <random>

#include "TSession.h"
#include "Notify/SNextTurnNotify.h"
#include "../Logic/Notify/SNextTurnNotify.h"
#include "../Logic/Notify/SEndGameNotify.h"
#include "AI/TAI.h"

#define DECL_VECTOR(xx, type) \
    type& TSession::xx() { return m_v##xx; }\
	
	DECL_VECTOR(Players, std::vector<std::shared_ptr<TPlayer>>);
	DECL_VECTOR(Cards, std::vector<std::shared_ptr<TCard>>);
	DECL_VECTOR(PlayCards, std::vector<std::shared_ptr<TCard>>);
	DECL_VECTOR(SparseCards, std::vector<std::shared_ptr<TCard>>);
	DECL_VECTOR(UnusedCards, std::vector<std::shared_ptr<TCard>>);
#undef DECL

#define DECL(xx, type, prefix) \
    type TSession::xx() { return m_##prefix##xx; }\
	
	DECL(Difficulty, NDifficulty, x);
	DECL(PlayersNumber, int, i);
	DECL(LocalPlayer, std::shared_ptr<TPlayer>, p);
	DECL(CurrentPlayer, std::shared_ptr<TPlayer>, p);
	DECL(Trump, NCardType, x);
#undef DECL

TSession::TSession(NDifficulty diff, int playersNumber) {
	CreateCards();
	TossCards();

	m_xDifficulty = diff;
	m_iPlayersNumber = playersNumber;
	for(auto i=0;i<m_iPlayersNumber;++i) {
		auto pl = std::make_shared<TPlayer>();
		if(i!=0) pl->IsAI(true);
		m_vPlayers.emplace_back(std::move(pl));
	}
	
	DistributeCardsAmongPlayers();
	m_pLocalPlayer = m_vPlayers[0];
	m_pCurrentPlayer = m_pLocalPlayer;
	//m_vPlayCards = m_vUnusedCards;
	//RandomPlayer();
	//NextTurn();
}

void TSession::CreateCards() {
	for(auto i=0;i<NCardType::Size;++i) {
		for(auto j=0;j<NCardValue::Size;++j) {
			auto card = std::make_shared<TCard>();
			card->Type(static_cast<NCardType>(i));
			card->Value(static_cast<NCardValue>(j));
			m_vCards.emplace_back(std::move(card));
		}
	}
}

void TSession::TossCards() {
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, m_vCards.size() - 1);
	for(auto i=0;i<100000;++i) {
		auto one = 0u;
		auto two = 0u;
		while(one == two) {
			one = dist(rng);
			two = dist(rng);
		}
		std::swap(m_vCards[one], m_vCards[two]);
	}
	m_xTrump = m_vCards.front()->Type();
}

void TSession::RandomPlayer() {
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,PlayersNumber() - 1);
	m_pCurrentPlayer = Players()[dist(rng)];
}

void TSession::NextTurn() {
	LocalPlayer()->IsTookAction(false);
	if(IsEndGame()) {
		SendNotify(std::make_shared<SEndGameNotify>(IsLocalPlayerWon()));
		return;
	}
	GiveCards();
	m_pCurrentPlayer->Active(false);
	auto nextPlayer = NextPlayer(m_pCurrentPlayer);
	while(nextPlayer->Cards().empty()) {
		nextPlayer = NextPlayer(nextPlayer);
	}
	m_pCurrentPlayer = nextPlayer;
	m_pCurrentPlayer->Active(true);
	
	auto isAI = m_pCurrentPlayer->IsAI();
	SendNotify(std::make_shared<SNextTurnNotify>(isAI));
	
	if(isAI) {
		CalculateAI();
	}
}

std::shared_ptr<TPlayer> TSession::NextPlayer(const std::shared_ptr<TPlayer>& player) {
	auto it = std::find(m_vPlayers.begin(), m_vPlayers.end(), player);
	return *((it + 1 == m_vPlayers.end()) ? (m_vPlayers.begin()) : (it + 1));
};

void TSession::GiveCards() {
	auto& cc =  CurrentPlayer()->Cards();
	while(cc.size() < PlayCardsNum() and not m_vUnusedCards.empty()) {
		cc.emplace_back(m_vUnusedCards.back());
		m_vUnusedCards.pop_back();
	}
}

void TSession::DistributeCardsAmongPlayers() {
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, m_vCards.size() - 1);
	for(auto i=0;i<m_iPlayersNumber;++i) {
		auto& pcards = m_vPlayers[i]->Cards();
		for(auto k = 0;k < PlayCardsNum(); ++k) {
			auto c = m_vCards[dist(rng)];
			while(IsAnyPlayerHasCard(c)) {
				c = m_vCards[dist(rng)];
			}
			pcards.push_back(c);
		}
	}
	for(auto& c : m_vCards) {
		if(not IsAnyPlayerHasCard(c)) {
			m_vUnusedCards.emplace_back(c);
		}
	}
}

bool TSession::IsAnyPlayerHasCard(const std::shared_ptr<TCard>& c) {
	for(auto j = 0;j < m_iPlayersNumber;++j) {
		auto& p = m_vPlayers[j];
		auto& cc = p->Cards();
		if(std::find(cc.begin(), cc.end(), c) != cc.end()) {
			return true;
		}
	}
	return false;
}

bool TSession::TryEndTurn() {
	if(LocalPlayer()->IsTookAction()) {
		NextTurn();
		return true;
	}
	return false;
}


bool TSession::TryTake(const std::vector<std::shared_ptr<TCard>>& selectedOwnCards,
					const std::vector<std::shared_ptr<TCard>>& selectedPlayCards) {
	if(CurrentPlayer()->FirstMove()) return false;
	const auto& soc = selectedOwnCards;
	const auto& spc = selectedPlayCards;
	
	if(not CheckTryTake(soc, spc)) return false;
	
	auto curpl = CurrentPlayer();
	auto& cpc = curpl->Cards();
	VectorCardDifference(cpc, soc);
	auto& pc = m_vPlayCards;
	VectorCardDifference(pc, spc);
	for(auto& c : soc) m_vSparseCards.emplace_back(c);
	for(auto& c : spc) m_vSparseCards.emplace_back(c);
	LocalPlayer()->IsTookAction(true);
	return true;
}

bool TSession::TryPut(const std::vector<std::shared_ptr<TCard>>& selectedOwnCards) {
	auto& soc = selectedOwnCards;
	if(soc.empty()) {
		return false;
	}
	auto cp = CurrentPlayer();
	if(cp->FirstMove() and soc.size() > 1) {
		return false;
	}
	if(soc.size() > 4) {
		return false;
	}
	auto& plc = CurrentPlayer()->Cards();
	VectorCardDifference(plc, soc);
	for(auto& c : selectedOwnCards) {
		m_vPlayCards.emplace_back(c);
	}
	if(cp->FirstMove()) {
		cp->FirstMove(false);
		NextTurn();
	}
	LocalPlayer()->IsTookAction(true);
	return true;
}

bool TSession::TryBack(std::vector<std::shared_ptr<TCard>>& selectedPlayCards) {
	if(selectedPlayCards.empty()) return false;
	auto& spc = selectedPlayCards;
	auto& pc = m_vPlayCards;
	VectorCardDifference(pc, spc);
	std::copy(spc.begin(), spc.end(), std::back_inserter(LocalPlayer()->Cards()));
	return true;
}

bool TSession::CheckTake(const std::vector<std::shared_ptr<TCard>>& selectedCards) {
	auto size = int(selectedCards.size());
	if(size!=3 and size!=4) return false;
	
	auto& c = selectedCards[0];
	for(auto i=1;i<size;++i) {
		if(c->Value() != selectedCards[i]->Value()) {
			return false;
		}
	}
	return true;
}

bool TSession::CheckTryTake(const std::vector<std::shared_ptr<TCard>>& local,
							const std::vector<std::shared_ptr<TCard>>& play) {
	return CheckTake(local) and CheckTake(play) and local.size() == play.size()
		and (IsAnyTrump(local) or local[0]->Value() > play[0]->Value());
}

bool TSession::IsAnyTrump(const std::vector<std::shared_ptr<TCard>>& local) {
	auto trumpIt = std::find_if(local.begin(), local.end(), [this](const auto& c) {
		return c->Type()==m_xTrump;
	});
	return trumpIt!=local.end();
}

void TSession::VectorCardDifference(std::vector<std::shared_ptr<TCard>>& vOne,
									const std::vector<std::shared_ptr<TCard>>& vTwo) {
	vOne.erase(std::remove_if(vOne.begin(), vOne.end(),
		[&vTwo](const auto& el) {
			return std::find(vTwo.begin(), vTwo.end(), el) != vTwo.end();
		}
	), vOne.end());
}

TThreadSafeQueue<std::shared_ptr<INotify>>& TSession::Notifies() {
	return m_stNotifies;
}

void TSession::SendNotify(const std::shared_ptr<INotify>& n) {
	m_stNotifies.Push(n);
}

void TSession::CalculateAI() {
	TAI::Get()->Move();
}

constexpr int TSession::PlayCardsNum() {
	return 4;
}

bool TSession::IsEndGame() {
	for(auto& p : m_vPlayers) {
		if(not p->Cards().empty()) {
			return false;
		}
	}
	return true;
}

bool TSession::IsLocalPlayerWon() {
	auto p = m_vPlayers;
	std::sort(p.begin(), p.end(),
		[](const auto& lhs, const auto& rhs) {
			return lhs->Score() < rhs->Score();
		}
	);
	return p.back() == LocalPlayer();
}



#undef DECL