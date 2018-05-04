#pragma once

#include <exception>
#include <iostream>
#include <thread>

class CThread {
public:
	CThread() = default;
	CThread( std::thread&& _thread );
	CThread( CThread&& other ) = default;
	~CThread();

private:
	std::thread thread{};
};

//----------------------------------------------------------------------------------------------------------------------

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
