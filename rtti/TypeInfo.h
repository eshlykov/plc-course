#pragma once

#include <algorithm>
#include <cctype>
#include <functional>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

class CTypeId {
public:
	std::string Name;
	std::size_t HashCode;

	CTypeId() = default;

	CTypeId( std::string name, std::size_t hashCode ) :
		Name( name ),
		HashCode( hashCode )
	{
	}
};

//----------------------------------------------------------------------------------------------------------------------

class CTypeIdStorage {
public:
	CTypeId TypeId;
};

//----------------------------------------------------------------------------------------------------------------------

std::unordered_set<char> OpenBracket{ '(', '[', '{', '<' };
std::unordered_set<char> CloseBracket{ ')', ']', '}', '>' };

bool GetBalance( const char c)
{
	// Do not need to check types of brackets because of the sequence of bracket is definitely correct.
	return ( OpenBracket.find( c ) != OpenBracket.end() ) ? 1 : ( CloseBracket.find( c ) != CloseBracket.end() ? -1 : 0);
}

std::vector<std::string> ExtractClassNamesList( std::string text )
{
	std::istringstream stream{ text };
	std::vector<std::string> words{};
	std::string word = "";
	char current;
	int balance = 0;

	while( stream.get( current ) ) {
		balance += GetBalance( current );
		if( current == ',' && balance == 0 ) {
			if( word != "" ) {
				// Now we are sure that word is some class name.
				words.push_back( word );
			}
			word = "";
		} else if( std::isspace( current ) && balance == 0 ) {
			if( word == "private" || word == "protected" || word == "public" ) {
				word = "";
			} else if( word == "virtual" ) {
				throw std::logic_error( "RTTI does not support virtual inheritance" );
			}
		} else if( word != "" || !std::isspace( current ) ) {
			// Don't write first space in the class name.
			word += current;
		}
	}
	if( word != "" ) {
		words.push_back( word );
	}
	return words;
}

//----------------------------------------------------------------------------------------------------------------------

std::unordered_map<std::string, std::size_t> ClassSizes;
std::unordered_map<std::string, std::vector<std::string>> ClassChildren;

void* UpdateTree( std::string child, std::string parents = "" )
{
	//auto parentsList = ExtractClassNamesList( parents );
	ClassChildren[child] = ExtractClassNamesList( parents );
	//for( const auto& parent : parentsList ) {
		//ClassChildren[child].push_back( parent );
	//}
	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

bool IsCastable( std::string base, std::string target )
{
	if( target.size() > 0 && target[target.size() - 1] == '*' ) {
		// Remove ending *.
		std::string dereferencedType;
		std::copy_n( target.begin(), target.size() - 1, std::back_inserter( dereferencedType ) );
		std::swap( target, dereferencedType );
	}
	if( base == target ) {
		return true;
	}
	if( ClassSizes[base] == 0 || ClassSizes[target] == 0 ) {
		// RTTI is not enabled or enabled in private section.
		return false;
	}

	for( const auto& child : ClassChildren[base] ) {
		if( child == target || IsCastable( child, target ) ) {
			return true;
		}
	}

	return false;
}

std::size_t CountShift( std::string base, std::string target )
{
	std::size_t shift = sizeof( CTypeIdStorage );
	for( const auto& child : ClassChildren[base] ) {
		if( child == target ) {
			return shift;
		} else if ( IsCastable( child, target ) ) {
			return shift + CountShift( child, target );
		} else {
			shift += ClassSizes[child];
		}
	}
	return shift;
}

//----------------------------------------------------------------------------------------------------------------------

#define Class( Class ) \
void* Class##Tree = UpdateTree( #Class ); \
class Class : virtual public CTypeIdStorage

#define ClassExtends( Class, ... ) \
void* Class##Tree = UpdateTree( #Class, #__VA_ARGS__ ); \
class Class : virtual public CTypeIdStorage, __VA_ARGS__

#define EnableRTTI( Class ) \
void* Class##RTTI = [this]{ \
	TypeId = CTypeId{ #Class, std::hash<std::string>{}( #Class ) };\
	ClassSizes[#Class] = sizeof( Class ); \
	return nullptr; \
}();

#define TypeId( object ) ( (object).TypeId )

#define DynamicCast( Type, Ptr ) ( IsCastable( TypeId( *Ptr ).Name, #Type ) ?  reinterpret_cast<Type>( Ptr + CountShift( TypeId( *Ptr ).Name, #Type ) ) : nullptr )
