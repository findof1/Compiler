#pragma once
#include "vector.h"
#include "tokenTypes.h"
#include "arena.h"
#include "ast.h"

typedef struct
{
  char *name;
  TokenType type;
} Symbol;

typedef struct Scope
{
  Vector symbols;
  struct Scope *parent;
} Scope;

typedef struct
{
  Scope *currentScope;
  Arena *arena;
} SemanticAnalyzer;

void printScopedVariables(SemanticAnalyzer *analyzer);
bool variableInScope(Scope *scope, char *var);
bool analyzeAST(Arena *arena, ASTNode *ast);
bool analyzeIfStatement(Arena *arena, SemanticAnalyzer *analyzer, ASTNode *current);
bool analyzeStatements(Arena *arena, SemanticAnalyzer *analyzer, Vector *statementsVector);
bool analyzeExpression(Arena *arena, SemanticAnalyzer *analyzer, ASTNode *expr);