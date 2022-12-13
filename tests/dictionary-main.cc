
#include "dictionary.h"
#include "strutils.h"
#include "args.h"

#include <memory>
#include <iostream>
#include <fstream>

#include <codecvt>
#include <locale>
#include <cassert>

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
   printf("<<-------------\n");
   {
      std::wifstream wsw(L"train-data.txt");
      std::vector<int32_t> line;
      wsw.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));

      int32_t tokens = 0;
      int32_t ntokens = 0;
      while ((tokens = dictionary.getLine(wsw, line, stopwords)) > 0)
      {
         ntokens += tokens;
         if (!line.empty())
         {
            for (auto id : line)
            {
               printf("%s ", dictionary.getWord(id).c_str());
            }
            printf("<< %d\n", line.size());
         }
      }
      printf("<<-------------%d", ntokens);
      assert(ntokens == dictionary.ntokens());
   }

   return 0;
}
