//
// Created by choleraplague on 26.11.22.
//
#include <random>

#include "TSession.h"

#define DECL_VECTOR(xx, type, prefix) \
    type& TSession::xx() { return m_##prefix##xx; }\
	
	DECL_VECTOR(Players, std::vector<std::shared_ptr<TPlayer>>, v);
	DECL_VECTOR(Cards, std::vector<std::shared_ptr<TCard>>, v);
	DECL_VECTOR(PlayCards, std::vector<std::shared_ptr<TCard>>, v);
	DECL_VECTOR(SparseCards, std::vector<std::shared_ptr<TCard>>, v);
#undef DECL

#define DECL(xx, type, prefix) \
    type TSession::xx() { return m_##prefix##xx; }\
	
	DECL(Difficulty, NDifficulty, x);
	DECL(PlayersNumber, int, i);
	DECL(LocalPlayer, std::shared_ptr<TPlayer>, p);
	DECL(CurrentPlayer, std::shared_ptr<TPlayer>, p);

#undef DECL

TSession::TSession(NDifficulty diff, int playersNumber) {

	for(auto i=0;i<NCardType::Size;++i) {
		for(auto j=0;j<NCardValue::Size;++j) {
			auto card = std::make_shared<TCard>();
			card->Type(static_cast<NCardType>(i));
			card->Value(static_cast<NCardValue>(j));
			m_vCards.emplace_back(std::move(card));
		}
	}

	m_xDifficulty = diff;
	m_iPlayersNumber = playersNumber;
	for(auto i=0;i<m_iPlayersNumber;++i) {
		m_vPlayers.emplace_back(std::make_shared<TPlayer>());
	}
	
	DistributeCardsAmongPlayers();
	m_pLocalPlayer = m_vPlayers[0];
	RandomPlayer();
	NextPlayer();
}

void TSession::RandomPlayer() {
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,PlayersNumber() - 1);
	m_pCurrentPlayer = Players()[dist(rng)];
}

void TSession::NextPlayer() {
	m_pCurrentPlayer->IsActive(false);
	auto curPlIt = std::find(m_vPlayers.begin(), m_vPlayers.end(), CurrentPlayer());
	m_pCurrentPlayer = *((curPlIt + 1 == m_vPlayers.end()) ? (m_vPlayers.begin()) : (curPlIt + 1));
	m_pCurrentPlayer->IsActive(true);
}

void TSession::NextTurn() {

}

void TSession::DistributeCardsAmongPlayers() {
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, m_vCards.size() - 1);
	for(auto i=0;i<m_iPlayersNumber;++i) {
		auto& pcards = m_vPlayers[i]->Cards();
		for(auto k = 0;k < 4; ++k) {
			std::shared_ptr<TCard> c = nullptr;
			auto isHas = true;
			//check if one of the players already has this card
			while(isHas) {
				isHas = false;
				c = m_vCards[dist(rng)];
				for(auto j = 0;j < m_iPlayersNumber;++j) {
					auto& p = m_vPlayers[j];
					auto& cc = p->Cards();
					isHas = std::find(cc.begin(), cc.end(), c) != cc.end();
					if(isHas) {
						break;
					}
				}
			}
			pcards.push_back(c);
		}
	}
}

bool TSession::TryTake(const std::vector<std::shared_ptr<TCard>>& selectedOwnCards,
					const std::vector<std::shared_ptr<TCard>>& selectedPlayCards) {
	const auto& soc = selectedOwnCards;
	const auto& spc = selectedPlayCards;
	if(not CheckSelected(soc)
		and CheckSelected(spc)
		and soc.size()==spc.size()
		and soc[0]->Value() > spc[0]->Value()) {
			return false;
	}
	auto& cpc = CurrentPlayer()->Cards();
	VectorCardDifference(cpc, soc);
	auto& pc = m_vPlayCards;
	VectorCardDifference(pc, spc);
	for(auto& c : soc) m_vSparseCards.emplace_back(c);
	for(auto& c : spc) m_vSparseCards.emplace_back(c);
	return true;
}

bool TSession::TryPut(const std::vector<std::shared_ptr<TCard>>& selectedOwnCards) {
	if(not CheckSelected(selectedOwnCards)) {
		return false;
	}
	for(auto& c : selectedOwnCards) m_vPlayCards.emplace_back(c);
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
};
#undef DECL