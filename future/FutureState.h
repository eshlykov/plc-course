#pragma once

#include <algorithm>
#include <condition_variable>
#include <exception>
#include <mutex>
#include <thread>
#include <vector>

enum TFutureState : int {
	FS_Waiting,
	FS_HasValue,
	FS_HasException,
};

//----------------------------------------------------------------------------------------------------------------------

template<class T>
class CFuture;


template<class T>
class CFutureState {
	friend CFuture<T>;

public:
	CFutureState() = default;

	~CFutureState();

private:
	TFutureState state{ FS_Waiting };
	T value{};
	std::exception exception{};
	std::vector<std::thread> threads{};
	std::mutex mutex{};
	std::condition_variable isNotWaiting{};
};

//----------------------------------------------------------------------------------------------------------------------

template<class T>
CFutureState<T>::~CFutureState()
{
	std::for_each( threads.begin(), threads.end(), [] ( auto& thread ) { thread.join(); } );
}
