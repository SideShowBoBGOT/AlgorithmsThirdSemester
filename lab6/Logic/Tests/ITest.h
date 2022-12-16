//
// Created by choleraplague on 16.12.22.
//

#ifndef UNTITLED1_ITEST_H
#define UNTITLED1_ITEST_H

class ITest {
	public:
	virtual bool Test() = 0;
	protected:
	ITest()=default;
	virtual ~ITest()=default;
};

#endif //UNTITLED1_ITEST_H
