#ifndef _TEST_METHOD_H_
#define _TEST_METHOD_H_

//#include "CppUnitTest.h"
//using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/// <summary>
/// A running id for the tests.
/// Its a variable shared by all test methods, the TestsManager is responsible for defining it.
/// (the extern makes it so the variable's existance is declared so that each source file that
/// includes the header knows about it, but it still needs to be defined somewhere)
/// </summary>
extern int global_test_id;


/// <summary>
/// This class encapsulates the TestMethod entity. 
/// This is the base class for all tests.
/// </summary>
class TestMethod
{
public:

	/// <summary>
	/// Default Constructor
	/// </summary>
	TestMethod();

	/// <summary>
	/// TestMethod Destructor
	/// </summary>
	~TestMethod();

	/// <summary>
	/// Executes the test.
	/// </summary>
	virtual void Execute() = 0;

protected:

	/// <summary>
	/// Reset the test id
	/// </summary>
	void resetTestID();

	/// <summary>
	/// Get the test id
	/// </summary>
	int getTestID();

};


#endif //_TEST_METHOD_H_
