//
// Created by choleraplague on 26.11.22.
//

#ifndef UNTITLED1_TLOGIC_H
#define UNTITLED1_TLOGIC_H

#include <thread>
#include "../Patterns/TSingleton.h"
#include "../Logic/TSession.h"

class TLogic : public TSingleton<TLogic> {
	public:
	TLogic()=default;
	virtual ~TLogic() override=default;
	
	public:
	std::shared_ptr<TSession> Session = nullptr;
	std::thread AIThread;
	
};


#endif //UNTITLED1_TLOGIC_H
