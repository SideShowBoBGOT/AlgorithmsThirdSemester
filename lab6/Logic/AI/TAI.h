//
// Created by choleraplague on 05.12.22.
//

#ifndef UNTITLED1_TAI_H
#define UNTITLED1_TAI_H


class TAI {
	public:
	TAI()=default;
	virtual ~TAI()=default;
	
	public:
	virtual void Move();
	
	protected:
	virtual void DoMove();
};


#endif //UNTITLED1_TAI_H
