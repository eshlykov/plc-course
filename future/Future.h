#pragma once

#include <atomic>
#include <mutex>
#include <condition_variable>

template<class T>
class CPromise;

template<class T>
class CFuture {
	friend CPromise<T>;

public:
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
	std::exception_ptr exceptionPtr{};
	T_FutureState state{ FS_Waiting };
	std::mutex mutex{};
	std::condition_variable isFinished{};

	void setValue( T&& _value );
	void setException( std::exception_ptr _exceptionPtr );
};

//----------------------------------------------------------------------------------------------------------------------

template<class T>
class CPromise {
public:
	CFuture<T>& GetFuture();
	void SetValue( T&& value );
	void SetException( std::exception_ptr exceptionPtr );

private:
	CFuture<T> future{};
};

//----------------------------------------------------------------------------------------------------------------------
