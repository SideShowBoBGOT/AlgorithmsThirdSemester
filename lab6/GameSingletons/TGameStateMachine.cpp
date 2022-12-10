//
// Created by choleraplague on 18.11.22.
//

#include "TGameStateMachine.h"

bool TGameStateMachine::HandleEvents() {
	if(m_vStates.empty()) return false;
	return m_vStates.top()->HandleEvents();
}
bool TGameStateMachine::Clean() {
	if(m_vStates.empty()) return false;
	return m_vStates.top()->Clean();
}
bool TGameStateMachine::Render() {
	if(m_vStates.empty()) return false;
	return m_vStates.top()->Render();
}
void TGameStateMachine::PushState(IControl* state) {
	m_vStates.push(state);
}
void TGameStateMachine::PopState() {
	m_vStates.pop();
}