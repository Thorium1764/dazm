#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint32_t hexstr_to_uint32(const char* str)
{
   uint32_t x = 0;
   for (int i = 0; *(str + i); i++){
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

void parse_line(char* buffer) // , uint8_t** table)
{
   uint8_t colon = strcspn(buffer, ":");
   *(buffer + colon) = '\0';
   char* values = buffer + colon + 1;
   trim(values);
   printf("key=%x, values=%s\n", hexstr_to_uint32(buffer), values);
}

void parse(const char* path)//, uint8_t** table)
{
   FILE* file = fopen(path, "r");
   
   char buffer[64];

   if (file != NULL){
      while (fgets(buffer, sizeof(buffer), file)){
         buffer[strcspn(buffer, "\n")] = '\0';
         parse_line(buffer);//, table);
      }
   } else {
      printf("Error: failed reading %s\n", path);
   }
}

int main()
{
   parse("/home/tobias/code/dazm/src/backend/dasm.out");
}


