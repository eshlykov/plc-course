#include "TypeInfo.h"

#include <iostream>
#include <string>

class A {
};

class B {
};

Class( CClass ) Extends( A ) Extends( B )  {
public:
	int cc = 17;
};

int main()
{
	CClass c;
	auto typeinfo = TypeId( c );
	std::cout << typeinfo.Name << " " << typeinfo.HashCode << std::endl;
	return 0;
}
