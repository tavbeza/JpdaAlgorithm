#ifndef _Drv_Uhart_H
#define _Drv_Uhart_H

/// <summary>
/// Encapsulate the uhart driver functionality 
/// </summary>
class DrvUart
{
public:

	/// <summary>
	/// Default Constructor
	/// </summary>
	DrvUart();

	/// <summary>
	/// DrvUart Destructor.
	/// </summary>
	~DrvUart();


	/// <summary>
	/// Initialize the uhart driver information.
	/// </summary>
	void Init();

	/// <summary>
	/// Writes data to the uhart.
	/// </summary>
	void Write();

	/// <summary>
	/// Read data from the uhart.
	/// </summary>
	void Read();
};

#endif // _Drv_Uhart_H