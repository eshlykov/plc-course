#pragma once

#include <atomic>
#include <exception>
#include <mutex>
#include <condition_variable>

template<class T>
class CPromise;

template<class T>
class CFuture {
	friend CPromise<T>;

public:
	CFuture() = default;
	CFuture( CFuture<T>&& other );

	T& Get();
	bool TryGet( T& result );

private:
	enum T_FutureState : int {
		FS_Waiting,
		FS_HasValue,
		FS_HasException,
	};

	T value{};
	std::exception exception{};
	T_FutureState state{ FS_Waiting };
	std::mutex mutex{};
	std::condition_variable isFinished{};

	void setValue( T&& _value );
	void setException( const std::exception& _exception );
};

//----------------------------------------------------------------------------------------------------------------------

template<class T>
class CPromise {
public:
	CFuture<T>& GetFuture();
	void SetValue( T&& value );
	void SetException( const std::exception& exception );

private:
	CFuture<T> future{};
};

//----------------------------------------------------------------------------------------------------------------------

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
		throw exception;
	}
	return value;
}

template<class T>
bool CFuture<T>::TryGet( T& result )
{
	std::lock_guard<std::mutex> lock( mutex );
	if( state == FS_HasValue ) {
		result = value;
		return false;
	}
	if( state == FS_HasException ) {
		throw exception;
	}
	return false;
}

template<class T>
void CFuture<T>::setValue( T&& _value )
{
	std::lock_guard<std::mutex> lock( mutex );
	value = std::move( _value );
	state = FS_HasValue;
	isFinished.notify_one();
}

template<class T>
void CFuture<T>::setException( const std::exception& _exception )
{
	std::lock_guard<std::mutex> lock( mutex );
	exception = _exception;
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
void CPromise<T>::SetValue( T && value )
{
	future.setValue( std::move( value ) );
}

template<class T>
void CPromise<T>::SetException( const std::exception& exception )
{
	future.setException( exception );
}
