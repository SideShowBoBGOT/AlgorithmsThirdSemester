//
// Created by choleraplague on 26.11.22.
//
#include <random>

#include "TSession.h"
#include "Notify/SNextTurnNotify.h"

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


static int constexpr s_iPlayCards = 4;

TSession::TSession(NDifficulty diff, int playersNumber) {
	m_pAI = std::make_shared<TAI>();
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
		for(auto j=0;j<NCardValue::Size - 1;++j) {
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
	GiveCards();
	m_pCurrentPlayer->Active(false);
	auto curPlIt = std::find(m_vPlayers.begin(), m_vPlayers.end(), CurrentPlayer());
	m_pCurrentPlayer = *((curPlIt + 1 == m_vPlayers.end()) ? (m_vPlayers.begin()) : (curPlIt + 1));
	m_pCurrentPlayer->Active(true);
	
	auto isAI = m_pCurrentPlayer->IsAI();
	SendNotify(std::make_shared<SNextTurnNotify>(isAI));
	
	if(isAI) {
		CalculateAI();
	}
}

void TSession::GiveCards() {
	auto& cc =  CurrentPlayer()->Cards();
	while(cc.size() < s_iPlayCards and not m_vUnusedCards.empty()) {
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
		for(auto k = 0;k < s_iPlayCards; ++k) {
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

bool TSession::TryTake(const std::vector<std::shared_ptr<TCard>>& selectedOwnCards,
					const std::vector<std::shared_ptr<TCard>>& selectedPlayCards) {
	if(CurrentPlayer()->FirstMove()) return false;
	const auto& soc = selectedOwnCards;
	const auto& spc = selectedPlayCards;
	auto trumpIt = std::find_if(soc.begin(), soc.end(), [this](const auto& c) {
		return c->Type()==m_xTrump;
	});
	auto isAnyTrump = trumpIt!=soc.end();
	if(not CheckSelected(soc)
		or not CheckSelected(spc)
		or soc.size() != spc.size()
		or !isAnyTrump and soc[0]->Value() < spc[0]->Value()) {
			return false;
	}
	auto curpl = CurrentPlayer();
	auto& cpc = curpl->Cards();
	VectorCardDifference(cpc, soc);
	auto& pc = m_vPlayCards;
	VectorCardDifference(pc, spc);
	for(auto& c : soc) m_vSparseCards.emplace_back(c);
	for(auto& c : spc) m_vSparseCards.emplace_back(c);
	curpl->Score(curpl->Score() + 1);
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
	cp->FirstMove(false);
	return true;
}

bool TSession::CheckSelected(const std::vector<std::shared_ptr<TCard>>& selectedCards) {
	auto size = static_cast<int>(selectedCards.size());
	if(not (size==3 or size==4)) {
		return false;
	}
	auto& c = selectedCards[0];
	for(auto i=1;i<size;++i) {
		if(c != selectedCards[i]) {
			return false;
		}
	}
	return true;
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
	std::lock_guard<std::mutex> lock(m_xNotifiesMutex);
	m_stNotifies.Push(n);
}

void TSession::CalculateAI() {
	m_pAI->Move();
};
#undef DECL