#pragma once
#include "vector.h"
#include "tokenTypes.h"
#include "arena.h"
#include "ast.h"

typedef struct Scope
{
  Vector symbols;
  struct Scope *parent;
} Scope;

typedef struct
{
  Scope *currentScope;
  Arena *arena;
  int nextId;
} SemanticAnalyzer;

void printScopedVariables(SemanticAnalyzer *analyzer);
Symbol *findStoredSymbol(Scope *scope, char *var);
bool analyzeAST(Arena *arena, ASTNode *ast);
bool analyzeIfStatement(Arena *arena, SemanticAnalyzer *analyzer, ASTNode *current);
bool analyzeStatements(Arena *arena, SemanticAnalyzer *analyzer, Vector *statementsVector);
bool analyzeExpression(Arena *arena, SemanticAnalyzer *analyzer, ASTNode *expr);