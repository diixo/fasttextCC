
#include "dictionary.h"
#include "args.h"
#include <memory>

int main()
{
   auto args = std::make_shared<fasttext::Args>();
   args->input = "train-data.txt";
   fasttext::Dictionary dictionary(args, 557);
   return 0;
}
