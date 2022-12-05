//
// Created by choleraplague on 04.12.22.
//

#include "TAnimationControl.h"

void TAnimationControl::Init(unsigned millisec, unsigned framesNum) {
	m_uInterval = millisec;
	m_uFramesNum = framesNum;
}

void TAnimationControl::Start() {
	Sx(0);
	m_xTimer.Start();
}

void TAnimationControl::Stop() { m_xTimer.Stop(); }

bool TAnimationControl::Render() {
	if(m_xTimer.IsRunning() and m_xTimer.ElapsedMilliseconds() > m_uInterval) {
		m_xTimer.Start();
		auto newSx = Sx() + Width();
		Sx((newSx < m_uFramesNum*Width()) ? newSx : 0);
	}
	return TControl::Render();
}