#include "pch.h"
#include "TestMethod.h"


TestMethod::TestMethod()
{
}


TestMethod::~TestMethod()
{
}

void TestMethod::resetTestID()
{
	global_test_id = 0;
}

int TestMethod::getTestID()
{
	return ++global_test_id;
}
