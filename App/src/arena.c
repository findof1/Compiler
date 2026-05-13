#include "arena.h"

size_t alignSize(size_t size)
{
  return (size + _Alignof(max_align_t) - 1) & ~(_Alignof(max_align_t) - 1);
}

bool initArena(Arena *arena, size_t capacity)
{
  size_t alignedCapacity = alignSize(capacity);
  arena->memory = (char *)malloc(alignedCapacity);
  if (!arena->memory)
  {
    return false;
  }
  arena->capacity = alignedCapacity;
  arena->offset = 0;

  return true;
}

void *arenaAlloc(Arena *arena, size_t size)
{
  size_t alignedSize = alignSize(size);

  if (arena->offset + alignedSize > arena->capacity)
  {
    printf("Arena Allocation Error: Attempting to allocate beyond arena capacity");
    return NULL;
  }
  void *ptr = arena->memory + arena->offset;
  arena->offset += alignedSize;
  return ptr;
}

void arenaFree(Arena *arena)
{
  if (arena->memory)
  {
    free(arena->memory);
    arena->memory = NULL;
  }
  arena->capacity = 0;
  arena->offset = 0;
}