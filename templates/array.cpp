#include <cstring>
#include <iostream>
#include <memory>
#include <string>
using namespace std;

// Хотим не просто массив, а массив обработчиков
struct ITextHandler {
	virtual ~ITextHandler() = default;
	virtual string Process( const string& text ) const = 0;
};

struct LowercaseHandler : ITextHandler {
	string Process( const string& text ) const
	{
		auto res = text;
		for( auto&& symb : res ) {
			symb = tolower( symb );
		}
		return res;
	}
};

struct TitlecaseHandler : ITextHandler {
	string Process( const string& text ) const
	{
		auto res = text;
		bool isWordStart = true;
		for( size_t i = 0; i < res.size(); ++i ) {
			if( isWordStart && res[i] != ' ' ) {
				res[i] = toupper( res[i] );
				isWordStart = false;
			} else if( res[i] == ' ' ) {
				isWordStart = true;
			} else if( !isWordStart ) {
				res[i] = tolower( res[i] );
			}
		}
		return res;
	}
};

template <typename T>
class HandlersArray {
public:
	~HandlersArray()
	{
		delete[] buffer;
	}

	void Add( const T& element )
	{
		buffer[size] = element;
		++size;
		tryResize();
	}

	int Size() const
	{
		return size;
	}

	T& operator[]( int pos )
	{
		return buffer[pos];
	}

	const T& operator[]( int pos ) const
	{
		return buffer[pos];
	}

	// Добавляем операцию-обработчик, запускающую по очереди все хэндлеры
	string Apply( const string& text ) const
	{
		string accumulator = text;
		for( int i = 0; i < size; ++i ) {
			accumulator = buffer[i]->Process( accumulator );
		}
		return accumulator;
	}

private:
	static constexpr int initialCapacity = 4;
	T* buffer = new T[initialCapacity];
	int size = 0;
	int capacity = initialCapacity;

	void tryResize()
	{
		if( size != capacity ) {
			return;
		}

		int newCapacity = 2 * capacity;
		T* newBuffer = new T[newCapacity];

		for( int i = 0; i < capacity; ++i ) {
			newBuffer[i] = std::move( buffer[i] );
		}

		capacity = newCapacity;
		std::swap( buffer, newBuffer );

		delete newBuffer;
	}
};

int main()
{
	HandlersArray<shared_ptr<ITextHandler>> handlers;
	handlers.Add( make_shared<LowercaseHandler>() );
	cout << handlers.Apply( "SaMple teXt" ) << endl;
	return 0;
}
