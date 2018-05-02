#pragma once

#include "Future.h"
#include "Promise.h"
#include "ThreadPool.h"

enum TAsyncType {
	AT_Async,
	AT_Sync
};

//----------------------------------------------------------------------------------------------------------------------

template<class T>
class CAsync {
public:
	static CFuture<T> Async( const TAsyncType type, std::function<T()> function );

private:
	static CThreadPool<T> pool;
};

//----------------------------------------------------------------------------------------------------------------------

template<class T>
CFuture<T> CAsync<T>::Async( const TAsyncType type, std::function<T()> function )
{
	if( type == AT_Async && pool.HasFreeWorkers() ) {
		return pool.Submit( function );
	}

	CPromise<T> promise{};
	auto future = promise.GetFuture();
	try {
		promise.SetValue( function() );
	} catch( std::exception& exception ) {
		promise.SetException( exception );
	}
	return future;
}
