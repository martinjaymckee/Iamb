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
// Fixed-Point Construction
//
TEST_CASE( "Fixed-point numbers of default type are constructed", "[fixedpoint]" ) {
	using value_t = iamb::FixedPoint<>; // This is a default s16.16 fixed-point type

	SECTION("Direct Storage Construction") {
		const value_t a = value_t::Storage(1<<16);
		REQUIRE(a == 1);
		const value_t b = value_t::Storage(1<<15);
		REQUIRE(b == 0.5);
		const value_t c = value_t::Storage(0xFFFF0000);
		REQUIRE(c == -1);
	}

	SECTION("Default Construction") {
		const value_t a;
		REQUIRE(a == 0);
	};

	SECTION("Construction from Float") {
		const value_t a{float(1.0)};
		REQUIRE(a == 1);
		const value_t b{0.5};
		REQUIRE(b == 0.5);
		const value_t c{float(-1.0)};
		REQUIRE(c == -1);
	};

	SECTION("Construction from Double") {
		const value_t a{double(1.0)};
		REQUIRE(a == 1);
		const value_t b{double(0.5)};
		REQUIRE(b == 0.5);
		const value_t c{double(-1.0)};
		REQUIRE(c == -1);
	};

	SECTION("Construction from Int") {
		const value_t a{int(1)};
		REQUIRE(a == 1);
		const value_t b{int(-1)};
		REQUIRE(b == -1);
	};

	SECTION("Construction from an integer division") {
		const value_t a = value_t::IntDiv(36, 8);
		REQUIRE(a == 4.5);
		const value_t b = value_t::IntDiv(1, 2);
		REQUIRE(b == 0.5);
	};
};

TEST_CASE("Fixed-point numbers with s2.2 format are constructed", "[fixedpoint]") {
	using value_t = iamb::FixedPoint<int8_t, 2, 4>; // This is an s2.2 fixed-point type

	SECTION("Direct Storage Construction") {
		const value_t a = value_t::Storage(1 << 2);
		REQUIRE(a == 1);
		const value_t b = value_t::Storage(1 << 1);
		REQUIRE(b == 0.5);
		const value_t c = value_t::Storage(0b00001100);
//		WARN("The storage value for -1 is 0b" << std::bitset<8>(c.storage()));
		REQUIRE(c == -1);
	}

	SECTION("Default Construction") {
		const value_t a;
		REQUIRE(a == 0);
	};

	SECTION("Construction from Float") {
		const value_t a{ float(1.0) };
		REQUIRE(a == 1);
		const value_t b{ 0.5 };
		REQUIRE(b == 0.5);
		const value_t c{ float(-1.0) };
		REQUIRE(c == -1);
	};

	SECTION("Construction from Double") {
		const value_t a{ double(1.0) };
		REQUIRE(a == 1);
		const value_t b{ double(0.5) };
		REQUIRE(b == 0.5);
		const value_t c{ double(-1.0) };
		REQUIRE(c == -1);
	};

	SECTION("Construction from Int") {
		const value_t a{ int(1) };
		REQUIRE(a == 1);
		const value_t b{ int(-1) };
		REQUIRE(b == -1);
	};

	SECTION("Construction from an integer division") {
		const value_t a = value_t::IntDiv(21, 6);
		REQUIRE(a == 3.5);
		const value_t b = value_t::IntDiv(1, 2);
		REQUIRE(b == 0.5);
	};
};