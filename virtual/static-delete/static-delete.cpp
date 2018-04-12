#include <iostream>

using namespace std;

class Animal {
public:
	virtual ~Animal()
	{
	}

	virtual void Say() = 0;
};

class Sheep : public Animal {
public:
	virtual ~Sheep()
	{
		cout << "Sheep is dead" << endl;
	}

	virtual void Say()
	{
		cout << "Sheep says baaaaa" << endl;
	}

	static void operator delete( void* p )
	{
		cout << "Reclaiming Sheep storage from " << p << endl;
		::operator delete( p ); // Вызов глобального delete.
	}
};

int main( int argc, char** argv )
{
	Animal* sheep = new Sheep;
	sheep->Say();
	delete sheep;
	// Почему вызывается delete для Anumal? У нас метод статический, поэтому тот delete, который должен вызываться, должен решаться на этапе компиляции.
	// По идее должен быть вызван глобальный delete, для Animal*. Но вызывается нужный. 
	// Однако если убрать виртуальные методы, то delete вызывается для Animal*!
	// Флаг /d1reportSingleClassLayoutSheep позволяет увидеть, как хранится класс в памяти.
	return 0;
}
