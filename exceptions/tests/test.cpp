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

TEST( DefaultThrow, NoDestructorCall ) {
	try {
		CDestructorCallTestClass sample;
		throw std::runtime_error( "" );
	} catch( std::runtime_error ) {
	}

	EXPECT_TRUE( CDestructorCallTestClass::DestructrorNotCalled );
}

//---------------------------------------------------------------------------------------------------------------------

TEST( Catching, ThrowAndCatch )
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

TEST( Catching, TestFinally )
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

TEST( Catching, TypingExceotion )
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

TEST( Catching, NestedCatch )
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

TEST( Catching, NestedThrow )
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

TEST( Catching, SeveralTryBlocks )
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

TEST( Catching, NoAppropriateCatch )
{
	bool isExceptionCaught = false;
	bool isOutOfRangeCaugth = false;
	bool isNestedFinallyRun = false;

	Try (
		Try (
			Throw( ET_Exception)
		) Catch ( ET_OutOfRange,
			isOutOfRangeCaugth = true;
		) Finally (
			isNestedFinallyRun = true;
		)
	) Catch ( ET_Exception,
		isExceptionCaught = true;
	) Finally (
	)

	EXPECT_TRUE( isExceptionCaught );
	EXPECT_FALSE( isOutOfRangeCaugth );
	EXPECT_FALSE( isNestedFinallyRun );

}

//---------------------------------------------------------------------------------------------------------------------

void FailingFunction()
{
	Throw( ET_Exception );
}

TEST( Jumping, FailingFunction )
{
	bool isExceptionCaight = false;

	Try (
		FailingFunction();
	) Catch ( ET_Exception,
		isExceptionCaight = true;
	) Finally (
	)

	EXPECT_TRUE( isExceptionCaight );
}

//---------------------------------------------------------------------------------------------------------------------

void CatchingFunction()
{
	bool isExceptionCaight = false;

	Try (
		FailingFunction();
	) Catch ( ET_Exception,
		isExceptionCaight = true;
	) Finally (
	)

	EXPECT_TRUE( isExceptionCaight );
}

TEST( Jumping, CatchingFunction )
{
	CatchingFunction();
}

//---------------------------------------------------------------------------------------------------------------------

void FailingCatchingFunction()
{
	Try (
		FailingFunction();
	) Catch ( ET_Exception,
		Throw( ET_Exception );
	) Finally (
	)
}

TEST( Jumping, FailingCatchingFunction )
{
	bool isExceptionCaught = false;

	Try (
		FailingCatchingFunction();
	) Catch ( ET_Exception,
		isExceptionCaught = true;
	) Finally (
	)

	EXPECT_TRUE( isExceptionCaught );
}

//---------------------------------------------------------------------------------------------------------------------

class CTestObject : private CManagedObject {
public:
	static int ConstructorCallsCount;
	static int CopyConstructorCallsCount; 
	static int DestructorCallsCount;

	CTestObject()
	{
		++ConstructorCallsCount;
	}

	CTestObject( const CTestObject& )
	{
		++CopyConstructorCallsCount;
	}

	~CTestObject()
	{
		++DestructorCallsCount;
	}
};

int CTestObject::ConstructorCallsCount = 0;
int CTestObject::CopyConstructorCallsCount = 0;
int CTestObject::DestructorCallsCount = 0;

void ClearTestObjectCounters()
{
	CTestObject::ConstructorCallsCount = 0;
	CTestObject::CopyConstructorCallsCount = 0;
	CTestObject::DestructorCallsCount = 0;
}

TEST( Clearing, DestructorCall )
{
	ClearTestObjectCounters();

	Try (
		CTestObject object{};
		Throw( ET_Exception );
	) Catch ( ET_Exception,
	) Finally (
	)

	EXPECT_EQ( CTestObject::ConstructorCallsCount, 1 );
	EXPECT_EQ( CTestObject::DestructorCallsCount, 1 );
}

//---------------------------------------------------------------------------------------------------------------------

TEST( Clearing, NoDestructorExtraCall )
{
	ClearTestObjectCounters();

	CTestObject object{};

	Try (
		Throw( ET_Exception );
	) Catch ( ET_Exception,
	) Finally (
	)

	EXPECT_EQ( CTestObject::ConstructorCallsCount, 1 );
	EXPECT_EQ( CTestObject::DestructorCallsCount, 0 );
}

//---------------------------------------------------------------------------------------------------------------------

void CreateObjectAndFail()
{
	CTestObject objets{};
	Throw( ET_Exception );
}

TEST( Clearing, ObjectInFunction )
{
	ClearTestObjectCounters();

	Try (
		CreateObjectAndFail();
	) Catch ( ET_Exception,
	) Finally (
	)

	EXPECT_EQ( CTestObject::ConstructorCallsCount, 1 );
	EXPECT_EQ( CTestObject::DestructorCallsCount, 1 );
}

//---------------------------------------------------------------------------------------------------------------------

void TakeObjectCopyAndFail( CTestObject object )
{
	Throw( ET_Exception );
}

TEST( Clearing, ObjectCopyInParameters )
{
	ClearTestObjectCounters();

	CTestObject object{};

	Try (
		TakeObjectCopyAndFail( object );
	) Catch ( ET_Exception,
	) Finally (
	)

	EXPECT_EQ( CTestObject::ConstructorCallsCount, 1 );
	EXPECT_EQ( CTestObject::CopyConstructorCallsCount, 1 );
	EXPECT_EQ( CTestObject::DestructorCallsCount, 1 );
}
