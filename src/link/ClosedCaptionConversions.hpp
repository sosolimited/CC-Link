#pragma once

#include <unordered_map>

const std::vector<char> double_byte_headers = { { 0x11, 0x19, 0x14, 0x17, 0x1C, 0x1F } };
const std::vector<char> special_char_headers = { { 0x11, 0x19 } };

///
/// Returns a UTF-8 formatted string from an EIA-608 character.
/// EIA 608 encoding from https://en.wikipedia.org/wiki/EIA-608
/// usage: EIA_608_to_string(char);
///
const auto closed_caption_to_string = ([]() {
    auto mapping = std::unordered_map<char, std::string>{
        { 0x20, u8" " }, // "(SP)"
        { 0x21, u8"!" },
        { 0x22, u8"\"" },
        { 0x23, u8"#" },
        { 0x24, u8"$" },
        { 0x25, u8"%" },
        { 0x26, u8"&" },
        { 0x27, u8"’" },
        { 0x28, u8"(" },
        { 0x29, u8")" },
        { 0x2A, u8"á" },
        { 0x2B, u8"+" },
        { 0x2C, u8"," },
        { 0x2D, u8"-" },
        { 0x2E, u8"." },
        { 0x2F, u8"/" },
        { 0x30, u8"0" },
        { 0x31, u8"1" },
        { 0x32, u8"2" },
        { 0x33, u8"3" },
        { 0x34, u8"4" },
        { 0x35, u8"5" },
        { 0x36, u8"6" },
        { 0x37, u8"7" },
        { 0x38, u8"8" },
        { 0x39, u8"9" },
        { 0x3A, u8":" },
        { 0x3B, u8";" },
        { 0x3C, u8"<" },
        { 0x3D, u8"=" },
        { 0x3E, u8">" },
        { 0x3F, u8"?" },
        { 0x40, u8"@" },
        { 0x41, u8"A" },
        { 0x42, u8"B" },
        { 0x43, u8"C" },
        { 0x44, u8"D" },
        { 0x45, u8"E" },
        { 0x46, u8"F" },
        { 0x47, u8"G" },
        { 0x48, u8"H" },
        { 0x49, u8"I" },
        { 0x4A, u8"J" },
        { 0x4B, u8"K" },
        { 0x4C, u8"L" },
        { 0x4D, u8"M" },
        { 0x4E, u8"N" },
        { 0x4F, u8"O" },
        { 0x50, u8"P" },
        { 0x51, u8"Q" },
        { 0x52, u8"R" },
        { 0x53, u8"S" },
        { 0x54, u8"T" },
        { 0x55, u8"U" },
        { 0x56, u8"V" },
        { 0x57, u8"W" },
        { 0x58, u8"X" },
        { 0x59, u8"Y" },
        { 0x5A, u8"Z" },
        { 0x5B, u8"[" },
        { 0x5C, u8"é" },
        { 0x5D, u8"]" },
        { 0x5E, u8"í" },
        { 0x5F, u8"ó" },
        { 0x60, u8"ú" },
        { 0x61, u8"a" },
        { 0x62, u8"b" },
        { 0x63, u8"c" },
        { 0x64, u8"d" },
        { 0x65, u8"e" },
        { 0x66, u8"f" },
        { 0x67, u8"g" },
        { 0x68, u8"h" },
        { 0x69, u8"i" },
        { 0x6A, u8"j" },
        { 0x6B, u8"k" },
        { 0x6C, u8"l" },
        { 0x6D, u8"m" },
        { 0x6E, u8"n" },
        { 0x6F, u8"o" },
        { 0x70, u8"p" },
        { 0x71, u8"q" },
        { 0x72, u8"r" },
        { 0x73, u8"s" },
        { 0x74, u8"t" },
        { 0x75, u8"u" },
        { 0x76, u8"v" },
        { 0x77, u8"w" },
        { 0x78, u8"x" },
        { 0x79, u8"y" },
        { 0x7A, u8"z" },
        { 0x7B, u8"ç" },
        { 0x7C, u8"÷" },
        { 0x7D, u8"Ñ" },
        { 0x7E, u8"ñ" },
        { 0x7F, u8"\b" } // "SB"
    };

    return [mapping](char cc_character) -> std::string {
        if (mapping.count(cc_character)) {
            return mapping.at(cc_character);
        }
        else {
            return u8"";
        }
    };

}());

const auto special_closed_caption_to_string = ([]() {
    auto mapping = std::unordered_map<char, std::string>{
        { 0x30, u8"®" }, // Registered Mark Symbol
        { 0x31, u8"°" }, // Degree sign
        { 0x32, u8"½" }, // 1/2 mark
        { 0x33, u8"¿" }, // Inverse question mark
        { 0x34, u8"™" }, // Trademark symbol
        { 0x35, u8"¢" }, // Cents sign
        { 0x36, u8"£" }, // Pounds Sterling symbol
        { 0x37, u8"♪" }, // Music note
        { 0x38, u8"à" }, // Lower case a with accent grave
        { 0x39, u8")" }, // Transparent space
        { 0x3A, u8"è" }, // Lower case e with accent grave
        { 0x3B, u8"â" }, // Lower case a with circumflex
        { 0x3C, u8"ê" }, // Lower case e with circumflex
        { 0x3D, u8"î" }, // Lower case i with circumflex
        { 0x3E, u8"ô" }, // Lower case o with circumflex
        { 0x3F, u8"û" } // Lower case u with circumflex
    };

    return [mapping](char cc_character) -> std::string {
        if (mapping.count(cc_character)) {
            return mapping.at(cc_character);
        }
        else {
            return u8"";
        }
    };

}());

const auto control_code_to_string = ([]() {
    auto mapping = std::unordered_map<char, std::string>{
        { 0x20, u8" " }, // Resume Caption Loading
        { 0x21, u8" " }, // Backspace
        { 0x22, u8" " }, // Tab
        { 0x23, u8" " }, // Tab
				{ 0x24, u8" " }, // Delete to End of Row
				{ 0x25, u8" " }, // Roll-Up Captions–2 Rows
				{ 0x26, u8" " }, // Roll-Up Captions–3 Rows
				{ 0x27, u8" " }, // Roll-Up Captions–4 Rows
				{ 0x28, u8" " }, // Flash On
				{ 0x29, u8" " }, // Resume Direct Captioning
				{ 0x2A, u8" " }, // Text Restart
				{ 0x2B, u8" " }, // Resume Text Display
				{ 0x2C, u8" " }, // Erase Displayed Memory
				{ 0x2D, u8" " }  // Carriage Return
    };

    return [mapping](char cc_character) -> std::string {
        if (mapping.count(cc_character)) {
            return mapping.at(cc_character);
        }
        else {
            return u8"";
        }
    };

}());

/*
bool is_eia_char(char c)
{
	if (c >= 0x20 && c <= 0x7F) {
		return true;
	}
	return false;
}
*/