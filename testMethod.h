#ifndef _TEST_METHOD_H_
#define _TEST_METHOD_H_

/// <summary>
/// This class encpsulate the TestMethod entity. 
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
};


#endif //_TEST_METHOD_H_