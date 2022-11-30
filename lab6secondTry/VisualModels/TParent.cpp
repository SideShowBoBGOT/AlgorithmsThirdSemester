//
// Created by choleraplague on 26.11.22.
//

#include "TParent.h"
#include "../GameSingletons/TInputHandler.h"

#define DECL_WITH_CHILD_STATE(xx, type, prefix) \
	void TParent::xx(type vv) {  \
        m_##prefix##xx = vv;          \
        if(m_pParent) m_pParent->OnChange(); \
        OnChange();                  \
        if(ControlChildState()) {                  \
            for(auto& c : m_vObjectsPool) {       \
                c->xx(TControl::xx());                    \
			}\
		}                           \
	}

	DECL_WITH_CHILD_STATE(State, NState, x);
	DECL_WITH_CHILD_STATE(Visible, bool, b);
	DECL_WITH_CHILD_STATE(Enabled, bool, b);
#undef DECL_WITH_CHILD_STATE

#define DECL(xx, type, prefix) \
	void TParent::xx(type vv) {  \
		m_##prefix##xx = vv;          \
		if(m_pParent) m_pParent->OnChange(); \
		OnChange();                  \
	}                             \
	type TParent::xx() { return m_##prefix##xx; }
	
	DECL(ControlChildState, bool, b);
#undef DECL


TParent::~TParent() {
	for(auto& child : m_vObjectsPool) {
		delete child;
	}
}

void TParent::Render() {
	TControl::Render();
	for(auto& child : m_vObjectsPool) {
		child->Render();
	}
}

void TParent::HandleEvents() {
	TControl::HandleEvents();
	
	auto controls = std::vector<TControl*>();
	for(auto& child : m_vObjectsPool) {
		if(child->IsMouseOn()) {
			controls.emplace_back(child);
		}
	}
	TControl* onTop = nullptr;
	if(not controls.empty()) {
		onTop = controls[0];
		for(auto& c:controls) {
			if(c->ZIndex() > onTop->ZIndex()) {
				onTop = c;
			}
		}
		onTop->HandleEvents();
	}
	for(auto& c : m_vObjectsPool) {
		if(c!=onTop) {
			c->Over(false);
		}
	}
}

void TParent::Clean() {
	TControl::Clean();
	for(auto& child : m_vObjectsPool) {
		child->Clean();
	}
}

std::vector<TControl*>& TParent::ObjectsPool() {
	return m_vObjectsPool;
}

void TParent::AddObject(TControl*c) {
	m_vObjectsPool.push_back(c);
	c->Parent(this);
	OnChangeHandler(this);
}
