//
// Created by choleraplague on 26.11.22.
//

#include "TParent.h"
#include "../GameSingletons/TInputHandler.h"

#define DECL_WITH_CHILD_STATE(xx, type, prefix) \
	void TParent::xx(type vv) {  \
        m_##prefix##xx = vv;          \
        if(ControlChildState()) {                  \
            for(auto& c : m_vObjectsPool) {       \
                c->xx(TControl::xx());                    \
			}\
		}                           \
	}

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

void TParent::Over(bool vv) {
	TControl::Over(vv);
	if(not vv) {
		for(auto& child : m_vObjectsPool) {
			child->Over(false);
		}
	}
}

TParent::~TParent() {
	for(auto child : m_vObjectsPool) {
		delete child;
	}
	m_vObjectsPool.clear();
}

bool TParent::Render() {
	auto res = TControl::Render();
	
	if(res) {
		for(auto& child:m_vObjectsPool) {
			child->Render();
		}
	}
	return res;
}

bool TParent::HandleEvents() {
	auto res = TControl::HandleEvents();
	if(res) {
		auto controls = std::vector<TControl*>();
		for(auto& child:m_vObjectsPool) {
			if(child->IsMouseOn()) {
				controls.emplace_back(child);
			}
		}
		TControl*onTop = nullptr;
		if(not controls.empty()) {
			onTop = controls[0];
			for(auto& c:controls) {
				if(c->ZIndex() > onTop->ZIndex()) {
					onTop = c;
				}
			}
			onTop->HandleEvents();
		}
		for(auto& c:m_vObjectsPool) {
			if(c != onTop) {
				c->Over(false);
			}
		}
	}
	return res;
}

bool TParent::Clean() {
	auto res = TControl::Clean();
	if(res) {
		for(auto& child:m_vObjectsPool) {
			child->Clean();
		}
	}
	return res;
}

std::vector<TControl*>& TParent::ObjectsPool() {
	return m_vObjectsPool;
}

void TParent::AddChild(TControl* child) {
	child->Parent(this);
	m_vObjectsPool.push_back(child);
}

void TParent::RemoveChild(TControl* child) {
	child->Parent(nullptr);
	auto& p = m_vObjectsPool;
	p.erase(std::remove(p.begin(), p.end(), child), p.end());
}
