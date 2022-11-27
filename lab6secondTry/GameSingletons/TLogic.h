//
// Created by choleraplague on 26.11.22.
//

#ifndef UNTITLED1_TLOGIC_H
#define UNTITLED1_TLOGIC_H

#include "../Patterns/TSingleton.h"
#include "../Logic/TSession.h"

class TLogic : public TSingleton<TLogic> {
	public:
	TLogic();
	virtual ~TLogic() override=default;
	
	public:
	std::vector<std::shared_ptr<TCard>> Cards;
	std::shared_ptr<TSession> Session = nullptr;
};


#endif //UNTITLED1_TLOGIC_H
