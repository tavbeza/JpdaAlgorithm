#include "TestsManager.h"


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
