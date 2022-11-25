//
// Created by choleraplague on 18.11.22.
//

#ifndef UNTITLED1_TGAMESTATE_H
#define UNTITLED1_TGAMESTATE_H


class TGameState {
	public:
	TGameState()=default;
	virtual ~TGameState()=default;
	
	public:
	virtual void HandleEvents()=0;
	virtual void Update()=0;
	virtual void Render()=0;
};


#endif //UNTITLED1_TGAMESTATE_H
