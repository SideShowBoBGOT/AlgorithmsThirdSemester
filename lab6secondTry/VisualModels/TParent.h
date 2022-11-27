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
	virtual void HandleEvents() override;
	virtual void Render() override;
	virtual void Clean() override;
	
	public:
	template<typename T>
	T* CreateObject() {
		auto object = new T();
		auto ctrl = dynamic_cast<TControl*>(object);
		ctrl->Parent(this);
		m_vObjectsPool.emplace_back(ctrl);
		return object;
	}
	
	public:
	std::vector<TControl*>& ObjectsPool();
	
	protected:
	std::vector<TControl*> m_vObjectsPool;
};


#endif //UNTITLED1_TPARENT_H
