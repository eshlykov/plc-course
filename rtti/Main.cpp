#include "TypeInfo.h"

#include <iostream>
#include <string>

Class( A ) {
	int ccc;
};

Class( B ) {
	int ccccc;
};

ClassExtends( C, A ) {
	int c = 17;
};

EnableCast( A );
EnableCast( B );
EnableCast( C );

int main()
{
	C c;
	auto typeinfo = TypeId( c );
	std::cout << typeinfo.Name << " " << typeinfo.HashCode << std::endl;
	return 0;
}
