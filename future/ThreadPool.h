#pragma once

#include "BlockingQueue.h"
#include "Future.h"

#include <atomic>
#include <functional>
#include <mutex>
#include <vector>

template<class T>
class CThreadPool {
	class CThreadPoolClosed : public std::runtime_error {
	public:
		CThreadPoolClosed();
	};

public:
	explicit CThreadPool( const int _workersCount );
	~CThreadPool();

	CFuture<T> Submit( std::function<T()> function );
	void DoTask();
	bool HasFreeWorkers();

private:
	int workersCount{ 0 };
	std::atomic<int> busyWorkersCount{ 0 };
	CBlockingQueue<std::function<void()>> queue{};
	std::vector<std::thread> workers{};
	std::mutex mutex{};
};

//----------------------------------------------------------------------------------------------------------------------

template<class T>
CThreadPool<T>::CThreadPoolClosed::CThreadPoolClosed() :
	std::runtime_error( "Thread Pool is closed for submits." )
{
}

//----------------------------------------------------------------------------------------------------------------------

template<class T>
CThreadPool<T>::CThreadPool( const int _workersCount ) :
	workersCount{ _workersCount }
{
	for( int i = 0; i < workersCount; ++i ) {
		workers.emplace_back( &CThreadPool<T>::DoTask, this );
	}
}

template<class T>
CThreadPool<T>::~CThreadPool()
{
	for( int i = 0; i < workersCount; ++i ) {
		queue.Produce( [] { throw CThreadPoolClosed{}; } );
	}
	std::for_each( workers.begin(), workers.end(), [] ( auto& worker ) { worker.join(); } );
}

template<class T>
CFuture<T> CThreadPool<T>::Submit( std::function<T()> function )
{
	std::lock_guard<std::mutex> lock( mutex );
	CPromise<T> promise{};
	auto future = promise.GetFuture();
	queue.Produce( [promise, function] () mutable {
		try {
			promise.SetValue( std::move( function() ) );
		} catch( const std::exception& exception ) {
			promise.SetException( exception );
		}
	});
	return future;
}

template<class T>
void CThreadPool<T>::DoTask()
{
	while( true ) {
		std::function<void()> task{};
		queue.Consume( task );
		++busyWorkersCount;
		try {
			task();
		} catch( const CThreadPoolClosed& exception ) {
			// Don't decrement busyWorkersCount because removed workers is not free actually.
			break;
		}
		--busyWorkersCount;
	}
}

template<class T>
bool CThreadPool<T>::HasFreeWorkers()
{
	return busyWorkersCount < workersCount;
}
