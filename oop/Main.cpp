#include <exception>
#include <iostream>

#include "VirtualClass.h"

VirtualClass( Base, int a = 1 )
DefineMethod( Base, OnlyBase, std::cout << "Base, OnlyBase: a = " << self->a << std::endl )
DefineMethod( Base, Both, std::cout << "Base, Both: a = " << self->a << std::endl )

VirtualClassDerived( Derived, Base, int b = 2 )
DefineMethod( Derived, OnlyDerived, std::cout << "Derived, OnlyDerived: b = " << self->b << std::endl )
DefineMethod( Derived, Both, std::cout << "Derived, Both: a = " << self->a << " b = " << self->b << std::endl )

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
