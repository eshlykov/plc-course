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
public:
	CFutureState() = default;

private:
	friend CFuture<T>;

	TFutureState state{ FS_Waiting };
	T value{};
	std::exception exception{};
	std::mutex mutex{};
	std::condition_variable isNotWaiting{};
};
