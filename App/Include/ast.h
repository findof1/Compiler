#pragma once
#include "vector.h"
#include "tokenTypes.h"
#include "arena.h"

typedef struct ASTNode ASTNode;

typedef struct
{
  char *name;
  TokenType type;
  int id;
} Symbol; // used for semantic analysis

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
  Symbol *symbol; // resolved during semantic analysis
} IdentifierAST;

typedef struct
{
  Vector statements;
} ProgramAST;

typedef struct
{
  TokenType varType;
  ASTNode *identifier;
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
  TokenType resolvedType; // only for analysis and IR gen
} BinaryExprAST;

typedef struct
{
  ASTNode *target;
  ASTNode *expr;
} AssignmentExprAST;

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
    AssignmentExprAST assignmentExpr;
    WhileStatementAST whileStatement;
  };
} ASTNode;

void printAST(ASTNode *node, int depth);

void printNumberNode(ASTNode *node);

const char *numberTypeToString(NumberType type);

void printStringNode(ASTNode *node);

void printIdentifierNode(ASTNode *node);