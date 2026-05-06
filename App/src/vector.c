#include "vector.h"

bool init(Vector *vec, size_t elementSize, size_t expectedElements)
{
  if (!vec || elementSize == 0 || expectedElements == 0)
    return false;

  if (elementSize > SIZE_MAX / expectedElements)
    return false;

  size_t totalSize = elementSize * expectedElements;

  vec->v = malloc(totalSize);
  if (!vec->v)
  {
    vec->itemBytes = 0;
    vec->currentSize = 0;
    vec->capacity = 0;
    return false;
  }

  vec->itemBytes = elementSize;
  vec->currentSize = 0;
  vec->capacity = expectedElements;

  return true;
}

bool resize(Vector *vec, size_t newCapacity)
{
  if (!vec || newCapacity < vec->capacity)
    return false;
  size_t newBytes = newCapacity * vec->itemBytes;
  size_t oldBytes = vec->currentSize * vec->itemBytes;

  void *old = vec->v;
  if (!old)
    oldBytes = 0;

  void *next = malloc(newBytes);

  if (!next)
    return false;

  if (oldBytes > 0)
    memcpy(next, old, oldBytes);

  if (old)
    free(old);

  vec->v = next;
  vec->capacity = newCapacity;

  return true;
}

bool push(Vector *vec, void *item)
{
  if (!vec || !item)
    return false;

  if (vec->currentSize >= vec->capacity)
  {
    size_t newCapacity = vec->capacity * 2;
    if (vec->capacity == 0)
    {
      newCapacity = 1;
    }

    if (!resize(vec, newCapacity))
    {
      return false;
    }
  }

  size_t target = vec->currentSize * vec->itemBytes;
  memcpy((char *)vec->v + target, item, vec->itemBytes);
  vec->currentSize++;

  return true;
}

bool pushInt(Vector *vec, int item)
{
  if (!vec)
    return false;

  if (vec->currentSize >= vec->capacity)
  {
    size_t newCapacity = (vec->capacity == 0) ? 1 : vec->capacity * 2;

    if (!resize(vec, newCapacity))
      return false;
  }

  int *arr = (int *)vec->v;
  arr[vec->currentSize] = item;

  vec->currentSize++;

  return true;
}

void *getItem(const Vector *vec, int index)
{
  char *data = (char *)vec->v;
  if (index < 0 || index >= (int)vec->currentSize)
  {
    printf("Warning: Accessing invalid vector index: %i\n", index);
    return NULL;
  }
  return data + index * (int)vec->itemBytes;
}