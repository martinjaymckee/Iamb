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
#include <core.h>
#include <comparison.h>

//
// Fixed-Point Comparisons
//
TEST_CASE("Fixed-point numbers with s5.11 format are constructed", "[fixedpoint]") {
	using value_t = iamb::SignedFixedPoint<5, 11>; // This is an s5.11 fixed-point type
	const value_t one{1};
	const value_t half{0.5};
	const value_t seven{7};
	const value_t zero{0};
	const value_t neg_three{-3};
	const value_t one_and_a_quarter{1.25};

	SECTION("Check equality against direct construction") {
		CAPTURE(one.storage(), half.storage(), seven.storage(), zero.storage(), neg_three.storage(), one_and_a_quarter.storage());
		REQUIRE(one == value_t::Storage(1<<11));
		REQUIRE(half == value_t::Storage(1<<10));
		REQUIRE(seven == value_t::Storage(0b111<<11));
		REQUIRE(zero == value_t::Storage(0));
		REQUIRE(neg_three == value_t::Storage(0b11101<<11));
		REQUIRE(one_and_a_quarter == value_t::Storage(0b101<<9));
	};

	SECTION("Check equality against values") {
		REQUIRE(one == 1);
		REQUIRE(half == 0.5);
		REQUIRE(seven == 7);
		REQUIRE(zero == 0);
		REQUIRE(neg_three == -3);
		REQUIRE(one_and_a_quarter == 1.25);
	};
};
