#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"


#define MAX_BYTES 16


void listInit(List *list)
{
   list->items = NULL;
   list->count = 0;
   list->capacity = 0;
}

void listAdd(List *list, uint32_t item)
{
   if (list->count >= list->capacity){
      size_t newcap = list->capacity ? list->capacity * 2 : 64;
      list->items = realloc(list->items, newcap * sizeof(uint32_t));
      list->capacity = newcap;
   }
   list->items[list->count++] = item;
}

void instr_init(Instruction* instr)
{
   for(int i = 0; i < MAX_BYTES; i++)
      instr->bytes[i] = 0;
   instr->nbytes = 0;
}

void add_byte(Instruction *instr, uint8_t byte)
{
   instr->bytes[instr->nbytes++] = byte;
}

void list_init(InstructionList *list)
{
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

void list_add(InstructionList *list, Instruction instr)
{
    if (list->count == list->capacity) {
        size_t newcap = list->capacity ? list->capacity * 2 : 64;
        list->items = realloc(list->items, newcap * sizeof(Instruction));
        list->capacity = newcap;
    }
    list->items[list->count++] = instr;
}

void print_list(InstructionList *list, List *keys)
{
   for (int i = 0; i < keys->count; i++){
        printf("%x:  ", keys->items[i]);
        for (int j = 0; j < list->items[i].nbytes; j++)
            printf("%x ", list->items[i].bytes[j]);
        printf("\n");
   }
}

void free_lists(InstructionList* instrs, List* keys)
{
   free(instrs->items);
   free(keys->items);
}


uint32_t hexstr_to_uint32(const char* str)
{
   uint32_t x = 0;
   for (int i = 0; *(str + i) && !isspace(*(str + i)); i++){
      uint8_t c = *(str + i);
      uint32_t v = (c & 0xF) + ((c >> 6) & 1) * 9;
      x = (x << 4) | v;
   }
   return x;
}

void trim(char* str)
{
   char* p = str;

   while(*p && isspace((unsigned char)*p))
      p++;

   if (p != str)
      memmove(str, p, strlen(p) + 1);

   for (size_t len = strlen(str); len > 0 && isspace((unsigned char)str[len - 1]); len--)
      str[len - 1] = '\0';
}

void parse_line(char* buffer, InstructionList* table, List* keys)
{
   uint8_t colon = strcspn(buffer, ":");
   buffer[colon] = '\0';
   char *values = buffer + colon + 1;
   trim(values);
   listAdd(keys, hexstr_to_uint32(buffer));
   Instruction instr;
   instr_init(&instr);
   char* byte = strtok(values, " ");
   while (byte != NULL){
      add_byte(&instr, hexstr_to_uint32(byte));
      byte = strtok(NULL, " ");
   }
   list_add(table, instr);
}

void parse(const char* path, InstructionList* table_out, List* keys_out)
{
   FILE* file = fopen(path, "r");
   
   char buffer[64];

   if (file != NULL){
      list_init(table_out);
      listInit(keys_out);
      for (int i = 0;fgets(buffer, sizeof(buffer), file); i++){
         buffer[strcspn(buffer, "\n")] = '\0';
         parse_line(buffer, table_out, keys_out);
      }
   } else {
      printf("Error: failed reading %s\n", path);
      exit(-2);
   }
}
