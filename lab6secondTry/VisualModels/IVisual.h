//
// Created by choleraplague on 18.11.22.
//

#ifndef UNTITLED1_IVisual_H
#define UNTITLED1_IVisual_H


class IVisual {
	public:
	IVisual()=default;
	virtual ~IVisual()=default;
	
	public:
	virtual void HandleEvents()=0;
	virtual void Clean()=0;
	virtual void Render()=0;
};


#endif //UNTITLED1_IVisual_H
