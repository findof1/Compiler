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
    float doubleValue;
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
  };
} ASTNode;