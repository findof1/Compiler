#pragma once
#include "vector.h"
#include "tokenTypes.h"

typedef struct ASTNode ASTNode;

typedef enum
{
  // statements
  Program,
  VariableDeclaration,
  // FunctionDeclaration, TODO: add functions to ast and parsing
  IfStatement,
  WhileStatement,
  // expressions
  AssignmentExpr,
  NumberLiteral,
  StringLiteral,
  Identifier,
  BinaryExpr
} ASTNodeType;

typedef enum
{
  Integer,
  Float,
  Double
} NumberType;

typedef struct
{
  NumberType type;

  union
  {
    int intValue;
    float floatValue;
    double doubleValue;
  };
} NumberAST;

typedef struct
{
  char *value;
} StringAST;

typedef struct
{
  char *name;
} IdentifierAST;

typedef struct
{
  Vector statements;
} ProgramAST;

typedef struct
{
  TokenType varType;
  char *identifier;
  ASTNode *expression;
} VarDeclarationAST;

typedef struct
{
  ASTNode *conditional;
  Vector body;
  ASTNode *elseNode;
} IfStatementAST;

typedef struct
{
  ASTNode *conditional;
  Vector body;
} WhileStatementAST;

typedef struct
{
  ASTNode *left;
  TokenType op;
  ASTNode *right;
} BinaryExprAST;

typedef struct ASTNode
{
  ASTNodeType type;

  union
  {
    NumberAST number;
    StringAST string;
    IdentifierAST identifier;
    ProgramAST program;
    VarDeclarationAST varDeclaration;
    IfStatementAST ifStatement;
    BinaryExprAST binaryExpr;
    WhileStatementAST whileStatement;
  };
} ASTNode;

ASTNode createIntNode(int i)
{
  ASTNode node;
  node.type = NumberLiteral;

  node.number.type = Integer;
  node.number.intValue = i;

  return node;
}

ASTNode createFloatNode(float f)
{
  ASTNode node;
  node.type = NumberLiteral;

  node.number.type = Float;
  node.number.floatValue = f;

  return node;
}

ASTNode createDoubleNode(double d)
{
  ASTNode node;
  node.type = NumberLiteral;

  node.number.type = Double;
  node.number.doubleValue = d;

  return node;
}

ASTNode createStringNode(char *s)
{
  ASTNode node;
  node.type = StringLiteral;

  node.string.value = s;

  return node;
}

ASTNode createIdentifierNode(char *name)
{
  ASTNode node;
  node.type = Identifier;

  node.identifier.name = name;

  return node;
}