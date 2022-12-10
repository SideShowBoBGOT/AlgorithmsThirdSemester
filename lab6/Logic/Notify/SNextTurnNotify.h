//
// Created by choleraplague on 06.12.22.
//

#ifndef UNTITLED1_SNEXTTURNNOTIFY_H
#define UNTITLED1_SNEXTTURNNOTIFY_H

#include "INotify.h"

class SNextTurnNotify : public INotify {
	public:
	SNextTurnNotify()=default;
	explicit SNextTurnNotify(bool isAI);
	virtual ~SNextTurnNotify() override=default;
	
	bool IsAI = false;
};


#endif //UNTITLED1_SNEXTTURNNOTIFY_H
