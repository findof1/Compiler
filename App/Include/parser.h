#pragma once

#include "vector.h"
#include "ast.h"
#include "arena.h"

#define PARSER_CAPACITY 1000

typedef struct
{
  Vector *tokens;
  int position;
  Arena *arena;
} Parser;

// returns current token and advances position
Token *advance(Parser *parser)
{
  Token *t = (Token *)getItem(parser->tokens, parser->position);

  if (t->type == EndOfFile) // so we don't accidently access past the end of the source
    return t;

  parser->position++;
  return t;
}

// returns current token and doesn't advance position
Token *get(Parser *parser)
{
  Token *t = (Token *)getItem(parser->tokens, parser->position);
  return t;
}

void initParser(Parser *parser, Vector *tokens, Arena *arena)
{
  parser->tokens = tokens;
  parser->position = 0;
  parser->arena = arena;
}

ASTNode *parseNumberLiteral(Parser *parser)
{
  NumberType type = Integer;

  Token *current = get(parser);
  char *numStr = current->value;
  double number = 0;
  if (current->type == Number)
  {
    advance(parser);

    char *extra;
    number = strtod(numStr, &extra);

    if (strchr(numStr, '.')) // If number has a dot, it is a float by default
    {
      type = Float;
    }

    current = get(parser);
    if (current->type == DoubleIndicator) // If the number has a double indicator, it becomes a double even if it doesn't have a dot
    {
      advance(parser);
      type = Double;
    }
  }
  else
  {
    return NULL;
  }

  ASTNode *node = (ASTNode *)arenaAlloc(parser->arena, sizeof(ASTNode));
  if (!node)
  {
    return NULL;
  }
  node->type = NumberLiteral;
  node->number.type = type;

  if (type == Integer)
  {
    node->number.intValue = (int)number;
  }
  else if (type == Float)
  {
    node->number.floatValue = (float)number;
  }
  else
  {
    node->number.doubleValue = (double)number;
  }

  return node;
}

ASTNode *parseStringLiteral(Parser *parser)
{
  Token *current = get(parser);
  char *str;
  if (current->type == DoubleQuote)
  {
    advance(parser);
    current = get(parser);
    str = current->value;
    if (current->type == IdentifierToken)
    {
      advance(parser);
      current = get(parser);

      if (current->type != DoubleQuote)
      {
        return NULL; // invalid string
      }
    }
    else
    {
      return NULL; // invalid string
    }
  }
  else
  {
    return NULL; // invalid string
  }

  ASTNode *node = (ASTNode *)arenaAlloc(parser->arena, sizeof(ASTNode));
  if (!node)
  {
    return NULL;
  }
  node->type = StringLiteral;
  node->string.value = str;

  return node;
}

ASTNode *parseIdentifier(Parser *parser)
{

  Token *current = get(parser);
  char *name;
  if (current->type == IdentifierToken)
  {
    advance(parser);
    name = current->value;
  }
  else
  {
    return NULL;
  }

  ASTNode *node = (ASTNode *)arenaAlloc(parser->arena, sizeof(ASTNode));
  if (!node)
  {
    return NULL;
  }
  node->type = Identifier;
  node->identifier.name = name;

  return node;
}