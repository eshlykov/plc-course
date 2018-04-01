#include "Exception.h"
#include "DestructorCallTests.h"

#include <iostream>
#include <memory>

void CheckAll()
{
	CheckNoDestructorCallsWhenThrow();
}

int main()
{
	//CheckAll();

	Try(
		Throw( ET_Exception );
	) Catch ( ET_Exception,
		std::cout << "ok" << std::endl;
	) Finally (
		std::cout << "fin" << std::endl;
	)
	
	Try (
		std::cout << "try" << std::endl;
		Try (
			std::cout << "nested try" << std::endl;
			Throw( ET_BadWeakPtr );
		) Catch ( ET_InvalidArgument,
			std::cout << "nested catch InvalidArgument" << std::endl;
		) Catch ( ET_BadWeakPtr,
			std::cout << "nested catch BadWeakPtr" << std::endl;
			Throw( ET_InvalidArgument );
		) Catch ( ET_SystemError,
			std::cout << "nested catch SystemError" << std::endl;
		) Finally (
			std::cout << "nested finally" << std::endl;
		)
	) Catch ( ET_InvalidArgument,
		std::cout << "catch InvalidArgument" << std::endl;
	) Finally (
		std::cout << "finally" << std::endl;
	)

	return 0;
}
