#ifndef _Queue_Manager
#define _Queue_Manager
#include "StaticQueue.h"

/// <sumary>
/// Encapsulates the logic of managing all the queues of the system.
/// <\summary>
class QueueManager
{
public:

	/// <sumary>
	/// Empty constructor
	/// <\summary>
	QueueManager();

	/// <sumary>
	/// Destructor
	/// <\summary>
	~QueueManager();

	/// <sumary>
	/// Allocates a queue
	/// <\summary>
	StaticQueue* AllocateQueue();

	/// <sumary>
	/// Returns the top queue from the list.
	/// <\summary>
	StaticQueue* GetQueue();


};

#endif //_Queue_Manager