//
//  CharacterEncoding_test.cpp
//
//  Created by Soso Limited on 9/28/15.
//
//

#include "catch.hpp"
#include "ClosedCaptionConversions.hpp"

TEST_CASE("Character Encoding")
{
	SECTION("If a character is outside the known sequence, we return a space.")
	{
		auto c = closed_caption_to_string(254);
		REQUIRE(c == u8" ");
	}
}
