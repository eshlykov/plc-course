#pragma once

#include <algorithm>
#include <csetjmp>
#include <memory>
#include <unordered_set>

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

class CManagedObject;

class CThrowHandler {
public:
	std::jmp_buf JumpBuffer{};
	std::unique_ptr<CThrowHandler> PrevHandler{ nullptr };
	std::unique_ptr<CThrowHandler> NextHandler{ nullptr };
	std::unordered_set<CManagedObject*> Objects{};

	CThrowHandler( std::unique_ptr<CThrowHandler>&& prevHandler = nullptr );
	~CThrowHandler();

	void ClearObjects();
};

//---------------------------------------------------------------------------------------------------------------------

class CManagedObject {
public:
	CManagedObject();
	virtual ~CManagedObject() = default;
};

//---------------------------------------------------------------------------------------------------------------------

CThrowHandler::CThrowHandler( std::unique_ptr<CThrowHandler>&& prevHandler ) :
	PrevHandler{ std::move( prevHandler ) }
{
}

CThrowHandler::~CThrowHandler()
{
	ClearObjects();
}

void CThrowHandler::ClearObjects()
{
	std::for_each( Objects.begin(), Objects.end(), [] ( CManagedObject* const pointer ) { pointer->~CManagedObject(); } );
	Objects.clear();
}

//---------------------------------------------------------------------------------------------------------------------

std::unique_ptr<CThrowHandler> topThrowHandler{ new CThrowHandler{} };

CManagedObject::CManagedObject()
{
	topThrowHandler->Objects.insert( this );
}

//---------------------------------------------------------------------------------------------------------------------

#define Try( tryBlock ) { \
	topThrowHandler = std::make_unique<CThrowHandler>( std::move( topThrowHandler ) ); \
	switch( setjmp( topThrowHandler->JumpBuffer ) ) { \
		case 0: \
		{ \
			tryBlock; \
			break; \
		}

#define Catch( type, catchBlock ) \
		case TExceptionType::type: \
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
	if( topThrowHandler->PrevHandler == nullptr ) { \
		std::terminate(); \
	} \
	\
	auto prevHandler = std::move( topThrowHandler->PrevHandler ); \
	auto handler = std::move( topThrowHandler ); \
	handler->ClearObjects(); \
	topThrowHandler = std::move( prevHandler ); \
	topThrowHandler->NextHandler = std::move( handler ); \
	\
	std::longjmp( topThrowHandler->NextHandler->JumpBuffer, exceptionType ); \
}
