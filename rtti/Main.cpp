#include "TypeInfo.h"

#include <iostream>
#include <string>

Class( A ) {
public:
	EnableRTTI( A );
	int a = 0;
};

Class( B ) {
public:
	EnableRTTI( B );
	int b = 1;
};

ClassExtends( C, public A, public B ) {
public:
	EnableRTTI( C );
	int c = 2;
};

int main()
{
	A* ptrC = new C;
	A* ptrA = new A();

	if( DynamicCast( C*, ptrC ) != nullptr ) {
		std::cout << "Can be casted to C*" << std::endl;
	} else {
		std::cout << "Cannot be casted to C*" << std::endl;
	}
	if( DynamicCast( B*, ptrC ) != nullptr ) {
		std::cout << "Can be casted to B*" << std::endl;
	} else {
		std::cout << "Cannot be casted to B*" << std::endl;
	}
	if( DynamicCast( B*,  ptrA ) != nullptr ) {
		std::cout << "Can be casted to B*" << std::endl;
	} else {
		std::cout << "Cannot be casted to B*" << std::endl;
	}
	return 0;
}
