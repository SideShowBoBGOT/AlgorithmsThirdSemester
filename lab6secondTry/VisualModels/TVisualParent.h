//
// Created by choleraplague on 26.11.22.
//

#ifndef UNTITLED1_TVISUALPARENT_H
#define UNTITLED1_TVISUALPARENT_H

#include "TVisualObject.h"

class TVisualParent : public TVisualObject {
	public:
	TVisualParent()=default;
	virtual ~TVisualParent() override;
	
	public:
	virtual void HandleEvents() override;
	virtual void Render() override;
	virtual void Clean() override;
	
	public:
	TVisualObject* CreateObject();
	
	public:
	std::vector<TVisualObject*>& ObjectsPool();
	
	protected:
	std::vector<TVisualObject*> m_vObjectsPool;
};


#endif //UNTITLED1_TVISUALPARENT_H
