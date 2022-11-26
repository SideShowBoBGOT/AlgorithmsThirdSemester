//
// Created by choleraplague on 26.11.22.
//

#include "TVisualParent.h"

TVisualParent::~TVisualParent() {
	for(auto& child : m_vObjectsPool) {
		delete child;
	}
}

void TVisualParent::Render() {
	TVisualObject::Render();
	for(auto& child : m_vObjectsPool) {
		child->Render();
	}
}

void TVisualParent::HandleEvents() {
	TVisualObject::HandleEvents();
	for(auto& child : m_vObjectsPool) {
		child->HandleEvents();
	}
}

void TVisualParent::Clean() {
	TVisualObject::Clean();
	for(auto& child : m_vObjectsPool) {
		child->Clean();
	}
}

TVisualObject* TVisualParent::CreateObject() {
	auto object = new TVisualObject();
	m_vObjectsPool.emplace_back(object);
	return object;
}

std::vector<TVisualObject*>& TVisualParent::ObjectsPool() {
	return m_vObjectsPool;
}