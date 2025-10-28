#pragma once

#include <stdint.h>
#include <stdlib.h>

#define MAX_BYTES 16

typedef struct {
    uint8_t bytes[MAX_BYTES];
    uint8_t nbytes;
} Instruction;

typedef struct {
    Instruction* items;
    size_t count;
    size_t capacity;
} InstructionList;

typedef struct {
   uint32_t* items;
   size_t count;
   size_t capacity;
} List;

void parse(const char* path, InstructionList* table_out, List* keys_out);
void print_list(InstructionList* table, List* keys);
void free_lists(InstructionList* table, List* keys);

