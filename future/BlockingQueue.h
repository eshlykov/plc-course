#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

template<class T>
class CBlockingQueue {
public:
	explicit CBlockingQueue( const int capacity = 0 );

	void Produce( T&& element );
	void Consume( T& result );

private:
	int capacity;
	std::queue<T> queue;
	std::condition_variable notFull;
	std::condition_variable notEmpty;
	std::mutex mutex;
};

//----------------------------------------------------------------------------------------------------------------------

template<class T>
CBlockingQueue<T>::CBlockingQueue( const int _capacity ) :
	// Zero means that queue is unbounded.
	capacity{ _capacity }
{
}

template<class T>
void CBlockingQueue<T>::Produce( T && element )
{
	std::unique_lock<std::mutex> lock( mutex );
	notFull.wait( lock, [&] { return capacity == 0 || queue.size() < capacity; } );
	queue.push( std::move( element ) );
	notEmpty.notify_one();
}

template<class T>
void CBlockingQueue<T>::Consume( T & result )
{
	std::unique_lock<std::mutex> lock( mutex );
	notEmpty.wait( lock, [&] { return !queue.empty(); } );
	result = std::move( queue.front() );
	queue.pop();
	notFull.notify_one();
}
