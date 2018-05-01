#include "Future.h"

template<class T>
CFuture<T>::CFuture( CFuture<T>&& other )
{
	value = std::move( other.value );
	state = other.state;
}

template<class T>
T& CFuture<T>::Get()
{
	std::unique_lock<std::mutex> lock;
	isFinished.wait( lock, [&] { return state != FS_Waiting; } );
	if( state == FS_HasException ) {
		throw exceptionPtr;
	}
	return value;
}

template<class T>
bool CFuture<T>::TryGet( T& result )
{
	std::lock_guard<std::mutex> lock;
	if( state == FS_HasValue ) {
		result = value;
		return false;
	}
	if( state == FS_HasException ) {
		throw exceptionPtr;
	}
	return false;
}

template<class T>
void CFuture<T>::setValue( T&& _value )
{
	std::lock_guard<std::mutex> lock;
	value = std::move( _value );
	state = FS_HasValue;
	isFinished.notify_one();
}

template<class T>
void CFuture<T>::setException( std::exception_ptr _exceptionPtr )
{
	std::lock_guard<std::mutex> lock;
	exceptionPtr = _exceptionPtr;
	state = FS_HasException;
	isFinished.notify_one();
}

//----------------------------------------------------------------------------------------------------------------------

template<class T>
CFuture<T>& CPromise<T>::GetFuture()
{
	return future;
}

template<class T>
void CPromise<T>::SetValue( T&& value )
{
	future.setValue( std::move( value ) );
}

template<class T>
void CPromise<T>::SetException( std::exception_ptr exceptionPtr )
{
	future.setException( ptr );
}

//----------------------------------------------------------------------------------------------------------------------