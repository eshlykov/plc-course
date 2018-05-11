#pragma once

#include "Future.h"
#include "Promise.h"
#include "ThreadPool.h"

#include <any>

enum TAsyncType {
	AT_Async,
	AT_Sync
};

//----------------------------------------------------------------------------------------------------------------------

class CAsync {
public:
	template<class T, class ...A>
	static CFuture<T> Async( const TAsyncType type, std::function<T(A...)> function, A... agruments );

	template<class T>
	static CFuture<T> Async( const TAsyncType type, std::function<T()> function );

private:
	static CThreadPool<std::any> pool;
};

//----------------------------------------------------------------------------------------------------------------------

template<class T, class ...A >
CFuture<T> CAsync::Async( const TAsyncType type, std::function<T(A...)> function, A... agruments )
{
	auto executable = std::bind( function, agruments... );

	if( type == AT_Async && pool.HasFreeWorkers() ) {
		return  CFuture<T>( pool.Submit( executable ).data );
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

//----------------------------------------------------------------------------------------------------------------------

template<class T>
CFuture<T> CAsync::Async( const TAsyncType type, std::function<T()> function )
{
	if( type == AT_Async && pool.HasFreeWorkers() ) {
		return CFuture<T>( pool.Submit( function ).data );
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
