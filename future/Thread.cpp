#include "Thread.h"

#include <exception>
#include <iostream>
#include <thread>

CThread::CThread( std::thread && _worker ) :
	thread{ std::move( _worker ) }
{
}

CThread::~CThread()
{
	try {
		if( thread.joinable() ) {
			thread.join();
		}
	} catch( const std::system_error& exception ) {
		std::cerr << exception.what() << std::endl;
	}
}
