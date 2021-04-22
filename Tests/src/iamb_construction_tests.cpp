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
// Type Construction
//
TEST_CASE("Construction of correctly sized fixed-point numbers", "[fixedpoint]") {
	SECTION("Default Signed Fixed-Point") {
		using fp_t = iamb::SignedFixedPoint<>;
		REQUIRE(fp_t::wholeBits == 16);
		REQUIRE(fp_t::fractionalBits == 16);
		REQUIRE(std::is_same<typename fp_t::storage_t, int32_t>::value);
		REQUIRE(std::is_same<typename fp_t::calc_t, int64_t>::value);
	};

	SECTION("Arbitrary Signed Fixed-Point (s5.7)") {
		using fp_t = iamb::SignedFixedPoint<5, 7>;
		REQUIRE(fp_t::wholeBits == 5);
		REQUIRE(fp_t::fractionalBits == 7);
		REQUIRE(std::is_same<typename fp_t::storage_t, int16_t>::value);
		REQUIRE(std::is_same<typename fp_t::calc_t, int32_t>::value);
	};

	SECTION("Arbitrary Unsigned Fixed-Point (u3.13)") {
		using fp_t = iamb::UnsignedFixedPoint<3, 13>;
		REQUIRE(fp_t::wholeBits == 3);
		REQUIRE(fp_t::fractionalBits == 13);
		REQUIRE(std::is_same<typename fp_t::storage_t, uint16_t>::value);
		REQUIRE(std::is_same<typename fp_t::calc_t, uint32_t>::value);
	};

};

//
// Fixed-Point Construction
//
TEST_CASE( "Fixed-point numbers of default type are constructed", "[fixedpoint]" ) {
	using value_t = iamb::SignedFixedPoint<>; // This is a default s16.16 fixed-point type

	SECTION("Direct Storage Construction") {
		const value_t a = value_t::Storage(1<<16);
		REQUIRE(a == 1);
		const value_t b = value_t::Storage(1<<15);
		REQUIRE(b == 0.5);
		const value_t c = value_t::Storage(0xFFFF0000);
		REQUIRE(c == -1);
		const value_t d = value_t::Storage(0xFFFF8000);
		REQUIRE(d == -0.5);
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
		const value_t d{float(-0.5)};
		REQUIRE(d == -0.5);
	};

	SECTION("Construction from Double") {
		const value_t a{double(1.0)};
		REQUIRE(a == 1);
		const value_t b{double(0.5)};
		REQUIRE(b == 0.5);
		const value_t c{double(-1.0)};
		REQUIRE(c == -1);
		const value_t d{float(-0.5)};
		REQUIRE(d == -0.5);
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
		const value_t c = value_t::IntDiv(3, -6);
		REQUIRE(c == -0.5);
	};
};

TEST_CASE("Fixed-point numbers with s2.2 format are constructed", "[fixedpoint]") {
	using value_t = iamb::SignedFixedPoint<2, 2>; // This is an s2.2 fixed-point type

	SECTION("Direct Storage Construction") {
		const value_t a = value_t::Storage(1 << 2);
		REQUIRE(a == 1);
		const value_t b = value_t::Storage(1 << 1);
		REQUIRE(b == 0.5);
		const value_t c = value_t::Storage(0b00001100);
		REQUIRE(c == -1);
		const value_t d = value_t::Storage(0b00001110);
		REQUIRE(d == -0.5);
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
		const value_t d{ float(-0.5) };
		REQUIRE(d == -0.5);
	};

	SECTION("Construction from Double") {
		const value_t a{ double(1.0) };
		REQUIRE(a == 1);
		const value_t b{ double(0.5) };
		REQUIRE(b == 0.5);
		const value_t c{ double(-1.0) };
		REQUIRE(c == -1);
		const value_t d{ float(-0.5) };
		REQUIRE(d == -0.5);
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
		const value_t c = value_t::IntDiv(-3, 6);
		REQUIRE(c == -0.5);
	};
};
