//
// Created by choleraplague on 26.11.22.
//

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
};
#undef DECL