#pragma once
#include "vector.h"
#include "tokenTypes.h"
#include "arena.h"

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

ASTNode *createIntNode(Arena *arena, int i);

ASTNode *createFloatNode(Arena *arena, float f);

ASTNode *createDoubleNode(Arena *arena, double d);

ASTNode *createStringNode(Arena *arena, char *s);

ASTNode *createIdentifierNode(Arena *arena, char *name);

void printNumberNode(ASTNode *node);

const char *numberTypeToString(NumberType type);

void printStringNode(ASTNode *node);

void printIdentifierNode(ASTNode *node);