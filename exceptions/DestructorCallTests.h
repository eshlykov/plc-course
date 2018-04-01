#pragma once

class CDestructorCallTestClass {

public:
	static bool DestructrorNotCalled;

	~CDestructorCallTestClass();
};

void CheckNoDestructorCallsWhenThrow();