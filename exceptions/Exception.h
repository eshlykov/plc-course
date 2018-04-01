#pragma once

#include <csetjmp>

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



//---------------------------------------------------------------------------------------------------------------------

#define Try( tryBlock ) { \
	std::jmp_buf jumpBuffer; \
	switch( setjmp( jumpBuffer ) ) { \
		case 0: \
			tryBlock \
			break;

#define Catch( exceptionType, catchBlock ) \
		case TExceptionType::exceptionType: \
			catchBlock \
			break;

#define Finally( finallyBlock ) \
		default: \
			break; \
	} \
	finallyBlock \
}

#define Throw( exception ) \
    std::longjmp( jumpBuffer, exception );
