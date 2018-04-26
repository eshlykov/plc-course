#include <iostream>

struct A {
	int a = 0;
	virtual ~A() = default;
};

struct B : A {
	int b = 1;
	virtual ~B() = default;
};

struct StrangeCasted : B, A {
	int c = 2;
	virtual ~StrangeCasted() = default;
};

int main( int argc, char** argv )
{
	// Когда static и dynamic cast'ы дадут разные результаты?
	A* ptrA = new StrangeCasted;
	B* ptrB1 = static_cast<B*>( ptrA );
	B* ptrB2 = dynamic_cast<B*>( ptrA );

	std::cout << ptrA << std::endl;
	std::cout << ptrB1 << std::endl;
	std::cout << ptrB2 << std::endl;

	// Флаг /d1reportSingleClassLayoutStrangeCasted позволяет увидеть, как хранится класс в памяти.
	return 0;
}
