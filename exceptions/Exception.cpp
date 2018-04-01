#include "Exception.h"

#include <cassert>
#include <iostream>

std::unique_ptr<CThrowHandler> topThrowHandler{ new CThrowHandler{} };

//---------------------------------------------------------------------------------------------------------------------

//CManagedObject::CManagedObject()
//{
//	if( topThrowHandler != nullptr ) {
//		//prevObject = std::move( topThrowHandler->GetObject() );
//		//topThrowHandler->SetObject( this );
//	}
//}
//
//CManagedObject::~CManagedObject()
//{
//	if( topThrowHandler != nullptr ) {
//		//topThrowHandler->SetObject( prevObject.release() );
//	}
//}