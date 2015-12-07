//
//  StringCleaningTest.cpp
//
//  Created by Soso Limited on 12/7/15.
//
//

#include "catch.hpp"
#include "StringManipulation.h"

using namespace std;

TEST_CASE("StringManipulation")
{
	SECTION("Leading and trailing brackets without an opposite match are removed")
	{
		auto insides = string("whatever");
		auto test_prefix = [insides] (const string &prefix) {
			auto leading_only = prefix + insides;
			REQUIRE(soso::trim_unmatched_brackets(leading_only) == insides);
		};

		auto test_suffix = [insides] (const string &suffix) {
			auto trailing_only = insides + suffix;
			REQUIRE(soso::trim_unmatched_brackets(trailing_only) == insides);
		};

		test_prefix("[");
		test_prefix("{");
		test_prefix("(");

		test_suffix("]");
		test_suffix("}");
		test_suffix(")");
	}

	SECTION("Matched brackets are kept")
	{
		auto insides = string("whatever");
		auto test_matched = [insides] (const string &prefix, const string &suffix) {
			auto wrapped = prefix + insides + suffix;
			REQUIRE(soso::trim_unmatched_brackets(wrapped) == wrapped);
		};

		test_matched("[", "]");
		test_matched("{", "}");
		test_matched("(", ")");
	}

	SECTION("Mismatched brackets are removed")
	{
		auto insides = string("goodies");
		auto miswrapped = "{" + insides + "]";
		REQUIRE(soso::trim_unmatched_brackets(miswrapped) == insides);
	}

	SECTION("Leading and trailing (western) punctuation is completely removed by strip_punctuation")
	{
		auto insides = string("stuff");
		auto dots = "." + insides + "...";
		REQUIRE(soso::strip_punctuation(dots) == insides);

		auto single_char = string("a");
		dots = "." + single_char + ".";
		REQUIRE(soso::strip_punctuation(dots) == single_char);
	}

	SECTION("Punctuation-only lines become empty strings")
	{
		auto punctuation = "..,\'.";
		REQUIRE(soso::strip_punctuation(punctuation) == "");
	}
}
