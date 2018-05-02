#pragma once

#include "FutureState.h"
#include "Promise.h"
#include "ThreadPool.h"

#include <algorithm>
#include <functional>
#include <memory>

template<class T>
class CPromise;

template<class T>
class CFuture {
public:
	CFuture() = default;

	CFuture( const CFuture<T>& other );
	CFuture( CFuture<T>&& other );

	T& Get();
	bool TryGet( T& result );
	CFuture<T> Then( std::function<T( T )> function );

private:
	friend CPromise<T>;

	std::shared_ptr<CFutureState<T>> data{ new CFutureState<T>{} };

	void setValue( T&& value );
	void setException( const std::exception& exception );
};

//----------------------------------------------------------------------------------------------------------------------

template<class T>
CFuture<T>::CFuture( const CFuture<T>& other )
{
	data = other.data;
}

template<class T>
CFuture<T>::CFuture( CFuture<T>&& other )
{
	data = std::move( other.data );
}

template<class T>
T& CFuture<T>::Get()
{
	std::unique_lock<std::mutex> lock( data->mutex );
	data->isNotWaiting.wait( lock, [&] { return data->state != FS_Waiting; } );
	if( data->state == FS_HasValue ) {
		return data->value;
	}
	throw data->exception;
}

template<class T>
bool CFuture<T>::TryGet( T& result )
{
	std::lock_guard<std::mutex> lock( data->mutex );
	if( data->state == FS_Waiting ) {
		return false;
	} else if( data->state == FS_HasValue ) {
		result = data->value;
		return true;
	}
	throw data->exception;
}

template<class T>
CFuture<T> CFuture<T>::Then( std::function<T( T )> function )
{
	CPromise<T> promise{};
	auto future = promise.GetFuture();
	auto executable = [this, function] ( CPromise<T>&& promise ) {
		try {
			promise.SetValue( function( Get() ) );
		} catch( const std::exception& exception ) {
			promise.SetException( exception );
		}
	};
	data->threads.emplace_back( executable, std::move( promise ) );
	return future;
}

template<class T>
void CFuture<T>::setValue( T&& value )
{
	std::lock_guard<std::mutex> lock( data->mutex );
	data->value = std::move( value );
	data->state = FS_HasValue;
	data->isNotWaiting.notify_one();
}

template<class T>
void CFuture<T>::setException( const std::exception& exception )
{
	std::lock_guard<std::mutex> lock( data->mutex );
	data->exception = exception;
	data->state = FS_HasException;
	data->isNotWaiting.notify_all();
}
