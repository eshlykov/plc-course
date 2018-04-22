#pragma once

#include <exception>
#include <iostream>
#include <string>
#include <unordered_map>

enum THierarchicalType : int {
	HT_Base,
	HT_Derived,
	HT_Both
};

const std::unordered_map<THierarchicalType, std::string> HierarchicalTypeName{
	{ HT_Base, "base" },
	{ HT_Derived, "derived" }
};

#define VirtualClass( Class, Fields ) \
class Class { \
public:  \
	const static THierarchicalType ClassType = HT_Base; \
	static std::unordered_map<std::string, THierarchicalType> Methods; \
	const THierarchicalType ObjectType; \
	const std::string Name; \
	Fields; \
	\
	Class( std::string name = #Class, THierarchicalType objectType = HT_Base ) : \
		Name( name ), \
		ObjectType( objectType ) \
	{ \
	} \
}; \
std::unordered_map<std::string, THierarchicalType> Class::Methods{};

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

#define DefineMethod( Class, Method ) \
void* Class##Method = []{ \
	if( Class::Methods.find( #Method ) != Class::Methods.end() ) { \
		Class::Methods[#Method] = HT_Both; \
	} else { \
		Class::Methods[#Method] = Class::ClassType; \
	} \
	return nullptr; \
}();

#define VirtualCall( ObjectPtr, Method ) { \
	if( ObjectPtr->Methods.find( #Method ) == ObjectPtr->Methods.end() || \
		ObjectPtr->ObjectType == HT_Base && ObjectPtr->Methods.at(#Method) == HT_Derived ) \
	{ \
		throw std::logic_error( "Class " + ObjectPtr->Name + " does not have method " + #Method + "." );  \
	} \
	\
	const auto invokerType = ObjectPtr->Methods.at(#Method) == HT_Base ? HT_Base : ObjectPtr->ObjectType; \
	std::cout << "Class " + ObjectPtr->Name + " called method " + #Method + " defined in the " + HierarchicalTypeName.at(invokerType) + " class." << std::endl; \
}