//
// Created by choleraplague on 26.11.22.
//

#include "TCard.h"

#define DECL(xx, type, prefix) \
    type TCard::xx() { return m_##prefix##xx; }\
    void TCard::xx(type vv) { m_##prefix##xx = vv; }
	
	DECL(Type, NCardType, x);
	DECL(Value, NCardValue, x);
#undef DECL