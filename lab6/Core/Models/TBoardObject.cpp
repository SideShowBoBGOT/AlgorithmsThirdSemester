//
// Created by choleraplague on 14.11.22.
//

#include <random>

#include "TBoardObject.h"
#include "../Config/TConfig.h"

namespace Logic {
	void TBoardObject::OnGameBegin() {
	
	}
	void TBoardObject::NextTurn() {
		auto player = PickNextPlayerToPlay();
		EndTurnFor(player);
		BeginTurnFor(player);
	}
	void TBoardObject::BeginTurnFor(const std::shared_ptr<TPlayerObject>& player) {
		m_pCurrentPlayer = player;
	}
	void TBoardObject::EndTurnFor(const std::shared_ptr<TPlayerObject>& player) {
		GivePlayerSparseCards(player);
	}
	std::shared_ptr<TPlayerObject> TBoardObject::PickPlayerToGiveCards() {
		auto index = rand() % m_vPlayCards.size();
		return m_vPlayers[index];
	}
	std::shared_ptr<TPlayerObject> TBoardObject::PickNextPlayerToPlay() {
 		auto it = std::find(m_vPlayers.begin(), m_vPlayers.end(), m_pCurrentPlayer);
		++it;
		if(it==m_vPlayers.end()) {
			it = m_vPlayers.begin();
		}
		return *it;
	}
	std::shared_ptr<TPlayerObject> TBoardObject::PickPlayerToPlayFirst() {
		srand(time(NULL));
		return m_vPlayers[rand() % m_vPlayers.size()];
	}
	std::shared_ptr<TPlayerObject>& TBoardObject::CurrentPlayer() {
		return m_pCurrentPlayer;
	}
	std::shared_ptr<TPlayerObject>& TBoardObject::LocalPlayer() {
		return m_pLocalPlayer;
	}
	std::vector<std::shared_ptr<TCardObject>>& TBoardObject::PlayCards() {
		return m_vPlayCards;
	}
	std::vector<std::shared_ptr<TCardObject>>& TBoardObject::SparseCards() {
		return m_vSparseCards;
	}

} // Logic