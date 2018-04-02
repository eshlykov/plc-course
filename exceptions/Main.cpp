#include "Exception.h"
#include "DestructorCallTests.h"

#include <iostream>
#include <memory>

void CheckAll()
{
	CheckNoDestructorCallsWhenThrow();
}

//---------------------------------------------------------------------------------------------------------------------

class CTestObject : private CManagedObject {
public:
	static int ObjectsCount;
	int InstanceNumber;

	CTestObject() :
		InstanceNumber{ ++ObjectsCount }
	{
		std::cout << InstanceNumber << "th object is created: " << this << "." << std::endl;
	}

	~CTestObject()
	{
		std::cout << InstanceNumber << "th object is destroyed: " << this << "." << std::endl;
	}
};

int CTestObject::ObjectsCount = 0;

//---------------------------------------------------------------------------------------------------------------------

void FailingFunction()
{
	CTestObject object{};
	std::cout << "ET_Exception is raised." << std::endl;
	Throw( ET_Exception );
}

//---------------------------------------------------------------------------------------------------------------------

//void FailingFunction()
//{
//	CTestObject object{};
//	std::cout << "ET_Exception is raised." << std::endl;
//	Throw( ET_Exception );
//}

//---------------------------------------------------------------------------------------------------------------------

//void FailingFunction()
//{
//	CTestObject object{};
//	std::cout << "ET_Exception is raised." << std::endl;
//	Throw( ET_Exception );
//}

//---------------------------------------------------------------------------------------------------------------------

int main()
{
	//CheckAll();

	Try(
		FailingFunction();
	) Catch ( ET_OverflowError,
		std::cout << "ET_OverflowError is caught." << std::endl;
	) Catch( ET_Exception,
		std::cout << "ET_Exception is caught." << std::endl;
	) Catch( ET_BadException,
		std::cout << "ET_BadException is caught." << std::endl;
	) Finally (
		std::cout << "All objects are destroyed." << std::endl;
	)

	return 0;
}
