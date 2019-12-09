#ifndef _Drv_Uhard_H
#define _Drv_Uhard_H

/// <summary>
/// Encapsulate the uhard driver functionality 
/// </summary>
class DrvUard
{
public:

	/// <summary>
	/// Default Constructor
	/// </summary>
	DrvUard();

	/// <summary>
	/// DrvUard Destructor.
	/// </summary>
	~DrvUard();


	/// <summary>
	/// Initialize the uhard driver information.
	/// </summary>
	void Init();

	/// <summary>
	/// Writes data to the uhard.
	/// </summary>
	void Write();

	/// <summary>
	/// Read data from the uhard.
	/// </summary>
	void Read();
};

#endif // _Drv_Uhard_H