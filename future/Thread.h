#pragma once

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
