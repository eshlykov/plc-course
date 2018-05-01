#include"Future.h"

#include <iostream>
#include <numeric>
#include <vector>

void Accumulate( std::vector<int>::iterator first, std::vector<int>::iterator last, CPromise<int> promise )
{
	int sum = std::accumulate( first, last, 0 );
	promise.SetValue( std::move( sum ) );
}

int main()
{
	std::vector<int> numbers = { 1, 2, 3, 4, 5, 6 };
	CPromise<int> promise{};
	CFuture<int> future = std::move( promise.GetFuture() );
	std::thread thread( Accumulate, numbers.begin(), numbers.end(), std::move( promise ) );
	std::cout << future.Get() << std::endl;
	thread.join();
	return 0;
}
