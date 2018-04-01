#include "Exception.h"

#include <cassert>
#include <iostream>

std::unique_ptr<CThrowHandler> topThrowHandler{ new CThrowHandler{} };

//---------------------------------------------------------------------------------------------------------------------

CManagedObject::CManagedObject()
{
	topThrowHandler->objects.insert( std::move( std::unique_ptr<CManagedObject>( this ) ) );
}

CThrowHandler::CThrowHandler( std::unique_ptr<CThrowHandler>&& _prevHandler) :
	prevHandler{ std::move( _prevHandler )}
{
}
