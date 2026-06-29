#include "tokenParser.h"

// returns current token and advances position
Token *advance(Parser *parser)
{
  Token *t = (Token *)getItem(parser->tokens, parser->position);

  if (t->type == EndOfFile) // so we don't accidently access past the end of the source
    return t;

  parser->position++;
  return t;
}

// returns current token and doesn't advance position
Token *get(Parser *parser)
{
  Token *t = (Token *)getItem(parser->tokens, parser->position);
  return t;
}

Token *expect(Parser *parser, TokenType type, const char *errMsg)
{
  Token *t = (Token *)getItem(parser->tokens, parser->position);
  if (t->type != type)
  {
    printf("Parsing Error: %s Got: %s Expected: %s", errMsg, tokenTypeToString(t->type), tokenTypeToString(type));
    getchar();
    return t;
  }

  if (t->type == EndOfFile) // so we don't accidently access past the end of the source
    return t;

  parser->position++;

  return t;
}

void initParser(Parser *parser, Vector *tokens, Arena *arena)
{
  parser->tokens = tokens;
  parser->position = 0;
  parser->arena = arena;
}

int getPrecedence(TokenType tokenType)
{
  if (tokenType == Equals)
  {
    return 1;
  }
  else if (tokenType == Or || tokenType == And)
  {
    return 2;
  }
  else if (tokenType == DoubleEquals || tokenType == NotEq || tokenType == Ls || tokenType == LsEq || tokenType == Gr || tokenType == GrEq)
  {
    return 3;
  }
  else if (tokenType == Plus)
  {
    return 4;
  }
  else if (tokenType == Minus)
  {
    return 5;
  }
  else if (tokenType == Times)
  {
    return 6;
  }
  else if (tokenType == Slash)
  {
    return 7;
  }

  return 0;
}

ASTNode *parseProgram(Parser *parser)
{
  ASTNode *program = (ASTNode *)arenaAlloc(parser->arena, sizeof(ASTNode));
  if (!program)
  {
    return NULL;
  }
  program->type = Program;
  init(&program->program.statements, sizeof(ASTNode *), 5);

  TokenType type = get(parser)->type;
  while (type != EndOfFile)
  {
    ASTNode *stmt = parseStatement(parser);
    push(&program->program.statements, &stmt);
    type = get(parser)->type;
  }

  return program;
}

ASTNode *parseStatement(Parser *parser)
{
  Token *current = get(parser);
  switch (current->type)
  {
  case IntType:
  case FloatType:
  case DoubleType:
  case StrType:
  case BoolType:
  case CharType:
    return parseVarDeclaration(parser);
  case If:
    return parseIfStatement(parser);
  case While:
    return parseWhileStatement(parser);
  default:
    return parseExpression(parser, 0);
  }
}

ASTNode *parseVarDeclaration(Parser *parser)
{
  TokenType varType = get(parser)->type;
  advance(parser);
  TokenType type = get(parser)->type;
  ASTNode *identifier = parseIdentifier(parser);
  if (identifier == NULL)
  {
    printf("Parsing Error: %s Got: %s Expected: %s", "Expected identifier during variable declaration", tokenTypeToString(type), tokenTypeToString(IdentifierToken));
    getchar();
  }
  expect(parser, Equals, "Expected equals sign during variable declaration");
  ASTNode *expr = parseExpression(parser, 0);

  ASTNode *varDecNode = (ASTNode *)arenaAlloc(parser->arena, sizeof(ASTNode));
  if (!varDecNode)
  {
    return NULL;
  }
  varDecNode->type = VariableDeclaration;
  varDecNode->varDeclaration.varType = varType;
  varDecNode->varDeclaration.identifier = identifier;
  varDecNode->varDeclaration.expression = expr;

  return varDecNode;
}

ASTNode *parseIfStatement(Parser *parser)
{
  advance(parser);
  expect(parser, OpenParen, "Expected open parenthesis during if statement.");
  ASTNode *comparison = parseExpression(parser, 0);
  expect(parser, ClosedParen, "Expected closed parenthesis during if statement.");
  expect(parser, OpenBrace, "Expected open brace during if statement.");

  ASTNode *ifNode = (ASTNode *)arenaAlloc(parser->arena, sizeof(ASTNode));
  if (!ifNode)
  {
    return NULL;
  }
  ifNode->type = IfStatement;
  ifNode->ifStatement.conditional = comparison;
  ifNode->ifStatement.elseNode = NULL;
  init(&ifNode->ifStatement.body, sizeof(ASTNode *), 5);

  TokenType type = get(parser)->type;
  while (type != EndOfFile && type != ClosedBrace)
  {
    ASTNode *stmt = parseStatement(parser);
    push(&ifNode->ifStatement.body, &stmt);
    type = get(parser)->type;
  }
  expect(parser, ClosedBrace, "Expected closed brace ending if statement.");

  type = get(parser)->type;
  if (type == Else)
  {
    advance(parser);
    type = get(parser)->type;
    if (type == If)
    {
      ifNode->ifStatement.elseNode = parseIfStatement(parser);
      return ifNode;
    }

    expect(parser, OpenBrace, "Expected open brace during if statement.");
    ASTNode *elseNode = (ASTNode *)arenaAlloc(parser->arena, sizeof(ASTNode));
    if (!elseNode)
    {
      return NULL;
    }
    elseNode->type = IfStatement;
    elseNode->ifStatement.conditional = NULL;
    elseNode->ifStatement.elseNode = NULL;

    init(&elseNode->ifStatement.body, sizeof(ASTNode *), 5);

    type = get(parser)->type;
    while (type != EndOfFile && type != ClosedBrace)
    {
      ASTNode *stmt = parseStatement(parser);
      push(&elseNode->ifStatement.body, &stmt);
      type = get(parser)->type;
    }

    ifNode->ifStatement.elseNode = elseNode;
  }

  return ifNode;
}

