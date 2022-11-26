//
// Created by choleraplague on 26.11.22.
//
#include <random>

#include "TSession.h"

#define DECL(xx, type, prefix) \
    type& TSession::xx() { return m_##prefix##xx; }\
	
	DECL(Players, std::vector<std::shared_ptr<TPlayer>>, v);
	DECL(Cards, std::vector<std::shared_ptr<TCard>>, v);
#undef DECL

#define DECL(xx, type, prefix) \
    type TSession::xx() { return m_##prefix##xx; }\
	
	DECL(Difficulty, NDifficulty, x)

TSession::TSession(NDifficulty diff, int playersNumber) {
	m_xDifficulty = diff;
	m_iPlayersNumber = playersNumber;
	for(auto i=0;i<m_iPlayersNumber;++i) {
		m_vPlayers.emplace_back(std::make_shared<TPlayer>());
	}
	m_pLocalPlayer = m_vPlayers[0];
	m_pCurrentPlayer = PickRandomPlayer();
}

std::shared_ptr<TPlayer> TSession::PickRandomPlayer() {
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1,PlayersNumber());
	return Players()[dist(rng)];
}

std::shared_ptr<TPlayer> TSession::PickNextPlayer() {
	auto curPlIt = std::find(m_vPlayers.begin(), m_vPlayers.end(), CurrentPlayer());
	return  *((curPlIt + 1 == m_vPlayers.end()) ? (m_vPlayers.begin()) : (curPlIt + 1));
};
#undef DECL