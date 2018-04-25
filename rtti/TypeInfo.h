#pragma once

#include <cctype>
#include <functional>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class CTypeId {
public:
	const std::string Name;
	const int HashCode;

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

std::unordered_set<char> openBracket{ '(', '[', '{', '<' };
std::unordered_set<char> closeBracket{ ')', ']', '}', '>' };

bool Is( const char c, const std::unordered_set<char>& set )
{
	return set.find( c ) != set.end();
}

std::string GetClassName( std::istringstream& stream, const std::string& prefix = "" )
{
	// The problem is classes like CSomeClass<CClass1, CClass2<CClass3>, std::string>,
	// that's why we need to count balance.
	int balance = 0;

	for( const auto& c : prefix ) {
		// No need to divide by types of brackets because the sequence is definitely correct.
		balance += Is( c, openBracket ) ? 1 : ( Is( c, closeBracket ) ? -1 : 0);
	}

	std::string className = prefix;

	char c;
	while( stream.get( c ) ) {
		if( std::isspace( c ) ) {
			continue;
		}
		if( c == ',' && balance == 0 ) {
			return className;
		}
		// If balance is not zero, the next char will be part of class name even when char is comma.
		className += c;
		balance += Is( c, openBracket ) ? 1 : ( Is( c, closeBracket ) ? -1 : 0 );
	}

	return className;
}

std::vector<std::string> ExtractClassNamesList( std::string text )
{
	std::istringstream stream{ "CTypeIdentificator<>," + text };
	std::vector<std::string> words{};
	std::string word;
	std::string className = "";

	while( stream >> word ) {
		if( word == "public" || word == "protected" || word == "private" || word == "virtual" ) {
			// Next token is a class name.
			className = GetClassName( stream );
		} else if( word == "virtual" ) {
			throw std::logic_error( "RTTI does not support virtual inheritance." );
		} else {
			// Default private, so we just read part of class name.
			className = GetClassName( stream, word );
		}
		if( className != "" ) {
			words.push_back( className );
		}
	}
	if( className != "" ) {
		words.push_back( className );
	}

	return words;
}

//----------------------------------------------------------------------------------------------------------------------

std::unordered_map<std::string, int> ClassSizes;
std::unordered_map<std::string, std::vector<std::string>> ClassTrees;

void* AddTree( std::string derivedClassName, std::string baseClasses )
{
	ClassTrees[derivedClassName] = ExtractClassNamesList( baseClasses );
	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

int CountShift( std::string from, std::string to )
{
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

#define ClassExtends( Class, ... ) \
extern const char Class##Name[] = #Class; \
void* Class##Tree = AddTree( #Class, #__VA_ARGS__ ); \
\
class Class : public CTypeIdentificator<Class##Name>, __VA_ARGS__

#define TypeId( object ) object.TypeId

#define DynamicCast( Type, Ptr ) static_cast<Type>( Ptr + CountShift( TypeId( *Ptr ).Name, #Type ) )