ASTNode *parseWhileStatement(Parser *parser)
{
  advance(parser);
  expect(parser, OpenParen, "Expected open parenthesis during while statement.");
  ASTNode *comparison = parseExpression(parser, 0);
  expect(parser, ClosedParen, "Expected closed parenthesis during while statement.");
  expect(parser, OpenBrace, "Expected open brace during while statement.");

  ASTNode *whileNode = (ASTNode *)arenaAlloc(parser->arena, sizeof(ASTNode));
  if (!whileNode)
  {
    return NULL;
  }
  whileNode->type = WhileStatement;
  whileNode->whileStatement.conditional = comparison;
  init(&whileNode->whileStatement.body, sizeof(ASTNode *), 5);

  TokenType type = get(parser)->type;
  while (type != EndOfFile && type != ClosedBrace)
  {
    ASTNode *stmt = parseStatement(parser);
    push(&whileNode->whileStatement.body, &stmt);
    type = get(parser)->type;
  }
  expect(parser, ClosedBrace, "Expected closed brace ending while statement.");

  return whileNode;
}

ASTNode *parseExpression(Parser *parser, int precedence)
{
  ASTNode *left = parsePrimary(parser);
  if (!left)
    return NULL;

  while (true)
  {
    Token *current = get(parser);
    TokenType op = current->type;
    int opPrecedence;
    if (op == OpenParen)
    {
      op = Times;
      opPrecedence = getPrecedence(Times);
      if (opPrecedence <= precedence)
        break;
    }
    else
    {
      opPrecedence = getPrecedence(op);
      if (opPrecedence <= precedence)
        break;
      advance(parser);
    }
    ASTNode *right = parseExpression(parser, opPrecedence);

    ASTNode *node = (ASTNode *)arenaAlloc(parser->arena, sizeof(ASTNode));
    if (!node)
    {
      return NULL;
    }
    if (op == Equals)
    {
      node->type = AssignmentExpr;
      node->assignmentExpr.target = left;
      node->assignmentExpr.expr = right;
    }
    else
    {
      node->type = BinaryExpr;
      node->binaryExpr.left = left;
      node->binaryExpr.op = op;
      node->binaryExpr.right = right;
    }

    left = node;
  }

  return left;
}

ASTNode *parsePrimary(Parser *parser)
{

  Token *current = get(parser);

  if (current->type == IdentifierToken)
  {
    return parseIdentifier(parser);
  }
  else if (current->type == Number)
  {
    return parseNumberLiteral(parser);
  }
  else if (current->type == DoubleQuote)
  {
    return parseStringLiteral(parser);
  }
  else if (current->type == OpenParen)
  {
    advance(parser);
    ASTNode *node = parseExpression(parser, 0);
    expect(parser, ClosedParen, "Missing closed paren in binary expression");

    return node;
  }
  else
  {
    advance(parser);
    printf("Parsing Error: %s Got: %s", "Expected Expression", tokenTypeToString(current->type));
    getchar();
    return NULL;
  }
}

ASTNode *parseNumberLiteral(Parser *parser)
{
  NumberType type = Integer;

  Token *current = get(parser);
  char *numStr = current->value;
  double number = 0;
  if (current->type == Number)
  {
    advance(parser);

    char *extra;
    number = strtod(numStr, &extra);

    if (strchr(numStr, '.')) // If number has a dot, it is a float by default
    {
      type = Float;
    }

    current = get(parser);
    if (current->type == DoubleIndicator) // If the number has a double indicator, it becomes a double even if it doesn't have a dot
    {
      advance(parser);
      type = Double;
    }
  }
  else
  {
    return NULL;
  }

  ASTNode *node = (ASTNode *)arenaAlloc(parser->arena, sizeof(ASTNode));
  if (!node)
  {
    return NULL;
  }
  node->type = NumberLiteral;
  node->number.type = type;

  if (type == Integer)
  {
    node->number.intValue = (int)number;
  }
  else if (type == Float)
  {
    node->number.floatValue = (float)number;
  }
  else
  {
    node->number.doubleValue = (double)number;
  }

  return node;
}

ASTNode *parseStringLiteral(Parser *parser)
{
  Token *current = get(parser);
  char *str;
  if (current->type == DoubleQuote)
  {
    advance(parser);
    current = get(parser);
    str = current->value;
    if (current->type == IdentifierToken)
    {
      advance(parser);
      current = get(parser);

      if (current->type != DoubleQuote)
      {
        return NULL; // invalid string
      }
    }
    else
    {
      return NULL; // invalid string
    }
  }
  else
  {
    return NULL; // invalid string
  }

  ASTNode *node = (ASTNode *)arenaAlloc(parser->arena, sizeof(ASTNode));
  if (!node)
  {
    return NULL;
  }
  node->type = StringLiteral;
  node->string.value = str;

  return node;
}

ASTNode *parseIdentifier(Parser *parser)
{

  Token *current = get(parser);
  char *name;
  if (current->type == IdentifierToken)
  {
    advance(parser);
    name = current->value;
  }
  else
  {
    return NULL;
  }

  ASTNode *node = (ASTNode *)arenaAlloc(parser->arena, sizeof(ASTNode));
  if (!node)
  {
    return NULL;
  }
  node->identifier.symbol = NULL;
  node->type = Identifier;
  node->identifier.name = name;

  return node;
}