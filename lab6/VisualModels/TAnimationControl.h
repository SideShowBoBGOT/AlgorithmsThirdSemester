//
// Created by choleraplague on 04.12.22.
//

#ifndef UNTITLED1_TANIMATIONCONTROL_H
#define UNTITLED1_TANIMATIONCONTROL_H

#include "TControl.h"
#include "../Other/TTimer.h"

class TAnimationControl : public TControl {
	public:
	TAnimationControl()=default;
	virtual void Init(unsigned millisec, unsigned framesNum);
	virtual ~TAnimationControl() override=default;
	
	public:
	virtual void Animate(bool isStart);
	virtual bool Render() override;
	
	protected:
	virtual void Start();
	virtual void Stop();
	
	protected:
	unsigned m_uInterval = 0;
	unsigned m_uFramesNum = 0;
	TTimer m_xTimer;
};


#endif //UNTITLED1_TANIMATIONCONTROL_H
