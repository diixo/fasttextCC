
#pragma once

#include <string>

//////////////////////////////////////////////////////////////////////////

std::wstring utf8_to_utf16(const std::string& utf8);

std::wstring cstr_to_wstr(const char* c_str);

//////////////////////////////////////////////////////////////////////////

bool isCharModificator(const wchar_t ch);

bool isCharDiacritic(const wchar_t ch);

bool isCharOutdated(const wchar_t ch);

bool isCharApostrophe(const wchar_t ch);

//////////////////////////////////////////////////////////////////////////

std::wstring& ltrim(std::wstring& inStr, const std::wstring& Delim);

std::wstring& rtrim(std::wstring& inStr, const std::wstring& Delim);

std::wstring& trim(std::wstring& inStr, const std::wstring& Delim);

std::wstring& trim(std::wstring& inStr, const std::wstring& Delim);

//////////////////////////////////////////////////////////////////////////

bool is_digit(const std::wstring& inStr, size_t start_id = 0);

bool is_anydigit(const std::wstring& inStr, size_t start_id = 0);

wchar_t translateChar(const wchar_t ch);

std::string translate_str(const std::wstring& wstr);

//////////////////////////////////////////////////////////////////////////
