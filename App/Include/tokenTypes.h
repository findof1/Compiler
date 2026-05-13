#pragma once
#include "vector.h"

typedef enum
{
  Number, // 0-9
  IdentifierToken,

  Plus,            // +
  Minus,           // -
  Times,           // *
  Slash,           // /
  DoubleIndicator, // ex: 1.002323d

  OpenParen,     // (
  ClosedParen,   // )
  OpenBracket,   // [
  ClosedBracket, // ]
  OpenBrace,     // {
  ClosedBrace,   // }
  Semicolon,     // ;
  Comma,         // ,
  Dot,           // .
  SingleQuote,   // '
  DoubleQuote,   // "

  IntType,    // int
  FloatType,  // float
  DoubleType, // double
  StrType,    // str
  CharType,   // char

  If,    // if
  Else,  // else
  While, // while

  Gr,           // >
  GrEq,         // >=
  Ls,           // <
  LsEq,         // <=
  NotEq,        // !=
  Equals,       // =
  DoubleEquals, // ==

  And, // and, &&
  Or,  // or, ||
  Not, // not, !

  EndOfFile
} TokenType;

typedef struct
{
  char *value;
  TokenType type;
} Token;

void printTokenVector(const Vector *vec);
void printToken(Token token);
const char *tokenTypeToString(TokenType type);