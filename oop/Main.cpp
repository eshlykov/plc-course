#include <exception>
#include <iostream>

#include "VirtualClass.h"

VirtualClass( Base, int a )
DefineMethod( Base, OnlyBase )
DefineMethod( Base, Both )

VirtualClassDerived( Derived, Base, int b )
DefineMethod( Derived, OnlyDerived )
DefineMethod( Derived, Both )

int main()
{
	try {
		Base* baseBase = new Base{};
		VirtualCall( baseBase, OnlyBase );
		VirtualCall( baseBase, Both );
		VirtualCall( baseBase, OnlyDerived );
	} catch( const std::logic_error& exception ) {
		std::cout << exception.what() << std::endl;
	}
	std::cout << std::endl;

	try {
		Derived* derivedDerived = new Derived{};
		VirtualCall( derivedDerived, OnlyBase );
		VirtualCall( derivedDerived, Both );
		VirtualCall( derivedDerived, OnlyDerived );
	} catch( const std::logic_error& exception ) {
		std::cout << exception.what() << std::endl;
	}
	std::cout << std::endl;

	try {
		Base* baseDerived = new Derived{};
		VirtualCall( baseDerived, OnlyBase );
		VirtualCall( baseDerived, Both );
		VirtualCall( baseDerived, OnlyDerived );
	} catch( const std::logic_error& exception ) {
		std::cout << exception.what() << std::endl;
	}
	std::cout << std::endl;
	return 0;
}