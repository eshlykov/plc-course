struct A {
	int a = 0;
};

struct B : virtual A {
	int b = 1;
};

struct C : virtual A {
	int c = 2;
};

struct InheritanceClass : B, C {
	int d = 3;
};


int main( int argc, char** argv )
{
	// ���� /d1reportSingleClassLayoutInheritanceClass ��������� �������, ��� �������� ����� � ������.
	return 0;
}
