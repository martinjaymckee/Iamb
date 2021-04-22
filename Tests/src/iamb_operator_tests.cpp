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
TEST_CASE("Sign access operators", "[fixedpoint]") {
	SECTION("Default (s16.16) fixed-point signs") {
		using value_t = iamb::SignedFixedPoint<>;
		const value_t a{ 1 };
		const value_t b{ -1 };
		const value_t c{ 0 };
		REQUIRE(a.isNegative() == false);
		REQUIRE(a.isPositive() == true);
		REQUIRE(a.isNonnegative() == true);
		REQUIRE(a.isZero() == false);
		REQUIRE(b.isNegative() == true);
		REQUIRE(b.isPositive() == false);
		REQUIRE(b.isNonnegative() == false);
		REQUIRE(b.isZero() == false);
		REQUIRE(c.isNegative() == false);
		REQUIRE(c.isPositive() == false);
		REQUIRE(c.isNonnegative() == true);
		REQUIRE(c.isZero() == true);
	};

	SECTION("Default unsigned (u16.16) fixed-point signs") {
		using value_t = iamb::UnsignedFixedPoint<>;
		const value_t a{ 1 };
		const value_t b{ -1 };
		const value_t c{ 0 };
		REQUIRE(a.isNegative() == false);
		REQUIRE(a.isPositive() == true);
		REQUIRE(a.isNonnegative() == true);
		REQUIRE(a.isZero() == false);
		REQUIRE(b.isNegative() == false);
		REQUIRE(b.isPositive() == true);
		REQUIRE(b.isNonnegative() == true);
		REQUIRE(b.isZero() == false);
		REQUIRE(c.isNegative() == false);
		REQUIRE(c.isPositive() == false);
		REQUIRE(c.isNonnegative() == true);
		REQUIRE(c.isZero() == true);
	};

	SECTION("Arbitrary size (s7.5) fixed-point signs") {
		using value_t = iamb::SignedFixedPoint<7, 5>;
		const value_t a{ 1 };
		const value_t b{ -1 };
		const value_t c{ 0 };
		REQUIRE(a.isNegative() == false);
		REQUIRE(a.isPositive() == true);
		REQUIRE(a.isNonnegative() == true);
		REQUIRE(a.isZero() == false);
		REQUIRE(b.isNegative() == true);
		REQUIRE(b.isPositive() == false);
		REQUIRE(b.isNonnegative() == false);
		REQUIRE(b.isZero() == false);
		REQUIRE(c.isNegative() == false);
		REQUIRE(c.isPositive() == false);
		REQUIRE(c.isNonnegative() == true);
		REQUIRE(c.isZero() == true);
	};

	SECTION("Arbitrary size (s7.5) unsigned fixed-point signs") {
		using value_t = iamb::UnsignedFixedPoint<7, 5>;
		const value_t a{ 1 };
		const value_t b{ -1 };
		const value_t c{ 0 };
		REQUIRE(a.isNegative() == false);
		REQUIRE(a.isPositive() == true);
		REQUIRE(a.isNonnegative() == true);
		REQUIRE(a.isZero() == false);
		REQUIRE(b.isNegative() == false);
		REQUIRE(b.isPositive() == true);
		REQUIRE(b.isNonnegative() == true);
		REQUIRE(b.isZero() == false);
		REQUIRE(c.isNegative() == false);
		REQUIRE(c.isPositive() == false);
		REQUIRE(c.isNonnegative() == true);
		REQUIRE(c.isZero() == true);
	};
};

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

TEST_CASE("Overflow Handling - Arbitrary Size", "[fixedpoint]") {
	using s2_2_wrapping = iamb::SignedFixedPoint<2, 2>;					// Range [-2, 1.75]
	using s2_2_saturating = iamb::SignedSaturatingFixedPoint<2, 2>;		// Range [-2, 1.75]
	using u2_2_wrapping = iamb::UnsignedFixedPoint<2, 2>;				// Range [0, 3.75]
	using u2_2_saturating = iamb::UnsignedSaturatingFixedPoint<2, 2>;	// Range [0, 3.75]

	// TODO: IMPLEMENT OVERFLOW HANDLING FOR NEGATION, MULTIPLICATION, AND DIVISION

	SECTION("Addition Signed") {
		// Wrapping
		const s2_2_wrapping a1{ 1.75 };
		const s2_2_wrapping b1{ 0.25 };
		REQUIRE((a1 + b1) == s2_2_wrapping::Storage(0b1000));

		// Signed addition of two positive numbers (positive overflow)
		const s2_2_saturating a2{ 1.75 };
		const s2_2_saturating b2{ 0.25 };
		REQUIRE((a2 + b2) == s2_2_saturating::Storage(0b0111));
		// Signed addition of positive and negative numbers (overflow impossible)
		const s2_2_saturating a3{ 1.75 };
		const s2_2_saturating b3{ -2 };
		REQUIRE((a3 + b3) == -0.25);
		const s2_2_saturating a4{ -2 };
		const s2_2_saturating b4{ 1.75 };
		REQUIRE((a4 + b4) == -0.25);
		// Signed addition of two negative numbers (negative overflow)
		const s2_2_saturating a5{ -1.75 };
		const s2_2_saturating b5{ -0.5 };
		REQUIRE((a5 + b5) == s2_2_saturating::Storage(0b1000));
	};

	SECTION("Addition Unsigned") {
		const u2_2_wrapping a1{ 3.75 };
		const u2_2_wrapping b1{ 0.25 };
		REQUIRE((a1 + b1) == u2_2_wrapping::Storage(0b0000));
		const u2_2_saturating a2{ 3.75 };
		const u2_2_saturating b2{ 0.25 };
		REQUIRE((a2 + b2) == u2_2_saturating::Storage(0b1111));
	};

	SECTION("Signed Subtraction") {
		const s2_2_wrapping a1{ -0.5 };
		const s2_2_wrapping b1{ 1.75 };
		REQUIRE((a1 - b1) == s2_2_wrapping::Storage(0b0111));
		const s2_2_saturating a2{ -0.5 };
		const s2_2_saturating b2{ 1.75 };
		REQUIRE((a2 - b2) == s2_2_saturating::Storage(0b0111));
	};

	SECTION("Unigned Subtraction") {
		const u2_2_wrapping a1{ 1 };
		const u2_2_wrapping b1{ 2 };
		REQUIRE((a1 - b1) == u2_2_wrapping::Storage(0b1100));
		const u2_2_saturating a2{ 1 };
		const u2_2_saturating b2{ 2 };
		REQUIRE((a2 - b2) == 0);
	};
};

TEST_CASE( "Fixed-point operators", "[fixedpoint]" ) {
	using value_t = iamb::SignedFixedPoint<7, 5>; // This is a s7.5 fixed-point type
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
