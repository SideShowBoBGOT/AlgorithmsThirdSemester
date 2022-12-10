//
// Created by choleraplague on 26.11.22.
//

#ifndef UNTITLED1_TPLAYER_H
#define UNTITLED1_TPLAYER_H

#include <vector>
#include <memory>
#include "TCard.h"

class TPlayer {
	public:
	TPlayer()=default;
	virtual ~TPlayer()=default;
	
	virtual std::vector<std::shared_ptr<TCard>>& Cards();
	#define DECL(xx, type, prefix, value) \
		protected:            \
		type m_##prefix##xx = value;      \
        public:                        \
		virtual void xx(type vv); \
		virtual type xx();
	
	DECL(Active, bool, b, false);
	DECL(FirstMove, bool, b, true);
	DECL(IsAI, bool, b, false);
	DECL(Score, int, i, 0);
	
	protected:
	std::vector<std::shared_ptr<TCard>> m_vCards;
};


#endif //UNTITLED1_TPLAYER_H
