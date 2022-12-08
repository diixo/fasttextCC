
#include "dictionary.h"
#include "strutils.h"
#include "args.h"

#include <memory>
#include <iostream>
#include <fstream>

#include <codecvt>
#include <locale>

const int VOCAB_SZ = 547;

int main()
{
   auto args = std::make_shared<fasttext::Args>();
   args->minCount = 1;
   args->stopwords = "stopwords.txt";

   auto stopwords = std::make_shared<fasttext::Dictionary>(args, VOCAB_SZ);
   {
      std::wifstream wsw(cstr_to_wstr(args->stopwords));
      wsw.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
      stopwords->readFromFile(wsw, nullptr);
      wsw.close();
   }


   fasttext::Dictionary dictionary(args, VOCAB_SZ);
   {
      std::wifstream wsw(L"train-data.txt");
      wsw.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
      dictionary.readFromFile(wsw, stopwords);
      wsw.close();

      dictionary.dump(std::cout);
   }

   return 0;
}
