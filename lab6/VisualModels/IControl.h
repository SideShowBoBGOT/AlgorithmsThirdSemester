//
// Created by choleraplague on 18.11.22.
//

#ifndef UNTITLED1_IVisual_H
#define UNTITLED1_IVisual_H


class IControl {
	public:
	IControl()=default;
	virtual ~IControl()=default;
	
	public:
	virtual bool HandleEvents()=0;
	virtual bool Clean()=0;
	virtual bool Render()=0;
};


#endif //UNTITLED1_IVisual_H
