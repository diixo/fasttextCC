
#pragma once

#include <string>

//////////////////////////////////////////////////////////////////////////

std::wstring convert_utf8_to_utf16(const std::string& utf8);

std::wstring cstr_to_wstr(const std::string& cstr);

//////////////////////////////////////////////////////////////////////////

bool isCharModificator(const wchar_t ch);

bool isCharDiacritic(const wchar_t ch);

bool isCharOutdated(const wchar_t ch);

bool isCharApostrophe(const wchar_t ch);

//////////////////////////////////////////////////////////////////////////

std::string& ltrim(const std::string& delim, std::string& io);

std::string& rtrim(const std::string& delim, std::string& io);

std::string& trim(const std::string& delim, std::string& io);

std::wstring& ltrim(const std::wstring& delim, std::wstring& io);

std::wstring& rtrim(const std::wstring& delim, std::wstring& io);

std::wstring& trim(const std::wstring& delim, std::wstring& io);

//////////////////////////////////////////////////////////////////////////

bool is_digit(const std::wstring& input, size_t start_id = 0);

bool is_anydigit(const std::wstring& input, size_t start_id = 0);

wchar_t translateChar(const wchar_t ch);

wchar_t transformChar(const wchar_t ch);

std::string translate_wstr(const std::wstring& wstr);

std::string transform_wstr(const std::wstring& wstr);

//////////////////////////////////////////////////////////////////////////
