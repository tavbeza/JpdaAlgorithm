#include <iostream>

#include "TestsManager.h"

using namespace std;

void main()
{
	// Already created 
	//TestMethod::CreateCSVfileOfPlotsAndDwells(); 

	// Test Extended Kalman Filter 2D.
	TestsManager testsManager;
	testsManager.Execute();

}
