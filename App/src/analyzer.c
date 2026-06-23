#include "analyzer.h"

void printScopedVariables(SemanticAnalyzer *analyzer)
{
  for (int i = 0; i < (int)analyzer->currentScope->symbols.currentSize; i++)
  {
    char **symbolPtr = getItem(&analyzer->currentScope->symbols, i);
    printf("'%s'\n", *symbolPtr);
  }
}

bool variableInScope(Scope *scope, char *var)
{
  for (int j = 0; j < (int)scope->symbols.currentSize; j++)
  {
    char **symbolPtr = (char **)getItem(&scope->symbols, j);
    char *symbol = *symbolPtr;

    if (strcmp(symbol, var) == 0)
    {
      return true;
    }
  }

  if (scope->parent != NULL)
  {
    return variableInScope(scope->parent, var);
  }
  return false;
}

bool analyzeAST(Arena *arena, ASTNode *ast)
{
  Scope *globalScope = arenaAlloc(arena, sizeof(Scope));
  globalScope->parent = NULL;
  init(&globalScope->symbols, sizeof(char *), 2);

  SemanticAnalyzer analyzer;
  analyzer.arena = arena;
  analyzer.currentScope = globalScope;
  (void)analyzer;
  if (ast->type != Program)
  {
    printf("AST Error: AST passed into analyzeAST() is not of type Program\n");
    return false;
  }

  return analyzeStatements(arena, &analyzer, &ast->program.statements);
}

bool analyzeIfStatement(Arena *arena, SemanticAnalyzer *analyzer, ASTNode *current)
{
  Scope *ifScope = arenaAlloc(arena, sizeof(Scope));
  ifScope->parent = analyzer->currentScope;
  init(&ifScope->symbols, sizeof(char *), 2);
  analyzer->currentScope = ifScope;

  if (!analyzeExpression(arena, analyzer, current->ifStatement.conditional))
  {
    return false;
  }
  if (!analyzeStatements(arena, analyzer, &current->ifStatement.body))
  {
    return false;
  }
  analyzer->currentScope = ifScope->parent;

  if (current->ifStatement.elseNode != NULL && current->ifStatement.elseNode->type == IfStatement)
  {
    return analyzeIfStatement(arena, analyzer, current->ifStatement.elseNode);
  }

  return true;
}

bool analyzeStatements(Arena *arena, SemanticAnalyzer *analyzer, Vector *statementsVector)
{
  const int max = (int)statementsVector->currentSize;
  for (int i = 0; i < max; i++)
  {
    ASTNode *current = *(ASTNode **)getItem(statementsVector, i);

    if (current->type == VariableDeclaration)
    {
      char *var = current->varDeclaration.identifier->identifier.name;
      if (variableInScope(analyzer->currentScope, var))
      {
        printf("AST Error: Cannot declare a variable twice. Variable %s was already declared\n", var);
        return false;
      }
      if (!analyzeExpression(arena, analyzer, current->varDeclaration.expression))
      {
        return false;
      }
      push(&analyzer->currentScope->symbols, &var);
    }
    else if (current->type == AssignmentExpr)
    {
      char *var = current->assignmentExpr.target->identifier.name;
      if (!variableInScope(analyzer->currentScope, var))
      {
        printf("AST Error: Assigning undefined variable: %s\n", var);
        return false;
      }

      if (current->assignmentExpr.expr != NULL && !analyzeExpression(arena, analyzer, current->assignmentExpr.expr))
      {
        return false;
      }
    }
    else if (current->type == IfStatement)
    {
      if (!analyzeIfStatement(arena, analyzer, current))
      {
        return false;
      }
    }
    else if (current->type == WhileStatement)
    {
      Scope *whileScope = arenaAlloc(arena, sizeof(Scope));
      whileScope->parent = analyzer->currentScope;
      init(&whileScope->symbols, sizeof(char *), 2);
      analyzer->currentScope = whileScope;
      if (!analyzeExpression(arena, analyzer, current->whileStatement.conditional))
      {
        return false;
      }
      if (!analyzeStatements(arena, analyzer, &current->whileStatement.body))
      {
        return false;
      }
      analyzer->currentScope = whileScope->parent;
    }
    else
    {
      if (!analyzeExpression(arena, analyzer, current))
      {
        return false;
      }
    }
  }

  return true;
}

bool analyzeExpression(Arena *arena, SemanticAnalyzer *analyzer, ASTNode *expr)
{
  if (expr->type == BinaryExpr)
  {
    if (!analyzeExpression(arena, analyzer, expr->binaryExpr.left))
    {
      return false;
    }
    if (!analyzeExpression(arena, analyzer, expr->binaryExpr.right))
    {
      return false;
    }
  }

  if (expr->type == Identifier)
  {
    char *var = expr->identifier.name;
    if (!variableInScope(analyzer->currentScope, var))
    {
      printf("AST Error: Using undefined variable: %s\n", var);

      return false;
    }
  }

  return true;
}