#pragma once

#include <exception>

enum T_FutureState : int {
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

private:
	T_FutureState state{ FS_Waiting };
	T value{};
	std::exception exception{};
};

//----------------------------------------------------------------------------------------------------------------------
