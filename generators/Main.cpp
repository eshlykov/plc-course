#include <fstream>
#include <iostream>
#include <string>

class CTokenGenerator {
public:
	class CTokenIterator {
	public:
		std::string operator*() const
		{
			return token;
		}

		bool operator==( const CTokenIterator& other ) const
		{
			return index == other.index || isFinished && other.isFinished;
		}
		bool operator!=( const CTokenIterator& other ) const
		{
			return !( *this == other );
		}

		CTokenIterator& operator++()
		{
			++index;
			isFinished = generator.readNextToken( token );
			return *this;
		}

	private:
		CTokenIterator( CTokenGenerator& _generator, const int _index, bool _isFinished = false ) :
			generator( _generator ), index( _index ), isFinished( _isFinished )
		{
		}

		CTokenGenerator& generator;
		int index{ 0 };
		std::string token{};
		bool isFinished{ false };

		friend class CTokenGenerator;
	};

	explicit CTokenGenerator( std::istream& _istream ) :
		istream( _istream )
	{
	}

	CTokenIterator begin()
	{
		return CTokenIterator( *this, 0, false );
	}

	CTokenIterator end()
	{
		return CTokenIterator( *this, -1, true );
	}

	bool readNextToken( std::string& token )
	{
		istream >> token;
		return istream.eof();
	}

private:
	std::istream& istream;
};

int main()
{
	std::ifstream fin{ "text.txt", std::ios::in };
	if( fin.is_open() == false ) {
		std::abort();
	}
	for( auto&& token : CTokenGenerator( fin ) ) {
		std::cout << token << std::endl;
	}
	return 0;
}
