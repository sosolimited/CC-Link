//
//  ControlCode_test.cpp
//
//  Created by Soso Limited on 9/28/15.
//
//

#include "catch.hpp"

#include "StringManipulation.h"
#include <iostream>
#include <fstream>

using namespace std;

TEST_CASE( "Strip control codes from incoming text." )
{
	// load the file
	auto captioning = ifstream("../../../assets/closed_captioning.txt");

	for( auto i = 0; i < 256; i += 1 )
	{
		char c;
		captioning >> c;
		cout << c;
	}

}
