#pragma once

#include "Async.h"
#include "FutureState.h"
#include "Promise.h"
#include "Thread.h"

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

template<class T>
class CPromise;

class CAsync;

template<class T>
class CFuture {
public:
	CFuture() = default;
	CFuture( const CFuture<T>& other );
	CFuture( CFuture<T>&& other );

	T Get();
	bool TryGet( T& result );
	template<class U> CFuture<U> Then( std::function<U( T )> function );

private:
	friend CPromise<T>;
	friend CAsync;
	template<class U> friend class CFuture;

	std::shared_ptr<CFutureState<T>> data{ new CFutureState<T>{} };
	inline static std::vector<CThread> workers{};

	void setValue( T&& value );
	void setException( const std::exception& exception );
	template<class U> CFuture<U> castValue();
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
T CFuture<T>::Get()
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
template<class U>
inline CFuture<U> CFuture<T>::Then( std::function<U( T )> function )
{
	CPromise<U> promise{};
	auto currentFuture = *this;
	auto thenFuture = promise.GetFuture();
	auto executable = [currentFuture, promise, function] () mutable {
		try {
			promise.SetValue( function( currentFuture.Get() ) );
		} catch( const std::exception& exception ) {
			promise.SetException( exception );
		}
	};
	workers.emplace_back( std::move( std::thread{ executable } ) );
	return thenFuture;
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

template<class T>
template<class U>
CFuture<U> CFuture<T>::castValue()
{
	std::unique_lock<std::mutex> lock( data->mutex );
	data->isNotWaiting.wait( lock, [&] { return data->state != FS_Waiting; } );
	CFuture<U> future{};
	future.data->state = data->state;
	if( data->state == FS_HasValue ) {
		future.data->value = std::any_cast<U>( data->value );
	} else if( data->state == FS_HasException ) {
		future.data->exception = data->exception;
	}
	return future;
}
