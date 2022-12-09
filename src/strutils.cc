
#include "strutils.h"
#include <vector>

const wchar_t apostrophe = 0x0027;

std::wstring convert_utf8_to_utf16(const std::string& utf8)
{
   std::vector<unsigned long> unicode;
   size_t i = 0;
   while (i < utf8.size())
   {
      unsigned long uni;
      size_t todo;
      bool error = false;
      unsigned char ch = utf8[i++];
      if (ch <= 0x7F)
      {
         uni = ch;
         todo = 0;
      }
      else if (ch <= 0xBF)
      {
         throw std::logic_error("not a UTF-8 string");
      }
      else if (ch <= 0xDF)
      {
         uni = ch & 0x1F;
         todo = 1;
      }
      else if (ch <= 0xEF)
      {
         uni = ch & 0x0F;
         todo = 2;
      }
      else if (ch <= 0xF7)
      {
         uni = ch & 0x07;
         todo = 3;
      }
      else
      {
         throw std::logic_error("not a UTF-8 string");
      }
      for (size_t j = 0; j < todo; ++j)
      {
         if (i == utf8.size())
            throw std::logic_error("not a UTF-8 string");
         unsigned char ch = utf8[i++];
         if (ch < 0x80 || ch > 0xBF)
            throw std::logic_error("not a UTF-8 string");
         uni <<= 6;
         uni += ch & 0x3F;
      }
      if (uni >= 0xD800 && uni <= 0xDFFF)
         throw std::logic_error("not a UTF-8 string");
      if (uni > 0x10FFFF)
         throw std::logic_error("not a UTF-8 string");
      unicode.push_back(uni);
   }
   std::wstring utf16;
   for (size_t i = 0; i < unicode.size(); ++i)
   {
      unsigned long uni = unicode[i];
      if (uni <= 0xFFFF)
      {
         utf16 += (wchar_t)uni;
      }
      else
      {
         uni -= 0x10000;
         utf16 += (wchar_t)((uni >> 10) + 0xD800);
         utf16 += (wchar_t)((uni & 0x3FF) + 0xDC00);
      }
   }
   return utf16;
}


std::wstring cstr_to_wstr(const std::string& cstr)
{
   return std::wstring(cstr.begin(), cstr.end());
}

bool isCharModificator(const wchar_t ch)
{
   // general group: 02B0—02FF
   return ((ch >= 0x02b0) && (ch <= 0x02ff));
}

bool isCharDiacritic(const wchar_t ch)
{
   // general group: 0300—036F
   const wchar_t diacr[] = // [22]
   {
      0x0301,
      0x0300,
      0x0308,
      0x0302,
      0x0311,
      0x030c,
      0x030b,
      0x030f,
      0x030a,
      0x0307,
      0x0303,
      0x0342,
      0x0304,
      0x0306,

      0x0326,

      0x032f,
      0x0331,
      0x032c,

      0x0327,
      0x0328,
      0x0337,
      0x0338
   };
   return ((ch >= 0x0300) && (ch <= 0x036F));
}

bool isCharOutdated(const wchar_t ch)
{
   return ((ch >= 0x0370) && (ch <= 0x03FF));
}

bool isCharApostrophe(const wchar_t ch)
{
   if (
      (ch == 0x0027) ||  // 39
      (ch == 0x0060) ||  // 96
      (ch == 0x0091) ||  // 145
      (ch == 0x00b4) ||  // 180 
      (ch == 0x2019) ||  // 8217 

      (ch == 0x02b9) ||  // 697
      (ch == 0x02bb) ||  // 699
      (ch == 0x02bc) ||  // 700
      (ch == 0x02bd)     // 701 
      )
   {
      return true;
   }
   return false;
}

//////////////////////////////////////////////////////////////////////////

std::string& ltrim(const std::string& delim, std::string& io)
{
   io.erase(0, io.find_first_not_of(delim));
   return io;
}

std::string& rtrim(const std::string& delim, std::string& io)
{
   io.erase(io.find_last_not_of(delim) + 1);
   return io;
}

std::string& trim(const std::string& delim, std::string& io)
{
   return ltrim(delim, rtrim(delim, io));
}

//////////////////////////////////////////////////////////////////////////

std::wstring& ltrim(const std::wstring& delim, std::wstring& io)
{
   io.erase(0, io.find_first_not_of(delim));
   return io;
}

std::wstring& rtrim(const std::wstring& delim, std::wstring& io)
{
   io.erase(io.find_last_not_of(delim) + 1);
   return io;
}

std::wstring& trim(const std::wstring& delim, std::wstring& io)
{
   return ltrim(delim, rtrim(delim, io));
}

//////////////////////////////////////////////////////////////////////////

bool is_digit(const std::wstring& input, size_t start_id)
{
   bool result = (start_id < input.size());
   while (start_id < input.size())
   {
      if (!iswdigit(input[start_id]))
      {
         result = false;
         break;
      }
      start_id++;
   }
   return result;
}

bool is_anydigit(const std::wstring& input, size_t start_id)
{
   bool result = false;
   while (start_id < input.size())
   {
      if (iswdigit(input[start_id]))
      {
         result = true;
         break;
      }
      start_id++;
   }
   return result;
}


wchar_t translateChar(const wchar_t ch)
{
   const wchar_t space = 0x0020;
   const wchar_t hyphen = 0x002d;

   if (ch < space)
   {
      return 0;
   }

   if ((ch >= 0x0041) && (ch <= 0x005a)) // check latin symbols
   {
      return ch;
   }

   if (ch == 0x00ad)
   {
      return hyphen;
   }

   if (isCharApostrophe(ch))
   {
      return apostrophe;
   }

   // replace hieroglyph symbols, also: (0x2028, 0x2029)
   if (ch >= 1280) // 0x0500
   {
      if ((ch >= 0x1e00) && (ch <= 0x1eff))
      {
         return ch;
      }
      if ((ch >= 8220) && (ch <= 8223))
      {
         return 0x0022;
      }
      if (ch == 0xfeff) // check if UTF8-BOM is first symbol
      {
         return 0;
      }
      return space;
   }
   return ch;
}

wchar_t transformChar(const wchar_t ch)
{
   const wchar_t space = 0x0020;
   const wchar_t hyphen = 0x002d;

   if (ch < space)
   {
      return 0;
   }

   if ((ch >= 0x0041) && (ch <= 0x005a)) // check latin symbols
   {
      return towlower(ch);
   }

   if (ch == 0x00ad)
   {
      return hyphen;
   }

   if (isCharApostrophe(ch))
   {
      return apostrophe;
   }

   // replace hieroglyph symbols, also: (0x2028, 0x2029)
   if (ch >= 1280) // 0x0500
   {
      if ((ch >= 0x1e00) && (ch <= 0x1eff))
      {
         return ch;
      }
      if ((ch >= 8220) && (ch <= 8223))
      {
         return 0x0022;
      }
      if (ch == 0xfeff) // check if UTF8-BOM is first symbol
      {
         return 0;
      }
      return space;
   }
   return ch;
}

std::string translate_wstr(const std::wstring& wstr)
{
   std::string sstr;
   for (auto it: wstr)
   {
      const char ch = static_cast<char>(translateChar(it));
      if (ch) sstr.push_back(ch);
   }
   return sstr;
}

std::string transform_wstr(const std::wstring& wstr)
{
   std::string sstr;
   for (auto it : wstr)
   {
      const char ch = static_cast<char>(transformChar(it));
      if (ch) sstr.push_back(ch);
   }
   return sstr;
}

