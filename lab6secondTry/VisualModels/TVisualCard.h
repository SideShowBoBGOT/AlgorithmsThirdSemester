//
// Created by choleraplague on 26.11.22.
//

#ifndef UNTITLED1_TVISUALCARD_H
#define UNTITLED1_TVISUALCARD_H

#include "TParent.h"

class TVisualCard : public TParent {
	public:
	TVisualCard();
	virtual ~TVisualCard() override;
	
	virtual bool Render() override;
	
	public:
	TControl* Content = nullptr;
	
	#define DECL_OVERRIDE(xx, type) \
        public:                        \
		virtual void xx(type vv) override; \
	
		DECL_OVERRIDE(Dx, int);
	#undef DECL_OVERRIDE
	
	#define DECL(xx, type, prefix, val) \
		protected:            \
		type m_##prefix##xx = val;      \
                                \
        public:                        \
		virtual void xx(type vv); \
		virtual type xx();
	
		DECL(ShiftY, int, i, 0);
	#undef DECL
};


#endif //UNTITLED1_TVISUALCARD_H
