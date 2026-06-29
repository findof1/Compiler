#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "lexer.h"
#include "tokenParser.h"
#include "ast.h"
#include "analyzer.h"
#include "IRTypes.h"
#include "IRGenerator.h"

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
  printf("Starting Tokenizing\n");
  Vector v = tokenize(src);
  printTokenVector(&v);
  printf("Done Tokenizing\n");
  Arena arena;
  initArena(&arena, sizeof(ASTNode) * PARSER_CAPACITY);
  Parser parser;
  initParser(&parser, &v, &arena);
  printf("Starting Parsing\n");
  ASTNode *program = parseProgram(&parser);
  printf("Done Parsing\n");
  printAST(program, 0);
  printf("Starting Analysis\n");

  if (analyzeAST(&arena, program))
  {
    printf("Program is valid\n");
  }
  else
  {
    printf("Program is not valid\n");
  }

  IRProgram programGen;
  init(&programGen.instructions, sizeof(IRInstruction), 10);
  programGen.nextLabel = 0;
  programGen.nextTemporary = 0;

  generateProgram(&programGen, program);
  printIR(&programGen);

  free(src);
  src = NULL;
  getchar();
  return 0;
}