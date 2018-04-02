#include "pch.h"

#include "../Exception.h"

//---------------------------------------------------------------------------------------------------------------------

class CDestructorCallTestClass {
public:
	static bool DestructrorNotCalled;

	~CDestructorCallTestClass()
	{
		DestructrorNotCalled = false;
	}
};

bool CDestructorCallTestClass::DestructrorNotCalled = true;

TEST( NoDefaultExceptions, DestructrorNotCalled ) {
	try {
		CDestructorCallTestClass sample;
		throw std::runtime_error( "" );
	} catch( std::runtime_error ) {
	}

	EXPECT_TRUE( CDestructorCallTestClass::DestructrorNotCalled );
}

//---------------------------------------------------------------------------------------------------------------------

TEST( CatchingExceptions, TestThrowAndCatch )
{
	bool isExceptuonThrown = true;
	bool isExceptionCaught = false;

	Try (
		Throw( ET_Exception );
		isExceptuonThrown = false;
	) Catch ( ET_Exception,
		isExceptionCaught = true;
	) Finally (
	)

	EXPECT_TRUE( isExceptuonThrown );
	EXPECT_TRUE( isExceptionCaught );
}

//---------------------------------------------------------------------------------------------------------------------

TEST( CatchingExceptions, TestFinally )
{
	bool isFinallyRun = false;

	Try (
		Throw( ET_Exception )
	) Catch ( ET_Exception,
	) Finally (
		isFinallyRun = true;
	)

	EXPECT_TRUE( isFinallyRun );
}

//---------------------------------------------------------------------------------------------------------------------

TEST( CatchingExceptions, TestTyping )
{
	bool isExceptionCaught = false;
	bool isOutOfRangeCaught = false;
	bool isOverflowErrorCaught = false;

	Try (
		Throw( ET_OutOfRange )
	) Catch ( ET_Exception,
		isExceptionCaught = true;
	) Catch ( ET_OutOfRange,
		isOutOfRangeCaught = true;
	) Catch ( ET_OverflowError,
		isOverflowErrorCaught = true;
	) Finally (
	)

	EXPECT_FALSE( isExceptionCaught );
	EXPECT_TRUE( isOutOfRangeCaught );
	EXPECT_FALSE( isOverflowErrorCaught );
}

//---------------------------------------------------------------------------------------------------------------------

TEST( CatchingExceptions, TestNestedCatch )
{
	bool isOutOfRangeCaugth = false;
	bool isNestedOutOfRangeCaught = false;

	Try (
		Try (
			Throw( ET_OutOfRange );
		) Catch ( ET_OutOfRange,
		isNestedOutOfRangeCaught = true;
		) Finally (
		)
	) Catch ( ET_OutOfRange,
		isOutOfRangeCaugth = true;
	) Finally (
	)

	EXPECT_FALSE( isOutOfRangeCaugth );
	EXPECT_TRUE( isNestedOutOfRangeCaught );
}

//---------------------------------------------------------------------------------------------------------------------

TEST( CatchingExceptions, TestNestedThrow )
{
	bool isOverflowErroCaught = false;

	Try (
		Try (
			Throw( ET_OutOfRange );
		) Catch ( ET_OutOfRange,
			Throw( ET_OverflowError );
		) Finally (
		)
	) Catch ( ET_OverflowError,
		isOverflowErroCaught = true;
	) Finally (
	)

	EXPECT_TRUE( isOverflowErroCaught );
}

//---------------------------------------------------------------------------------------------------------------------

TEST( CatchingExceptions, TestSeveralTryBlocks )
{
	bool isOverflowErroCaught = false;
	bool isOutOfRangeCaugth = false;

	Try (
		Throw( ET_OverflowError );
	) Catch ( ET_OverflowError,
		isOverflowErroCaught = true;
	) Catch ( ET_OutOfRange,
		isOutOfRangeCaugth = true;
	) Finally (
	)

	EXPECT_TRUE( isOverflowErroCaught );
	EXPECT_FALSE( isOutOfRangeCaugth );

	isOverflowErroCaught = false;
	isOutOfRangeCaugth = false;

	Try (
		Throw( ET_OutOfRange );
	) Catch ( ET_OverflowError,
		isOverflowErroCaught = true;
	) Catch ( ET_OutOfRange,
		isOutOfRangeCaugth = true;
	) Finally (
	)

	EXPECT_FALSE( isOverflowErroCaught );
	EXPECT_TRUE( isOutOfRangeCaugth );
}

//---------------------------------------------------------------------------------------------------------------------

class CTestObject : private CManagedObject {
public:
	static int ObjectsCount;

	CTestObject()
	{
		++ObjectsCount;
	}

	~CTestObject()
	{
		--ObjectsCount;
	}
};

int CTestObject::ObjectsCount = 0;

TEST( Clearing, DestructorCalling )
{
	Try(
		CTestObject object{};
		Throw( ET_Exception );
	) Catch ( ET_Exception,
	) Finally (
	)

	EXPECT_EQ( CTestObject::ObjectsCount, 0 );
}
