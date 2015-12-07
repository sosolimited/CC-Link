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
	static string punctuation = R"p(-,.;:?!/'")p";
	auto begin = (punctuation.find(str.front()) == string::npos) ? 0 : 1;
	auto end = (punctuation.find(str.back()) == string::npos) ? str.size() : str.size() - 1;
	return str.substr(begin, end - begin);
}

inline std::string trim_unmatched_brackets(const std::string &str)
{
	using std::string;
	static string opening = "[{(";
	static string closing = "]})";
	const auto &first = str.front();
	const auto &last = str.back();
	auto begin = 0;
	auto end = str.size();

	auto first_match = opening.find(first);
	if (first_match != string::npos) {
		auto last_match = closing.find(last);
		if (last_match != first_match) {
			// unmatched
			begin = 1;
			if (last_match != string::npos) {
				// and end was a different bracket
				end -= 1;
			}
		}
	}
	else if (closing.find(last) != string::npos) {
		// only end was a bracket
		end -= 1;
	}

	if (end - begin <= 0) {
		return str;
	}

	return str.substr(begin, end - begin);
}

} // namespace soso
