#include "TypeInfo.h"

#include <iostream>
#include <string>

Class( A ) {
public:
	EnableRTTI( A );
	int ccc;
};

Class( B ) {
public:
	EnableRTTI( B );
	int ccccc;
};

ClassExtends( C, A, B ) {
public:
	EnableRTTI( C );
	int c = 17;
};

int main()
{
	C c;
	auto typeinfo = TypeId( c );
	std::cout << typeinfo.Name << " " << typeinfo.HashCode << std::endl;
	return 0;
}
