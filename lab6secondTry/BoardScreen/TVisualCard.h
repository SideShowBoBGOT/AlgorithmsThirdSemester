//
// Created by choleraplague on 26.11.22.
//

#ifndef UNTITLED1_TVISUALCARD_H
#define UNTITLED1_TVISUALCARD_H

#include "../VisualModels/TVisualObject.h"

class TVisualCard : public TVisualObject {
	public:
	TVisualCard()=default;
	virtual ~TVisualCard()=default;
	
	virtual void Render() override;
	
	#define DECL(xx, type, prefix, val) \
		protected:            \
		type m_##prefix##xx = val;      \
                                \
        public:                        \
		virtual void xx(type vv); \
		virtual type xx();
	
		DECL(DefaultY, int, i, 0);
		DECL(DefaultX, int, i, 0);
		DECL(ShiftX, int, i, 0);
		DECL(ShiftY, int, i, 0);
	#undef DECL
};


#endif //UNTITLED1_TVISUALCARD_H
