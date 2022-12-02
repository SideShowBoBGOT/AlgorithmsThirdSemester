//
// Created by choleraplague on 26.11.22.
//

#ifndef UNTITLED1_TPARENT_H
#define UNTITLED1_TPARENT_H

#include "TControl.h"

class TParent : public TControl {
	public:
	TParent()=default;
	virtual ~TParent() override;
	
	public:
	virtual bool HandleEvents() override;
	virtual bool Render() override;
	virtual bool Clean() override;
	
	#define DECL_OVERRIDE(xx, type) \
        public:                        \
		virtual void xx(type vv) override; \
	
		DECL_OVERRIDE(Over, bool);
		DECL_OVERRIDE(Enabled, bool);
		DECL_OVERRIDE(Visible, bool);
	#undef DECL_OVERRIDE
	
	#define DECL(xx, type, prefix, val) \
		protected:            \
		type m_##prefix##xx = val;      \
								\
		public:                        \
		virtual void xx(type vv); \
		virtual type xx();
	
		DECL(ControlChildState, bool, b, false);
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
	virtual void AddChild(TControl* child);
	virtual void RemoveChild(TControl* child);
	
	std::vector<TControl*>& ObjectsPool();
	
	protected:
	std::vector<TControl*> m_vObjectsPool;
};


#endif //UNTITLED1_TPARENT_H
