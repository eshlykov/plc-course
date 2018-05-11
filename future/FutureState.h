#pragma once

#include <algorithm>
#include <any>
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

class CFutureState {
public:
	CFutureState() = default;

private:
	template<class T> friend class CFuture;

	TFutureState state{ FS_Waiting };
	std::any value{};
	std::exception exception{};
	std::mutex mutex{};
	std::condition_variable isNotWaiting{};
};
