
#include "dictionary.h"
#include "args.h"

#include <memory>
#include <iostream>
#include <fstream>

int main()
{
   const int VOCAB_SZ = 547;

   auto args = std::make_shared<fasttext::Args>();
   args->minCount = 1;

   auto stopwords = std::make_shared<fasttext::Dictionary>(args, VOCAB_SZ);
   stopwords->readFromFile(std::ifstream("stopwords.txt"), nullptr);

   stopwords->dump(std::cout);

   fasttext::Dictionary dictionary(args, VOCAB_SZ);
   dictionary.readFromFile(std::ifstream("train-data.txt"), stopwords);

   dictionary.dump(std::cout);

   return 0;
}
