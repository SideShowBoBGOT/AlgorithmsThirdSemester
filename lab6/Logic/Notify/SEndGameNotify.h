//
// Created by choleraplague on 17.12.22.
//

#ifndef UNTITLED1_SENDGAMENOTIFY_H
#define UNTITLED1_SENDGAMENOTIFY_H

#include "INotify.h"

class SEndGameNotify : public INotify {
	public:
	SEndGameNotify()=default;
	explicit SEndGameNotify(bool isLocalPlayerWon) : IsLocalPlayerWon(isLocalPlayerWon) {};
	virtual ~SEndGameNotify() override=default;
	
	bool IsLocalPlayerWon = false;
};

#endif //UNTITLED1_SENDGAMENOTIFY_H
