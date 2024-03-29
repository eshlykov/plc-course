#include "pch.h"

#include "../Async.h"
#include "../Future.h"
#include "../Promise.h"

#include <chrono>
#include <exception>
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

CThreadPool<std::any> CAsync::pool{ 4 };

TEST( FuturePromise, SetValue )
{
	CPromise<int> promise{};
	auto future = promise.GetFuture();

	promise.SetValue( 42 );

	EXPECT_EQ( future.Get(), 42 );
}

//----------------------------------------------------------------------------------------------------------------------

TEST( FuturePromise, SetException )
{
	CPromise<int> promise{};
	auto future = promise.GetFuture();

	promise.SetException( std::runtime_error( "For test needs." ) );

	try {
		future.Get();
		EXPECT_TRUE( false );
	} catch( const std::exception& exception ) {
		EXPECT_STREQ( exception.what(), "For test needs." );
	}
}

//----------------------------------------------------------------------------------------------------------------------

TEST( FuturePromise, TryGetValue )
{
	CPromise<int> promise{};
	auto future = promise.GetFuture();

	promise.SetValue( 42 );

	int result;
	EXPECT_TRUE( future.TryGet( result ) );
	EXPECT_EQ( result, 42 );
}

//----------------------------------------------------------------------------------------------------------------------

TEST( FuturePromise, TryGetWaiting )
{
	CPromise<int> promise{};
	auto future = promise.GetFuture();

	int result;
	EXPECT_FALSE( future.TryGet( result ) );
}

//----------------------------------------------------------------------------------------------------------------------

TEST( FuturePromise, TryGetException )
{
	CPromise<int> promise{};
	auto future = promise.GetFuture();

	promise.SetException( std::runtime_error( "For test needs." ) );

	try {
		int result;
		future.TryGet( result );
		EXPECT_TRUE( false );
	} catch( const std::exception& exception ) {
		EXPECT_STREQ( exception.what(), "For test needs." );
	}
}

//----------------------------------------------------------------------------------------------------------------------

TEST( Async, GetAsyncValue )
{
	auto future = CAsync::Async<int>( AT_Async, [&] () -> int { return 42; } );

	EXPECT_EQ( future.Get(), 42 );
}

//----------------------------------------------------------------------------------------------------------------------

TEST( Async, GetAsyncException )
{
	auto future = CAsync::Async<int>( AT_Async, [&] () -> int { throw std::runtime_error( "For test needs." ); } );

	try {
		future.Get();
		EXPECT_TRUE( false );
	} catch( const std::exception& exception ) {
		EXPECT_STREQ( exception.what(), "For test needs." );
	}
}

//----------------------------------------------------------------------------------------------------------------------

TEST( Async, TryGetAsyncValue )
{
	auto future = CAsync::Async<int>( AT_Async, [&] () -> int { return 42; } );

	std::this_thread::sleep_for( 2s ); // Waut to be sure that future is ready.

	int result;
	EXPECT_TRUE( future.TryGet( result ) );
	EXPECT_EQ( result, 42 );

	try {
		future.Get();
	} catch( ... ) {
	}
}

//----------------------------------------------------------------------------------------------------------------------

TEST( Async, TryGetAsyncWaiting )
{
	auto future = CAsync::Async<int>( AT_Async, [&] () -> int {
		std::this_thread::sleep_for( 2s );
		return 42;
	} );

	try {
		int result;
		EXPECT_FALSE( future.TryGet( result ) ); // Either false or exception, most likely false.
	} catch( const std::exception& exception ) {
		EXPECT_STREQ( exception.what(), "For test needs." );
	}

	try {
		future.Get();
	} catch( ... ) {
	}
}

//----------------------------------------------------------------------------------------------------------------------

TEST( Async, TryGetAsyncException )
{
	auto future = CAsync::Async<int>( AT_Async, [&] () -> int { throw std::runtime_error( "For test needs." ); } );

	std::this_thread::sleep_for( 2s ); // Waut to be sure that future is ready.

	try {
		int result;
		EXPECT_TRUE( future.TryGet( result ) ); // Either false or exception, most likely exception.
	} catch( const std::exception& exception ) {
		EXPECT_STREQ(exception.what(), "For test needs." );
	}

	try {
		future.Get();
	} catch( ... ) {
	}
}

//----------------------------------------------------------------------------------------------------------------------

TEST( Async, TryGetSyncValue )
{
	auto start = std::chrono::high_resolution_clock::now();
	auto future = CAsync::Async<int>( AT_Sync, [&] () -> int {
		std::this_thread::sleep_for( 2s );
		return 42;
	} );
	auto end = std::chrono::high_resolution_clock::now();

	int result;
	EXPECT_TRUE( future.TryGet( result ) );
	EXPECT_EQ( result, 42 );

	std::chrono::duration<double, std::milli> elapsed = end - start;
	EXPECT_GE( elapsed.count(), 2000 );

	try {
		future.Get();
	} catch( ... ) {
	}
}

//----------------------------------------------------------------------------------------------------------------------

