//
// Created by choleraplague on 26.11.22.
//

#ifndef UNTITLED1_TSESSION_H
#define UNTITLED1_TSESSION_H

#include "TPlayer.h"
#include "NNDifficulty.h"

class TSession {
	public:
	TSession()=default;
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
	#undef DECL
	
	protected:
	int m_iPlayersNumber = 2;
	
	
	
	
};


#endif //UNTITLED1_TSESSION_H
