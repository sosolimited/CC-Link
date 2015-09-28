//
//  StringManipulation.h
//
//  Created by Soso Limited on 9/8/15.
//
//

#pragma once
#include <string>

namespace soso {

inline std::string strip_punctuation(const std::string &str)
{
	using std::string;
	static string punctuation = R"(,.;:?/'")"; // raw string (syntax highlighting doesn't work, though)
	auto begin = (punctuation.find(str.front()) == string::npos) ? 0 : 1;
	auto end = (punctuation.find(str.back()) == string::npos) ? str.size() : str.size() - 1;
	return str.substr(begin, end);
}

} // namespace soso
