#pragma once
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stddef.h>

typedef struct
{
  char *memory;
  size_t capacity;
  size_t offset;
} Arena;

size_t alignSize(size_t size);

bool initArena(Arena *arena, size_t capacity);

void *arenaAlloc(Arena *arena, size_t size);

void arenaFree(Arena *arena);