#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

/// <sumary>
/// In this class we alloction memory to the program, and if some process or variable need some memory
/// this class allocate memory to him
/// <\summary>
class MemoryPool
{
public:
	/// <sumary>
	/// Empty constructor
	/// <\summary>
	MemoryPool();

	/// <sumary>
	/// Destructor
	/// <\summary>
	~MemoryPool();

	/// <sumary>
	/// This function allocate memory to the program
	/// <\summary>
	void allocate();

protected:
	double m_startAddress;
	double m_finalAddress;
};

#endif _MEMORY_POOL_H_