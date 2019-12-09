#ifndef _STATIC_QUEUE_H_
#define _STATIC_QUEUE_H_
/// <sumary>
/// This class capsulate the queue to all the queue in this program
/// <\summary>
class StaticQueue
{
public:

	/// <sumary>
	/// Empty constructor
	/// <\summary>
	StaticQueue();

	/// <sumary>
	/// Destructor
	/// <\summary>
	~StaticQueue();

	/// <sumary>
	/// This function allocate memory to the queue
	/// <\summary>
	void allocate();

	/// <sumary>
	/// This function set queue with pointer to start and queue size
	/// <\summary>
	void setQueue(double startAddress, int size);

	/// <sumary>
	/// pop element from the queue
	/// <\summary>
	void pop();

	/// <sumary>
	/// push element to the queue
	/// <\summary>
	void push();

	/// <sumary>
	/// cheak if the queue is empty
	/// <\summary>
	bool isEmpty();

protected:

};

#endif _STATIC_QUEUE_H_