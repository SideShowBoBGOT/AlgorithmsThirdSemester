//
// Created by choleraplague on 26.11.22.
//

#ifndef UNTITLED1_TVISUALPARENT_H
#define UNTITLED1_TVISUALPARENT_H

#include "TVisualObject.h"

class TVisualParent : public TVisualObject {
	public:
	TVisualParent()=default;
	virtual ~TVisualParent() override=default;
	
	public:
	virtual void HandleEvents() override;
	virtual void Render() override;
	virtual void Clean() override;
	
	public:
	std::shared_ptr<TVisualObject> CreateObject();
	
	public:
	std::vector<std::shared_ptr<TVisualObject>>& ObjectsPool();
	
	protected:
	std::vector<std::shared_ptr<TVisualObject>> m_vObjectsPool;
};


#endif //UNTITLED1_TVISUALPARENT_H
