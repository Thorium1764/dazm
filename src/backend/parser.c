#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void parse_line(char* buffer, uint8_t** table, uint32_t* keys)
{
   static int index = 0;
   uint8_t colon = strcspn(buffer, ":");
   *(buffer + colon) = '\0';
   char* values = buffer + colon + 1;
   trim(values);
   int capacity = 2;
   uint8_t* ptr = malloc(capacity * sizeof(uint8_t));
   if(!ptr) exit(-1);
   char* byte = strtok(values, " ");
   uint16_t count = 0;
   while(byte != NULL){
      if (count >= capacity){
         capacity *= 2;
         uint8_t* tmp = realloc(ptr, capacity * sizeof(uint8_t));
         if (!tmp){
            free(byte);
            exit(-1);
         }
         ptr = tmp;
      }
      ptr[count++] = (uint8_t)(hexstr_to_uint32(byte));
      byte = strtok(NULL, " ");
   }
   table[hexstr_to_uint32(buffer)] = ptr;
   keys[index++] = hexstr_to_uint32(buffer);
}

void parse(const char* path, uint8_t** table, uint32_t* keys)
{
   FILE* file = fopen(path, "r");
   
   char buffer[64];

   if (file != NULL){
      int count = 0;
      int capacity = 1000;
      while (fgets(buffer, sizeof(buffer), file)){
         if(count >= capacity){
            capacity *= 2;
            uint8_t** tmp = realloc(table, capacity * sizeof(uint8_t*));
            if (!tmp){
               free(table);
               for (int i = 0; keys[i]; i++)
                  free(table[keys[i]]);
               exit(-1);
            }
            table = tmp;
         }
         buffer[strcspn(buffer, "\n")] = '\0';
         parse_line(buffer, table, keys);
      }
   } else {
      printf("Error: failed reading %s\n", path);
      exit(-2);
   }
}

int main(int argc, char** argv)
{
   if(argc != 2) {
      printf("Error: expected 1 argument, recieved %u\n", argc);
   } else {
      uint8_t** table = malloc(1000 * sizeof(uint8_t*));
      uint32_t* keys = malloc(1000 * sizeof(uint32_t));
      parse(*(argv + 1), table, keys);
      for (int i = 0; keys[i]; i++)
         free(table[keys[i]]);
      free(table);
   }
}


