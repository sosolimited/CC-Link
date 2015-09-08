//
//  main.cpp
//  CCLinkUnitTest
//
//  Created by Soso Limited on 9/8/15.
//
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "StringManipulation.h"

#include <iostream>

using namespace std;

TEST_CASE( "Strip control codes from incoming text." )
{
	// load the file
	ifstream captioning("../../../assets/closed_captioning.txt");

	for( auto i = 0; i < 10; i += 1 )
	{
		char c;
		captioning >> c;
		cout << c;
	}


	REQUIRE( 1 == 2 );

}