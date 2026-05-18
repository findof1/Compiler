#include "ast.h"

void printIndent(int depth)
{
  for (int i = 0; i < depth; i++)
  {
    printf("  ");
  }
}

void printAST(ASTNode *node, int depth)
{
  if (!node)
  {
    printIndent(depth);
    printf("NULL\n");
    return;
  }

  switch (node->type)
  {
  case BinaryExpr:
  {
    printIndent(depth);
    printf("Binary Expression\n");

    printIndent(depth);
    printf("Operator: %s\n", tokenTypeToString(node->binaryExpr.op));

    printIndent(depth);
    printf("Left:\n");
    printAST(node->binaryExpr.left, depth + 1);

    printIndent(depth);
    printf("Right:\n");
    printAST(node->binaryExpr.right, depth + 1);
    break;
  }

  case NumberLiteral:
    printIndent(depth);
    printNumberNode(node);
    break;

  case StringLiteral:
    printIndent(depth);
    printStringNode(node);
    break;

  case Identifier:
    printIndent(depth);
    printIdentifierNode(node);
    break;

  default:
    printIndent(depth);
    printf("Unknown node type\n");
    break;
  }
}

void printNumberNode(ASTNode *node)
{
  double number = 0;

  if (node->number.type == Integer)
    number = (double)node->number.intValue;
  else if (node->number.type == Float)
    number = (double)node->number.floatValue;
  else
    number = (double)node->number.doubleValue;

  printf("Number Literal | Type: %s | Value: %lf\n",
         numberTypeToString(node->number.type),
         number);
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

void printStringNode(ASTNode *node)
{
  printf("String Literal | Value: \"%s\"\n", node->string.value);
}

void printIdentifierNode(ASTNode *node)
{
  printf("Identifier | Name: %s\n", node->identifier.name);
}