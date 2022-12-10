//
// Created by choleraplague on 26.11.22.
//

#ifndef UNTITLED1_TCARD_H
#define UNTITLED1_TCARD_H

#include "Enums/NNCardType.h"
#include "Enums/NNCardValue.h"

class TCard {
	public:
	TCard()=default;
	virtual ~TCard()=default;
	
	#define DECL(xx, type, prefix, value) \
		protected:            \
		type m_##prefix##xx = value;      \
        public:                        \
		virtual void xx(type vv); \
		virtual type xx();
	
	DECL(Type, NCardType, x, NCardType::Hearts);
	DECL(Value, NCardValue, x, NCardValue::Six);
	#undef DECL
};


#endif //UNTITLED1_TCARD_H
