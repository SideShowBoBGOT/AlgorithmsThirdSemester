//
// Created by choleraplague on 16.12.22.
//

#ifndef UNTITLED1_TTAKETEST_H
#define UNTITLED1_TTAKETEST_H

#include "ITest.h"
#include "../Enums/NNCardValue.h"
#include <vector>
#include <memory>

class TCard;

class TTakeTest : public ITest {
	public:
	TTakeTest()=default;
	virtual ~TTakeTest()=default;
	
	public:
	virtual bool Test() override;
	std::vector<std::shared_ptr<TCard>> GetSetOfCards(NCardValue value);
};


#endif //UNTITLED1_TTAKETEST_H
