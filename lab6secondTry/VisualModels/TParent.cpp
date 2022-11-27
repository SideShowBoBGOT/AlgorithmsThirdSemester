//
// Created by choleraplague on 26.11.22.
//

#include "TParent.h"

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
	for(auto& child : m_vObjectsPool) {
		child->HandleEvents();
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