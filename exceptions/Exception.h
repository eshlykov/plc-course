#pragma once

#include <csetjmp>
#include <iostream>
#include <memory>
#include <type_traits>

class CThrowHandler;
extern std::unique_ptr<CThrowHandler> topThrowHandler;

//---------------------------------------------------------------------------------------------------------------------

enum TExceptionType : int {
	ET_BadAlloc = 1,
	ET_BadAnyCast,
	ET_BadArrayNewLength,
	ET_BadCast,
	ET_BadException,
	ET_BadFunctionCall,
	ET_BadOptionalAccess,
	ET_BadTypeId,
	ET_BadVariantAccess,
	ET_BadWeakPtr,
	ET_DomainError,
	ET_Exception,
	ET_FilesystemFilesystemError,
	ET_FutureError,
	ET_InvalidArgument,
	ET_IosBaseFailure,
	ET_LengthError,
	ET_LogicError,
	ET_NestedException,
	ET_OutOfRange,
	ET_OverflowError,
	ET_RangeError,
	ET_RegexError,
	ET_SystemError,
	ET_TxException,
	ET_UnderflowError
};

//---------------------------------------------------------------------------------------------------------------------

//class CManagedObject {
//public:
//	std::unique_ptr<CManagedObject> prevObject{ nullptr };
//
//	CManagedObject();
//	virtual ~CManagedObject();
//};

//---------------------------------------------------------------------------------------------------------------------

class CThrowHandler {
public:
	std::jmp_buf JumpBuffer{};
	std::unique_ptr<CThrowHandler> prevHandler{ nullptr };
	std::unique_ptr<CThrowHandler> nextHandler{ nullptr };
	//std::unique_ptr<CManagedObject> object{ nullptr };

	CThrowHandler() = default;
	~CThrowHandler() = default;
};

//---------------------------------------------------------------------------------------------------------------------

#define Try( tryBlock ) { \
	auto handler = std::move( topThrowHandler ); \
	topThrowHandler = std::make_unique<CThrowHandler>(); \
	topThrowHandler->prevHandler = std::move( handler ); \
	switch(  setjmp( topThrowHandler->JumpBuffer ) ) { \
		case 0: \
		{ \
			tryBlock; \
			break; \
		}

#define Catch( exceptionType, catchBlock ) \
		case TExceptionType::exceptionType: \
		{ \
			catchBlock; \
			break; \
		}

#define Finally( finallyBlock ) \
		default: \
			break; \
	} \
	{ \
		finallyBlock; \
	} \
}

#define Throw( exceptionType ) { \
	if( topThrowHandler->prevHandler == nullptr ) { \
		std::terminate(); \
	} \
	auto prevHandler = std::move( topThrowHandler->prevHandler ); \
	auto handler = std::move( topThrowHandler ); \
	topThrowHandler = std::move( prevHandler ); \
	topThrowHandler->nextHandler = std::move( handler ); \
	std::longjmp( topThrowHandler->nextHandler->JumpBuffer, exceptionType ); \
}
