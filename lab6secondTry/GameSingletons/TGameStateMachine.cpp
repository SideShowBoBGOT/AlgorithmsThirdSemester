//
// Created by choleraplague on 18.11.22.
//

#include "TGameStateMachine.h"

void TGameStateMachine::HandleEvents() {
	m_vStates.top()->HandleEvents();
}
void TGameStateMachine::Clean() {
	m_vStates.top()->Clean();
}
void TGameStateMachine::Render() {
	m_vStates.top()->Render();
}
void TGameStateMachine::PushState(IControl* state) {
	m_vStates.push(state);
}
void TGameStateMachine::PopState() {
	m_vStates.pop();
}