TEST( Async, TryGetSyncException )
{
	auto start = std::chrono::high_resolution_clock::now();
	auto future = CAsync::Async<int>( AT_Sync, [&] () -> int {
		std::this_thread::sleep_for( 2s );
		throw std::runtime_error( "For test needs." );
	} );
	auto end = std::chrono::high_resolution_clock::now();

	try {
		int result;
		future.TryGet( result );
		EXPECT_TRUE( false );
	} catch( const std::exception& exception ) {
		EXPECT_STREQ( exception.what(), "For test needs." );
	}

	std::chrono::duration<double, std::milli> elapsed = end - start;
	EXPECT_GE( elapsed.count(), 2000 );

	try {
		future.Get();
	} catch( ... ) {
	}
}

//----------------------------------------------------------------------------------------------------------------------

TEST( Async, NotOverloadedPool )
{
	std::vector<CFuture<int>> futures;
	auto start = std::chrono::high_resolution_clock::now();

	for( int i = 0; i < 3; ++i ) {
		futures.push_back( CAsync::Async<int>( AT_Async, [&] () -> int {
			std::this_thread::sleep_for( 4s );
			return 42;
		} ) );
	}

	auto future = CAsync::Async<int>( AT_Async, [&] () -> int {
		std::this_thread::sleep_for( 2s );
		return 42;
	} );

	auto end = std::chrono::high_resolution_clock::now();

	EXPECT_EQ( future.Get(), 42 );

	std::chrono::duration<double, std::milli> elapsed = end - start;
	EXPECT_LE( elapsed.count(), 2000 );  // Task in async mode so Async returns future immediately.

	for( auto& future : futures ) {
		try {
			future.Get();
		} catch( ... ) {
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

TEST( Async, OverloadedPool )
{
	std::vector<CFuture<int>> futures;
	auto start = std::chrono::high_resolution_clock::now();

	for( int i = 0; i < 4; ++i ) {
		futures.push_back( CAsync::Async<int>( AT_Async, [&] () -> int {
			std::this_thread::sleep_for( 4s );
			return 42;
		} ) );
	}

	// Sleep in order to be sure that all tasks above were added to pool before the next task.
	std::this_thread::sleep_for( 1s );

	auto future = CAsync::Async<int>( AT_Async, [&] () -> int {
		std::this_thread::sleep_for( 2s );
		return 42;
	} );

	auto end = std::chrono::high_resolution_clock::now();

	int result;
	EXPECT_TRUE( future.TryGet( result ) );
	EXPECT_EQ( result, 42 );

	std::chrono::duration<double, std::milli> elapsed = end - start;
	EXPECT_GE( elapsed.count(), 3000 ); // We in sync mode, and we have to wait for time.
	EXPECT_LE( elapsed.count(), 5000 ); // But we don't have to wait for long time.

	try {
		future.Get();
	} catch( ... ) {
	}
	for( auto& future : futures ) {
		try {
			future.Get();
		} catch( ... ) {
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

TEST( Async, ManyArguments )
{
	auto future = CAsync::Async<int, int, std::string>( AT_Async, [&] ( int number, std::string str ) -> int { return number + str.length(); }, 42, "Test string" );

	EXPECT_EQ( future.Get(), 42 + std::string{ "Test string" }.length() );
}

//----------------------------------------------------------------------------------------------------------------------

TEST( Then, SavingValues )
{
	CPromise<int> promise{};
	auto future = promise.GetFuture();
	std::function<int( int )> function = [] ( int number ) -> int { return -number; };
	auto next = future.Then( function );

	promise.SetValue( 42 );

	auto futureResult = future.Get();
	auto nextResult = next.Get();

	EXPECT_EQ( futureResult, 42 );
	EXPECT_EQ( nextResult, -42 );
}

//----------------------------------------------------------------------------------------------------------------------

TEST( Then, SavingWhenException )
{
	CPromise<int> promise{};
	auto future = promise.GetFuture();
	auto next = future.Then<int>( [&] ( int number ) -> int { throw std::runtime_error( "For test needs." ); } );

	promise.SetValue( 42 );

	auto futureResult = future.Get();
	EXPECT_EQ( futureResult, 42 );

	try {
		next.Get();
		EXPECT_TRUE( false );
	} catch( const std::exception& exception ) {
		EXPECT_STREQ( exception.what(), "For test needs." );
	}
}

//----------------------------------------------------------------------------------------------------------------------

TEST( Then, NotCallWhenException )
{
	CPromise<int> promise{};
	auto future = promise.GetFuture();
	auto next = future.Then<int>( [&] ( int number ) -> int {
		EXPECT_TRUE( false );
		return 0;
	} );

	promise.SetException( std::runtime_error( "For test needs." ) );

	try {
		future.Get();
	} catch( const std::exception& exception ) {
		EXPECT_STREQ( exception.what(), "For test needs."  );
	}

	try {
		next.Get();
	} catch( const std::exception& exception ) {
		EXPECT_STREQ( exception.what(), "For test needs." );
	}
}

//----------------------------------------------------------------------------------------------------------------------

TEST( Then, OtherType )
{
	CPromise<int> promise{};
	auto future = promise.GetFuture();
	auto next = future.Then<double>( [] ( int number ) -> double { return static_cast<double>( number ) / 5.0; } );

	promise.SetValue( 42 );

	auto futureResult = future.Get();
	auto nextResult = next.Get();

	EXPECT_EQ( futureResult, 42 );
	EXPECT_EQ( nextResult, 42.0 / 5.0 );
}
