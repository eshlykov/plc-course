#pragma once

#include "Future.h"
#include "Promise.h"
#include "ThreadPool.h"

enum TAsyncType {
	AT_Async,
	AT_Sync
};

//----------------------------------------------------------------------------------------------------------------------

template<class T, class ...A>
class CAsync {
public:
	static CFuture<T> Async( const TAsyncType type, std::function<T(A...)> function, A... agruments );

private:
	static CThreadPool<T> pool;
};

//----------------------------------------------------------------------------------------------------------------------

template<class T, class ...A>
CFuture<T> CAsync<T, A...>::Async( const TAsyncType type, std::function<T(A...)> function, A... agruments )
{
	auto executable = std::bind( function, agruments... );

	if( type == AT_Async && pool.HasFreeWorkers() ) {
		return pool.Submit( executable );
	}

	CPromise<T> promise{};
	auto future = promise.GetFuture();
	try {
		promise.SetValue( executable() );
	} catch( std::exception& exception ) {
		promise.SetException( exception );
	}
	return future;
}
