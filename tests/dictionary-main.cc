
#include "dictionary.h"
#include "args.h"

#include <memory>
#include <iostream>
#include <fstream>

int main()
{
   auto args = std::make_shared<fasttext::Args>();
   args->input = "train-data.txt";
   fasttext::Dictionary dictionary(args, 557);
   dictionary.readFromFile(std::ifstream(args->input), nullptr);
   return 0;
}
