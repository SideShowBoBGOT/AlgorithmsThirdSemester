//
// Created by choleraplague on 27.11.22.
//

#ifndef UNTITLED1_TAUTOALIGNAREA_H
#define UNTITLED1_TAUTOALIGNAREA_H

#include "TParent.h"
#include "Enums/NNAlign.h"

class TAutoAlignArea : public TParent {
	public:
	TAutoAlignArea();
	virtual ~TAutoAlignArea() override=default;
	
	#define DECL(xx, type, prefix, val) \
		protected:            \
		type m_##prefix##xx = val;      \
                                \
        public:                        \
		virtual void xx(type vv); \
		virtual type xx();
		
		DECL(Padding, int, i, 0);
		DECL(Align, NAlign, x, NAlign::Horizontal);
	#undef DECL
	
	public:
	virtual void ALignObjects();
	
	public:
	virtual void OnChange(std::function<void(TControl* obj)>&& func) override;
};


#endif //UNTITLED1_TAUTOALIGNAREA_H
