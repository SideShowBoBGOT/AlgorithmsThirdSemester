//
// Created by choleraplague on 26.11.22.
//

#ifndef UNTITLED1_TSESSION_H
#define UNTITLED1_TSESSION_H

#include "TPlayer.h"
#include "NNDifficulty.h"

class TSession {
	public:
	TSession()=delete;
	TSession(NDifficulty diff, int playersNumber);
	virtual ~TSession()=default;
	
	#define DECL_VECTOR(xx, type, prefix) \
		protected:            \
		type m_##prefix##xx;      \
        public:                        \
		virtual type& xx();
	
		DECL_VECTOR(Players, std::vector<std::shared_ptr<TPlayer>>, v);
		DECL_VECTOR(Cards, std::vector<std::shared_ptr<TCard>>, v);
	#undef DECL_VECTOR
	
	#define DECL(xx, type, prefix, value) \
		protected:            \
		type m_##prefix##xx = value;      \
        public:                        \
		virtual type xx();
	
		DECL(Difficulty, NDifficulty, x, NDifficulty::Medium);
		DECL(PlayersNumber, int, i, 2);
		DECL(LocalPlayer, std::shared_ptr<TPlayer>, p, nullptr);
		DECL(CurrentPlayer, std::shared_ptr<TPlayer>, p, nullptr);
	#undef DECL
	
	protected:
	std::shared_ptr<TPlayer> PickRandomPlayer();
	std::shared_ptr<TPlayer> PickNextPlayer();
};


#endif //UNTITLED1_TSESSION_H
