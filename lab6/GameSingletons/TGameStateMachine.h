//
// Created by choleraplague on 18.11.22.
//

#ifndef UNTITLED1_TGAMESTATEMACHINE_H
#define UNTITLED1_TGAMESTATEMACHINE_H

#include <stack>
#include "../Patterns/TSingleton.h"
#include "../VisualModels/IControl.h"

class TGameStateMachine : public IControl, public TSingleton<TGameStateMachine> {
	public:
	TGameStateMachine()=default;
	virtual ~TGameStateMachine() override=default;
	
	public:
	virtual bool HandleEvents() override;
	virtual bool Clean() override;
	virtual bool Render() override;
	
	public:
	virtual void PushState(IControl* state);
	virtual void PopState();
	
	protected:
	std::stack<IControl*> m_vStates;
};


#endif //UNTITLED1_TGAMESTATEMACHINE_H
