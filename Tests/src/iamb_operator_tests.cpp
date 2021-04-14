//
// C++ Includes
//
#include <bitset>
#include <iomanip>
#include <iostream>

//
// Include Catch2 Testing Framework
//
#include <catch.hpp>

//
// Include Chandra Features to Test
//
#include <arithmetic.h>
#include <core.h>
#include <comparison.h>

//
// Fixed-Point Operations
//
TEST_CASE( "Fixed-point assignment operators", "[fixedpoint]" ) {
	using value_t = iamb::FixedPoint<int32_t, 6, 12>; // This is a s6.6 fixed-point type
	const value_t one{1};
	const value_t two{2};
	const value_t half{0.5};
	const value_t three{3};
	const value_t neg_two_half{-2.5};

	SECTION("Add Assignment") {
		value_t a = one;
		a += one;
		REQUIRE(a == two);
		a += 1;
		REQUIRE(a == three);
	};

	SECTION("Subtract Assignment") {
		value_t a = half;
		a -= three;
		REQUIRE(a == neg_two_half);
		a -= three;
		REQUIRE(a == value_t{-5.5});
	};

	SECTION("Multiply Assignment") {
		value_t a = three;
		a *= half;
		REQUIRE(a == value_t{1.5});
		a *= three;
		REQUIRE(a == value_t{4.5});
	};

	SECTION("Divide Assignment") {
		value_t a = one;
		a /= two;
		REQUIRE(a == half);
		a /= two;
		REQUIRE(a == value_t{0.25});
	};
};

TEST_CASE( "Fixed-point operators", "[fixedpoint]" ) {
	using value_t = iamb::FixedPoint<int32_t, 5, 12>; // This is a s7.5 fixed-point type
	const value_t one{1};
	const value_t two{2};
	const value_t three_half{3.5};
	const value_t neg_three_half{-3.5};
	const value_t seven{7};

	SECTION("Add Operator") {
		REQUIRE( (three_half + three_half) == seven);
		REQUIRE( (seven + one) == value_t{8});
	};

	SECTION("Subtract Operator") {
		REQUIRE( (seven - three_half) == three_half);
		REQUIRE( (two - one) == one);
	};
	
	SECTION("Multiply Operator") {
		REQUIRE( (2 * three_half) == seven);
		REQUIRE( (one * 2) == two);
		REQUIRE( (three_half * two) == seven);
	};

	SECTION("Divide Operator") {
		REQUIRE( (seven / two) == three_half);
		REQUIRE( (three_half / 0.875) == value_t{4});		
	};
	
	SECTION("Negation Operation") {
		REQUIRE(-one == value_t{-1});
		REQUIRE(-three_half == value_t{-3.5});
	};

	SECTION("Reciprocal Operation") {
		REQUIRE(reciprocal(two) == value_t{ 0.5 });
		REQUIRE(reciprocal(value_t{ 4 }) == value_t{ 0.25 });
	}
};
