//
//  StringCleaningTest.cpp
//
//  Created by Soso Limited on 12/7/15.
//
//

#include "catch.hpp"
#include "StringManipulation.h"

using namespace std;

TEST_CASE("StringCleaningTest")
{
	SECTION("trim_unmatched_brackets removes leading or trailing brackets as needed.")
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

	SECTION("trim_unmatched_brackets leaves in brackets that are matched")
	{
		auto insides = string("whatever");
		auto test_matched = [insides] (const string &prefix, const string &suffix) {
			auto wrapped = prefix + insides + suffix;
			REQUIRE(soso::trim_unmatched_brackets(wrapped) == wrapped);
		};

		test_matched("[", "]");
		test_matched("{", "}");
		test_matched("(", ")");

		auto miswrapped = "{" + insides + "]";
		REQUIRE(soso::trim_unmatched_brackets(miswrapped) == insides);
	}

	SECTION("strip_punctuation removes leading and trailing (western) punctuation")
	{
		auto insides = string("stuff");
		auto dots = "." + insides + ".";
		REQUIRE(soso::strip_punctuation(dots) == insides);
	}
}
