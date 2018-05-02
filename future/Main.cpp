#include "Promise.h"

#include <array>
#include <iostream>
#include <functional>
#include <numeric>
#include <vector>

void Accumulate( std::vector<int>::iterator first, std::vector<int>::iterator last, CPromise<int>&& promise )
{
	int sum = std::accumulate( first, last, 0 );
	promise.SetValue( std::move( sum ) );
}

int main()
{
	std::vector<int> numbers;
	for( int i = 0; i < 100; ++i ) {
		numbers.emplace_back( i );
	}

	const int tasksCount = 10;
	std::array<CPromise<int>, tasksCount> promises{};
	std::vector<CFuture<int>> futures{};
	for( auto& promise : promises ) {
		futures.push_back( promise.GetFuture() );
	}

	const int chankSize = numbers.size() / tasksCount;
	std::vector<std::thread> threads;
	for( int i = 0; i < tasksCount; ++i ) {
		threads.emplace_back( Accumulate, numbers.begin() + i * chankSize, numbers.begin() + ( i + 1 ) * chankSize, std::move( promises[i] ) );
	}

	int sum = 0;
	for( auto& future : futures ) {
		auto f = future.Then( [] ( int n ) { return n * n; } );
		sum += f.Get();
	}

	std::cout << sum << std::endl;

	for( auto& thread : threads ) {
		thread.join();
	}
	return 0;
}
