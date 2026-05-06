#include "tokenTypes.h"
#include "vector.h"

void printTokenVector(const Vector *vec)
{
  for (int i = 0; i < (int)vec->currentSize; i++)
  {
    void *ptr = getItem(vec, i);
    if (!ptr)
    {
      printf("Printing Token Error: invalid token vector\n");
      continue;
    }
    Token *t = (Token *)ptr;
    if (!t)
    {
      printf("Printing Token Error: items are wrong type\n");
      continue;
    }
    printToken(*t);
  }
}

void printToken(Token token)
{
  printf("Token: \n Type: %s\n  Value: %s\n", tokenTypeToString(token.type), token.value ? token.value : "NULL");
}

const char *tokenTypeToString(TokenType type)
{
  static const char *names[] = {
      "Number",
      "Identifier",

      "Plus",
      "Minus",
      "Times",
      "Slash",

      "OpenParen",
      "ClosedParen",
      "OpenBracket",
      "ClosedBracket",
      "OpenBrace",
      "ClosedBrace",
      "Semicolon",
      "Comma",
      "Dot",
      "SingleQuote",
      "DoubleQuote",

      "IntType",
      "FloatType",
      "DoubleType",
      "StrType",
      "CharType",

      "If",
      "Else",
      "While",

      "Gr",
      "GrEq",
      "Ls",
      "LsEq",
      "NotEq",
      "Equals",
      "DoubleEquals",

      "And",
      "Or",
      "Not",

      "EndOfFile"};

  if (type < 0 || type >= (sizeof(names) / sizeof(names[0])))
    return "Unknown";

  return names[type];
}