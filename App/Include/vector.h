#pragma once
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct
{
  void *v;
  size_t itemBytes;   // memory size of each item
  size_t currentSize; // num of items in vector
  size_t capacity;    // num of items allocated in memory
} Vector;

bool init(Vector *vec, size_t elementSize, size_t expectedElements);

bool resize(Vector *vec, size_t newCapacity);

bool push(Vector *vec, void *item);

bool pushInt(Vector *vec, int item);

void *getItem(const Vector *vec, int index);