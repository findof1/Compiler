#pragma once

#include "vector.h"
#include "ast.h"
#include "arena.h"

#define PARSER_CAPACITY 1000

typedef struct
{
  Vector *tokens;
  int position;
  Arena *arena;
} Parser;

// returns current token and advances position
Token *advance(Parser *parser);

// returns current token and doesn't advance position
Token *get(Parser *parser);

void initParser(Parser *parser, Vector *tokens, Arena *arena);

int getPrecedence(TokenType tokenType);

ASTNode *parseProgram(Parser *parser);

ASTNode *parseStatement(Parser *parser);

ASTNode *parseWhileStatement(Parser *parser);

ASTNode *parseIfStatement(Parser *parser);

ASTNode *parseExpression(Parser *parser, int precedence);

ASTNode *parsePrimary(Parser *parser);

ASTNode *parseNumberLiteral(Parser *parser);

ASTNode *parseStringLiteral(Parser *parser);

ASTNode *parseIdentifier(Parser *parser);