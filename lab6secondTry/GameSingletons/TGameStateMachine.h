//
// Created by choleraplague on 18.11.22.
//

#ifndef UNTITLED1_TGAMESTATEMACHINE_H
#define UNTITLED1_TGAMESTATEMACHINE_H

#include <stack>
#include "../Patterns/TSingleton.h"
#include "../Models/TGameState.h"

class TGameStateMachine : public IGameState, public TSingleton<TGameStateMachine> {
	public:
	TGameStateMachine()=default;
	virtual ~TGameStateMachine() override=default;
	
	public:
	virtual void HandleEvents() override;
	virtual void Clean() override;
	virtual void Render() override;
	
	public:
	virtual void PushState(IGameState* state);
	virtual void PopState();
	
	protected:
	std::stack<IGameState*> m_vStates;
};


#endif //UNTITLED1_TGAMESTATEMACHINE_H
