
#include "dictionary.h"
#include "strutils.h"
#include "args.h"

#include <memory>
#include <iostream>
#include <fstream>

#include <codecvt>
#include <locale>
#include <cassert>

#include "fasttext.h"
#include "utils.h"

using namespace fasttext;

const int VOCAB_SZ = 577;

void test()
{
   auto ft = std::make_shared<fasttext::FastText>();
   auto args = std::make_shared<fasttext::Args>();
   args->minCount = 1;
   args->maxn = 0;
   args->input = "train.txt";
   args->epoch = 200;
   args->lrUpdateRate = args->epoch / 10;
   args->dim = 10;
   args->loss = fasttext::loss_name::softmax;
   args->model = fasttext::model_name::cbow;
   args->thread = 1;
   args->vocabSz = 97;
   args->ws = 1;
   ft->train(*args);

   real similarity = ft->getSimilarity("c++", "python");
   similarity = 0.f;
}

void test_nn()
{
   auto ft = std::make_shared<fasttext::FastText>();
   auto args = std::make_shared<fasttext::Args>();
   args->minCount = 1;
   args->maxn = 0;
   args->input = "train-nn-w.txt";
   args->stopwords = "stopwords.txt";
   args->epoch = 200;
   args->lrUpdateRate = args->epoch / 10;
   args->dim = 10;
   args->loss = fasttext::loss_name::softmax;
   args->model = fasttext::model_name::cbow;
   args->thread = 1;
   args->vocabSz = 1511;
   args->ws = 2;
   ft->train(*args);

   // ctrl+z = EOF marker to skip CIN.
   std::wistream& in = std::wcin;
   std::vector<std::pair<real, std::string>> predictions;


   //while (in.rdbuf()->sungetc() != std::char_traits<char>::eof()
   //   && in.get() != in.widen('\n'))
   //{
   //   ft->predictNext(in, predictions, 0.f);
   //   continue;
   //}

   real threshold = 0.f;
   bool printProb = true;
  
   while (in.good())
   {
      if (!ft->predictNext(in, predictions, threshold))
      {
         break;
      }
      printPredictions(predictions, printProb, false);
   }
}


int main()
{
   test_nn();
   return 0;

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
