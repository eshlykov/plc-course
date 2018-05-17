#pragma once

#include <exception>
#include <iostream>
#include <functional>
#include <string>
#include <unordered_map>

enum THierarchicalType : int {
	HT_Base,
	HT_Derived
};

#define VirtualClass( Class, Fields ) \
class Class { \
public:  \
	const static THierarchicalType ClassType = HT_Base; \
	static std::unordered_map<std::string, std::function<void()>> BaseMethods; \
	static std::unordered_map<std::string, std::function<void()>> DerivedMethods; \
	const THierarchicalType ObjectType; \
	const std::string Name; \
	Fields; \
	\
	Class( const std::string& name = #Class, THierarchicalType objectType = HT_Base ) : \
		Name( name ), \
		ObjectType( objectType ) \
	{ \
	} \
}; \
std::unordered_map<std::string, std::function<void()>> Class::BaseMethods{}; \
std::unordered_map<std::string, std::function<void()>> Class::DerivedMethods{};

#define VirtualClassDerived( Class, Base, Fields ) \
class Class : public Base { \
public:  \
	const static THierarchicalType ClassType = HT_Derived; \
	Fields; \
	\
	Class() : \
		Base( #Class, HT_Derived ) \
	{ \
	} \
};

#define DefineMethod( Class, Method, Body ) \
void* Class##Method = []{ \
	if( Class::ClassType == HT_Base ) { \
		Class::BaseMethods[#Method] = [&]{ Body; }; \
	} else { \
		Class::DerivedMethods[#Method] = [&]{ Body; }; \
	} \
	return nullptr; \
}();

#define VirtualCall( ObjectPtr, Method ) { \
	if( ObjectPtr->ObjectType == HT_Base ) { \
		if( ObjectPtr->BaseMethods.find( #Method ) != ObjectPtr->BaseMethods.end() ) { \
			ObjectPtr->BaseMethods.at( #Method )(); \
		} else { \
			throw std::logic_error( "Class " + ObjectPtr->Name + " does not have method " + #Method + "." ); \
		} \
	} else { \
		if( ObjectPtr->DerivedMethods.find( #Method ) != ObjectPtr->DerivedMethods.end() ) { \
			ObjectPtr->DerivedMethods.at( #Method )(); \
		} else if( ObjectPtr->BaseMethods.find( #Method ) != ObjectPtr->BaseMethods.end() ) { \
			ObjectPtr->BaseMethods.at( #Method )(); \
		} else { \
			throw std::logic_error( "Class " + ObjectPtr->Name + " does not have method " + #Method + "." ); \
		} \
	} \
}
