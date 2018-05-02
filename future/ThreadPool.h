#pragma once

#include "BlockingQueue.h"
#include "Future.h"

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
	explicit CThreadPool( const int _threadCount );
	~CThreadPool();

	CFuture<T> Submit( std::function<T()> function );
	void DoTask();
	bool HasFreeWorkers() const;

private:
	int threadCount{ 0 };
	int workingThreads{ 0 };
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
CThreadPool<T>::CThreadPool( const int _threadCount ) :
	threadCount{ _threadCount }
{
	for( int i = 0; i < threadCount; ++i ) {
		workers.emplace_back( &CThreadPool<T>::DoTask, this );
	}
}

template<class T>
CThreadPool<T>::~CThreadPool()
{
	for( int i = 0; i < threadCount; ++i ) {
		queue.Produce( [] { throw CThreadPoolClosed{}; } );
	}
	for( auto& worker : workers ) {
		worker.join();
	}
}

template<class T>
CFuture<T> CThreadPool<T>::Submit( std::function<T()> function )
{
	std::unique_lock<std::mutex> lock( mutex );
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
		++workingThreads;
		try {
			task();
		} catch( const CThreadPoolClosed& exception ) {
			break;
		}
		--workingThreads;
	}
}

template<class T>
bool CThreadPool<T>::HasFreeWorkers() const
{
	std::unique_lock<std::mutex> lock( mutex );
	return workingThreads < threadCount;
}
