#pragma once

#include <functional>
#include <string>

class CTypeId {
public:
	std::string Name;
	int HashCode;

	CTypeId() = delete;

	CTypeId( std::string name, int hashCode ) :
		Name( name ),
		HashCode( hashCode )
	{
	}
};

//----------------------------------------------------------------------------------------------------------------------

template <char const* className>
class CTypeIdentificator {
public:
	const CTypeId TypeId{ className, std::hash<std::string>{}( className ) };
};

//----------------------------------------------------------------------------------------------------------------------

#define Class( Class ) \
extern const char Class##TemplateParameter[] = #Class; \
class Class : public CTypeIdentificator<Class##TemplateParameter>

#define Extends( Base ) , Base

//----------------------------------------------------------------------------------------------------------------------

#define TypeId( object ) object.TypeId
