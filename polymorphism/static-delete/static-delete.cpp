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
		::operator delete( p ); // ����� ����������� delete.
	}
};

int main( int argc, char** argv )
{
	Animal* sheep = new Sheep;
	sheep->Say();
	delete sheep;
	// ������ ���������� delete ��� Anumal? � ��� ����� �����������, ������� ��� delete, ������� ������ ����������, ������ �������� �� ����� ����������.
	// �� ���� ������ ���� ������ ���������� delete, ��� Animal*. �� ���������� ������. 
	// ������ ���� ������ ����������� ������, �� delete ���������� ��� Animal*!
	// ���� /d1reportSingleClassLayoutSheep ��������� �������, ��� �������� ����� � ������.
	return 0;
}
