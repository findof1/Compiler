#include "ast.h"

ASTNode *createIntNode(Arena *arena, int i)
{
  ASTNode *node = (ASTNode *)arenaAlloc(arena, sizeof(ASTNode));
  if (!node)
  {
    return NULL;
  }
  node->type = NumberLiteral;

  node->number.type = Integer;
  node->number.intValue = i;

  return node;
}

ASTNode *createFloatNode(Arena *arena, float f)
{
  ASTNode *node = (ASTNode *)arenaAlloc(arena, sizeof(ASTNode));
  if (!node)
  {
    return NULL;
  }
  node->type = NumberLiteral;

  node->number.type = Float;
  node->number.floatValue = f;

  return node;
}

ASTNode *createDoubleNode(Arena *arena, double d)
{
  ASTNode *node = (ASTNode *)arenaAlloc(arena, sizeof(ASTNode));
  if (!node)
  {
    return NULL;
  }
  node->type = NumberLiteral;

  node->number.type = Double;
  node->number.doubleValue = d;

  return node;
}

ASTNode *createStringNode(Arena *arena, char *s)
{
  ASTNode *node = (ASTNode *)arenaAlloc(arena, sizeof(ASTNode));
  if (!node)
  {
    return NULL;
  }
  node->type = StringLiteral;

  node->string.value = s;

  return node;
}

ASTNode *createIdentifierNode(Arena *arena, char *name)
{
  ASTNode *node = (ASTNode *)arenaAlloc(arena, sizeof(ASTNode));
  if (!node)
  {
    return NULL;
  }
  node->type = Identifier;

  node->identifier.name = name;

  return node;
}

void printNumberNode(ASTNode *node)
{

  double number = 0;
  if (node->number.type == Integer)
  {
    number = (double)node->number.intValue;
  }
  else if (node->number.type == Float)
  {
    number = (double)node->number.floatValue;
  }
  else
  {
    number = (double)node->number.doubleValue;
  }
  printf("Node: \n Node Type: Number Literal\n Number Type: %s\n Value: %lf\n", numberTypeToString(node->number.type), number);
}

const char *numberTypeToString(NumberType type)
{
  static const char *names[] = {
      "Integer",
      "Float",
      "Double"};

  if (type < 0 || type >= (sizeof(names) / sizeof(names[0])))
    return "Unknown";

  return names[type];
}