#include <iostream>

const int MagicConst = 22;

void foo() {
	const int start = 0;
	int const* searcher = &start;

	while( *searcher != MagicConst ) {
		++searcher;
	}

	std::cout << ( searcher - &start ) << std::endl;
}

void bar()
{
	int searched = MagicConst;
	foo();
}

int main()
{
	bar();
 	return 0;
}
