#pragma once

#include <algorithm>
#include <any>
#include <condition_variable>
#include <exception>
#include <mutex>
#include <thread>
#include <type_traits>
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
	template<class U> friend class CFuture;
	template<class U> friend class CFutureState;

	TFutureState state{ FS_Waiting };
	T value{};
	std::exception exception{};
	std::mutex mutex{};
	std::condition_variable isNotWaiting{};
};