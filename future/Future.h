#pragma once

#include "FutureState.h"

#include <exception>
#include <memory>
#include <mutex>
#include <condition_variable>

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

private:
	friend CPromise<T>;

	std::shared_ptr<CFutureState<T>> data{ new CFutureState<T>{} };
	std::mutex mutex{};
	std::condition_variable isFinished{};

	void setValue( T&& _value );
	void setException( const std::exception& _exception );
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
	std::unique_lock<std::mutex> lock( mutex );
	isFinished.wait( lock, [&] { return data->state != FS_Waiting; } );
	if( data->state == FS_HasException ) {
		throw data->exception;
	}
	return data->value;
}

template<class T>
bool CFuture<T>::TryGet( T& result )
{
	std::lock_guard<std::mutex> lock( mutex );
	if( data->state == FS_HasValue ) {
		result = data->value;
		return false;
	}
	if( data->state == FS_HasException ) {
		throw data->exception;
	}
	return false;
}

template<class T>
void CFuture<T>::setValue( T&& _value )
{
	std::lock_guard<std::mutex> lock( mutex );
	data->value = std::move( _value );
	data->state = FS_HasValue;
	isFinished.notify_one();
}

template<class T>
void CFuture<T>::setException( const std::exception& _exception )
{
	std::lock_guard<std::mutex> lock( mutex );
	data->exception = _exception;
	data->state = FS_HasException;
	isFinished.notify_one();
}
