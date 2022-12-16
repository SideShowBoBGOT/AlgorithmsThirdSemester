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
		
		DECL(Align, NAlign, x, NAlign::Horizontal);
	#undef DECL
	
	public:
	template<typename T>
	T* CreateObject() {
		auto object = new T();
		auto ctrl = dynamic_cast<TControl*>(object);
		AddChild(ctrl);
		return object;
	}
	
	public:
	virtual void AddChild(TControl* child) override;
	
	public:
	virtual void ALignObjects();
	
	protected:
	virtual void AlignHorizontal();
	virtual void AlignCentral();
	virtual int CountVisibleChildren();
	virtual void DetermineZIndexes();
	
	protected:
	
};


#endif //UNTITLED1_TAUTOALIGNAREA_H
