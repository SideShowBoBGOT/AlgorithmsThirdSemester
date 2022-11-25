//
// Created by choleraplague on 18.11.22.
//

#ifndef UNTITLED1_TGAMESTATE_H
#define UNTITLED1_TGAMESTATE_H


class IGameState {
	public:
	IGameState()=default;
	virtual ~IGameState()=default;
	
	public:
	virtual void HandleEvents()=0;
	virtual void Clean()=0;
	virtual void Render()=0;
};


#endif //UNTITLED1_TGAMESTATE_H
