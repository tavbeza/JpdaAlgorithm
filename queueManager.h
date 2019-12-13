#ifndef _Queue_Manager
#define _Queue_Manager
#include "StaticQueue.h"

/// <summary>
/// Encapsulates the logic of managing all the queues of the system.
/// </summary>
class QueueManager
{
public:

	/// <summary>
	/// Empty constructor
	/// </summary>
	QueueManager();

	/// <summary>
	/// Destructor
	/// </summary>
	~QueueManager();

	/// <summary>
	/// Allocates a queue
	/// </summary>
	StaticQueue* AllocateQueue();

	/// <summary>
	/// Returns the top queue from the list.
	/// </summary>
	StaticQueue* GetQueue();


};

#endif //_Queue_Manager