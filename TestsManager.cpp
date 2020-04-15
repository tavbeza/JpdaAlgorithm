#include "TestsManager.h"
int global_test_id;


void TestsManager::Execute()
{
	try
	{
		KalmanTests kalmanTests;
		kalmanTests.Execute();
	}
	catch (std::exception & e)
	{
		std::cout << e.what();
	}

}
