//
// Created by choleraplague on 19.11.22.
//

#ifndef UNTITLED1_TINPUTHANDLER_H
#define UNTITLED1_TINPUTHANDLER_H

#include <SDL2/SDL.h>
#include "../Patterns/TSingleton.h"

class TInputHandler : public TSingleton<TInputHandler> {
	public:
	TInputHandler()=default;
	virtual ~TInputHandler()=default;
	
	enum NMouseButton {
		Left=0,
		Right=1,
		Middle=2,
		Size=3
	};
	
	public:
	virtual void Update();
	virtual void Clean();
	
	public:
	virtual bool Downs(NMouseButton btn);
	virtual bool Ups(NMouseButton btn);
	virtual int X();
	virtual int Y();
	
	protected:
	bool m_vDowns[3] = { false, false, false };
	bool m_vUps[3] = { false, false, false };
	int m_iX = -1;
	int m_iY = -1;
};

using NMouseButton = TInputHandler::NMouseButton;

#endif //UNTITLED1_TINPUTHANDLER_H
