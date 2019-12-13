#ifndef _STATIC_QUEUE_H_
#define _STATIC_QUEUE_H_
/// <summary>
/// This class capsulate the queue to all the queue in this program
/// </summary>
class StaticQueue
{
public:

	/// <summary>
	/// Empty constructor
	/// </summary>
	StaticQueue();

	/// <summary>
	/// Destructor
	/// </summary>
	~StaticQueue();

	/// <summary>
	/// This function allocate memory to the queue
	/// </summary>
	void allocate();

	/// <summary>
	/// This function set queue with pointer to start and queue size
	/// </summary>
	void setQueue(double startAddress, int size);

	/// <summary>
	/// pop element from the queue
	/// </summary>
	void pop();

	/// <summary>
	/// push element to the queue
	/// </summary>
	void push();

	/// <summary>
	/// cheak if the queue is empty
	/// </summary>
	bool isEmpty();

protected:

};

#endif _STATIC_QUEUE_H_