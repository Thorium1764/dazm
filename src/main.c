#include "lexer.h"
#include <stdio.h>

int main(int argc, char** argv)
{
   if(argc != 2) {
      printf("Error: expected 1 argument, recieved %u\n", argc - 1);
   } else {
      InstructionList table;
      List keys;
      parse(argv[1], &table, &keys);
      // do something
      print_list(&table, &keys); //temp
      free_lists(&table, &keys);
   }
}


