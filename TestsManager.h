#ifndef _TEST_MANAGER_H_
#define _TEST_MANAGER_H_

#include <iostream>

#include "TestMethod.h"
#include "KalmanTests.h"

class TestsManager : public TestMethod
{
public:
	virtual void Execute();
};


#endif //_TEST_MANAGER_H_
