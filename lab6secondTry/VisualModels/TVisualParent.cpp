//
// Created by choleraplague on 26.11.22.
//

#include "TVisualParent.h"

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

std::shared_ptr<TVisualObject> TVisualParent::CreateObject() {
	auto object = std::make_shared<TVisualObject>();
	m_vObjectsPool.emplace_back(object);
	return object;
}

std::vector<std::shared_ptr<TVisualObject>>& TVisualParent::ObjectsPool() {
	return m_vObjectsPool;
}