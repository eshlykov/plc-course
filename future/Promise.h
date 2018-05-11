#pragma once

#include "Future.h"

#include <exception>

template<class T>
class CPromise {
public:
	CPromise() = default;

	CFuture<T> GetFuture();
	void SetValue( T&& value );
	void SetException( const std::exception& exception );

private:
	CFuture<T> future{};
};

//----------------------------------------------------------------------------------------------------------------------

template<class T>
CFuture<T> CPromise<T>::GetFuture()
{
	return future;
}

template<class T>
void CPromise<T>::SetValue( T&& value )
{
	future.setValue( std::move( value ) );
}

template<class T>
void CPromise<T>::SetException( const std::exception& exception )
{
	future.setException( exception );
}
