#include "analyzer.h"

void printScopedVariables(SemanticAnalyzer *analyzer)
{
  for (int i = 0; i < (int)analyzer->currentScope->symbols.currentSize; i++)
  {
    Symbol **symbolPtrPtr = getItem(&analyzer->currentScope->symbols, i);
    Symbol *symbolPtr = *symbolPtrPtr;
    printf("'%s'\n", symbolPtr->name);
  }
}

Symbol *findStoredSymbol(Scope *scope, char *var)
{
  for (int i = 0; i < (int)scope->symbols.currentSize; i++)
  {
    Symbol **symbolPtrPtr = getItem(&scope->symbols, i);
    Symbol *symbolPtr = *symbolPtrPtr;

    if (strcmp(symbolPtr->name, var) == 0)
    {
      return symbolPtr;
    }
  }

  if (scope->parent != NULL)
  {
    return findStoredSymbol(scope->parent, var);
  }
  return NULL;
}

bool analyzeAST(Arena *arena, ASTNode *ast)
{
  Scope *globalScope = arenaAlloc(arena, sizeof(Scope));
  globalScope->parent = NULL;
  init(&globalScope->symbols, sizeof(Symbol), 2);

  SemanticAnalyzer analyzer;
  analyzer.arena = arena;
  analyzer.currentScope = globalScope;
  analyzer.nextId = 0;
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
  init(&ifScope->symbols, sizeof(Symbol), 2);
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
      if (findStoredSymbol(analyzer->currentScope, var) != NULL)
      {
        printf("AST Error: Cannot declare a variable twice. Variable %s was already declared\n", var);
        return false;
      }
      if (!analyzeExpression(arena, analyzer, current->varDeclaration.expression))
      {
        return false;
      }
      Symbol *symbol = arenaAlloc(arena, sizeof(Symbol));
      symbol->name = var;
      symbol->type = current->varDeclaration.varType;
      symbol->id = analyzer->nextId++;
      current->varDeclaration.identifier->identifier.symbol = symbol;
      push(&analyzer->currentScope->symbols, &symbol);
    }
    else if (current->type == AssignmentExpr)
    {
      char *var = current->assignmentExpr.target->identifier.name;
      Symbol *symbol = findStoredSymbol(analyzer->currentScope, var);
      if (symbol == NULL)
      {
        printf("AST Error: Assigning undefined variable: %s\n", var);
        return false;
      }
      current->assignmentExpr.target->identifier.symbol = symbol;

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
      init(&whileScope->symbols, sizeof(Symbol), 2);
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

TokenType getExpressionType(ASTNode *expr)
{
  switch (expr->type)
  {
  case BinaryExpr:
    return expr->binaryExpr.resolvedType;
  case Identifier:
    return expr->identifier.symbol->type;
  case NumberLiteral:
    if (expr->number.type == Integer)
    {
      return IntType;
    }
    if (expr->number.type == Float)
    {
      return FloatType;
    }
    if (expr->number.type == Double)
    {
      return DoubleType;
    }
    return IntType;
  case StringLiteral:
    return StrType;
  default:
    printf("AST Error: Trying to get expression type of invalid type: %i\n", expr->type);
    return IntType;
  }
}

bool analyzeExpression(Arena *arena, SemanticAnalyzer *analyzer, ASTNode *expr)
{
  if (expr->type == BinaryExpr)
  {
    if (!analyzeExpression(arena, analyzer, expr->binaryExpr.left))
    {
      return false;
    }

    TokenType type = getExpressionType(expr->binaryExpr.left);
    if (!analyzeExpression(arena, analyzer, expr->binaryExpr.right))
    {
      return false;
    }
    TokenType type2 = getExpressionType(expr->binaryExpr.right);
    if (type != type2)
    {
      printf("AST Error: Binary Expression is using two different types\n");

      return false;
    }
    if (expr->binaryExpr.op == Gr || expr->binaryExpr.op == GrEq || expr->binaryExpr.op == Ls || expr->binaryExpr.op == LsEq || expr->binaryExpr.op == NotEq || expr->binaryExpr.op == DoubleEquals || expr->binaryExpr.op == And || expr->binaryExpr.op == Or || expr->binaryExpr.op == Not)
    {
      expr->binaryExpr.resolvedType = BoolType;
    }
    expr->binaryExpr.resolvedType = type;
  }

  if (expr->type == Identifier)
  {
    char *var = expr->identifier.name;
    Symbol *symbol = findStoredSymbol(analyzer->currentScope, var);
    if (symbol == NULL)
    {
      printf("AST Error: Using undefined variable: %s\n", var);

      return false;
    }
    expr->identifier.symbol = symbol;
  }

  return true;
}