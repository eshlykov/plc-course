#include "DestructorCallTests.h"

#include <cassert>
#include <stdexcept>

bool CDestructorCallTestClass::DestructrorNotCalled = true;

CDestructorCallTestClass::~CDestructorCallTestClass()
{
	DestructrorNotCalled = false;
}

void CheckNoDestructorCallsWhenThrow()
{
	try {
		CDestructorCallTestClass sample;
		throw std::runtime_error( "" );
	} catch( std::runtime_error ) {
		assert( CDestructorCallTestClass::DestructrorNotCalled );
	}
}