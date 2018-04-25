#include "TypeInfo.h"

#include <iostream>
#include <string>

class A {
};

class B {
};

ClassExtends( C, A, B ) {
public:
	int c = 17;
};

int main()
{
	C c;
	auto typeinfo = TypeId( c );
	std::cout << typeinfo.Name << " " << typeinfo.HashCode << std::endl;
	return 0;
}
