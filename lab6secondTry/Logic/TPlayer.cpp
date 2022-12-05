//
// Created by choleraplague on 26.11.22.
//

#include "TPlayer.h"

#define DECL(xx, type, prefix) \
    type TPlayer::xx() { return m_##prefix##xx; }\
    void TPlayer::xx(type vv) { m_##prefix##xx = vv; }
	
	DECL(Active, bool, b);
	DECL(FirstMove, bool, b);
	DECL(Score, int, i);
#undef DECL

std::vector<std::shared_ptr<TCard>>& TPlayer::Cards() {
	return m_vCards;
}
