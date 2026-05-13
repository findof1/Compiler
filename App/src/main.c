#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "ast.h"

char *readFile(const char *path)
{
  FILE *file = fopen(path, "rb");
  if (!file)
    return NULL;

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  char *buffer = malloc((size_t)size + 1);
  if (!buffer)
  {
    fclose(file);
    return NULL;
  }

  fread(buffer, 1, (size_t)size, file);
  buffer[size] = '\0';

  fclose(file);
  return buffer;
}

int main(void)
{
  char *src = readFile("main.maw");
  if (!src)
  {
    printf("Failed to read file\n");
    return 1;
  }
  printf("%s\n", src);
  Vector v = tokenize(src);
  printTokenVector(&v);
  Arena arena;
  initArena(&arena, sizeof(ASTNode) * PARSER_CAPACITY);
  Parser parser;
  initParser(&parser, &v, &arena);
  ASTNode *node = parseNumberLiteral(&parser);
  printNumberNode(node);
  node = parseNumberLiteral(&parser);
  printNumberNode(node);
  node = parseNumberLiteral(&parser);
  printNumberNode(node);
  node = parseNumberLiteral(&parser);
  printNumberNode(node);
  node = parseNumberLiteral(&parser);
  printNumberNode(node);

  free(src);
  src = NULL;
  getchar();
  return 0;
}