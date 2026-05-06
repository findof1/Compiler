#include "lexer.h"

bool isCharSkippable(const char c)
{
  if (c == ' ')
  {
    return true;
  }

  if (c == '\n')
  {
    return true;
  }

  if (c == '\r')
  {
    return true;
  }

  if (c == '\t')
  {
    return true;
  }

  return false;
}

bool isInt(char c)
{
  if (c >= '0' && c <= '9')
  {
    return true;
  }
  return false;
}

bool isAlpha(char c)
{
  if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
  {
    return true;
  }
  return false;
}

Vector tokenize(const char *sourceCode)
{
  Vector tokens;
  init(&tokens, sizeof(Token), 20);

  if (!sourceCode)
    return tokens;

  size_t sourceLength = strlen(sourceCode);

  int i = 0;
  while ((size_t)i < sourceLength)
  {
    char c = sourceCode[i];
    if (isCharSkippable(c))
    {
      i++;
      continue;
    }

    char nextC = ' ';
    if ((size_t)i + 1 < sourceLength)
    {
      nextC = sourceCode[i + 1];
    }

    if (c == '+')
    {
      Token t;
      t.type = Plus;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == '-')
    {
      Token t;
      t.type = Minus;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == '*')
    {
      Token t;
      t.type = Times;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == '/')
    {
      Token t;
      t.type = Slash;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == '(')
    {
      Token t;
      t.type = OpenParen;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == ')')
    {
      Token t;
      t.type = ClosedParen;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == '[')
    {
      Token t;
      t.type = OpenBracket;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == ']')
    {
      Token t;
      t.type = ClosedBracket;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == '{')
    {
      Token t;
      t.type = OpenBrace;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == '}')
    {
      Token t;
      t.type = ClosedBrace;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == ';')
    {
      Token t;
      t.type = Semicolon;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == ',')
    {
      Token t;
      t.type = Comma;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == '.')
    {
      Token t;
      t.type = Dot;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == '\'')
    {
      Token t;
      t.type = SingleQuote;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == '"')
    {
      Token t;
      t.type = DoubleQuote;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == '>' && nextC == '=')
    {
      Token t;
      t.type = GrEq;
      t.value = NULL;
      push(&tokens, &t);
      i += 2;
      continue;
    }
    else if (c == '>')
    {
      Token t;
      t.type = Gr;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == '<' && nextC == '=')
    {
      Token t;
      t.type = LsEq;
      t.value = NULL;
      push(&tokens, &t);
      i += 2;
      continue;
    }
    else if (c == '<')
    {
      Token t;
      t.type = Ls;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == '=' && nextC == '=')
    {
      Token t;
      t.type = DoubleEquals;
      t.value = NULL;
      push(&tokens, &t);
      i += 2;
      continue;
    }
    else if (c == '=')
    {
      Token t;
      t.type = Equals;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == '!' && nextC == '=')
    {
      Token t;
      t.type = NotEq;
      t.value = NULL;
      push(&tokens, &t);
      i += 2;
      continue;
    }
    else if (c == '!')
    {
      Token t;
      t.type = Not;
      t.value = NULL;
      push(&tokens, &t);
      i++;
      continue;
    }
    else if (c == '&' && nextC == '&')
    {
      Token t;
      t.type = And;
      t.value = NULL;
      push(&tokens, &t);
      i += 2;
      continue;
    }
    else if (c == '|' && nextC == '|')
    {
      Token t;
      t.type = Or;
      t.value = NULL;
      push(&tokens, &t);
      i += 2;
      continue;
    }
    else if (isInt(c))
    {
      int start = i;
      char number = c;
      while (isInt(number) && (size_t)i + 1 < sourceLength)
      {
        i++;
        number = sourceCode[i];
      }
      int numberLength = i - start;

      char *intStr = malloc((size_t)numberLength + 1);
      if (!intStr)
      {
        printf("Memory Allocation Failure when Tokenizing\n");
      }

      memcpy(intStr, sourceCode + start, (size_t)numberLength);
      intStr[numberLength] = '\0';

      Token t;
      t.type = Number;
      t.value = intStr;
      push(&tokens, &t);
      continue;
    }
    else if (isAlpha(c))
    {
      int start = i;
      char letter = c;
      while (isAlpha(letter) && (size_t)i + 1 < sourceLength)
      {
        i++;
        letter = sourceCode[i];
      }
      int identifierLength = i - start;
      char *identifier = malloc((size_t)identifierLength + 1);
      if (!identifier)
      {
        printf("Memory Allocation Failure when Tokenizing\n");
      }

      memcpy(identifier, sourceCode + start, (size_t)identifierLength);
      identifier[identifierLength] = '\0';

      if (strcmp(identifier, "int") == 0)
      {
        free(identifier);
        Token t;
        t.type = IntType;
        t.value = NULL;
        push(&tokens, &t);
        continue;
      }
      else if (strcmp(identifier, "float") == 0)
      {
        free(identifier);
        Token t;
        t.type = FloatType;
        t.value = NULL;
        push(&tokens, &t);
        continue;
      }
      else if (strcmp(identifier, "double") == 0)
      {
        free(identifier);
        Token t;
        t.type = DoubleType;
        t.value = NULL;
        push(&tokens, &t);
        continue;
      }
      else if (strcmp(identifier, "string") == 0 || strcmp(identifier, "str") == 0)
      {
        free(identifier);
        Token t;
        t.type = StrType;
        t.value = NULL;
        push(&tokens, &t);
        continue;
      }
      else if (strcmp(identifier, "char") == 0)
      {
        free(identifier);
        Token t;
        t.type = CharType;
        t.value = NULL;
        push(&tokens, &t);
        continue;
      }
      else if (strcmp(identifier, "if") == 0)
      {
        free(identifier);
        Token t;
        t.type = If;
        t.value = NULL;
        push(&tokens, &t);
        continue;
      }
      else if (strcmp(identifier, "elif") == 0)
      {
        free(identifier);
        Token t;
        t.type = Else;
        t.value = NULL;
        push(&tokens, &t);

        Token t2;
        t2.type = If;
        t2.value = NULL;
        push(&tokens, &t2);
        continue;
      }
      else if (strcmp(identifier, "else") == 0)
      {
        free(identifier);
        Token t;
        t.type = Else;
        t.value = NULL;
        push(&tokens, &t);
        continue;
      }
      else if (strcmp(identifier, "while") == 0)
      {
        free(identifier);
        Token t;
        t.type = While;
        t.value = NULL;
        push(&tokens, &t);
        continue;
      }
      else if (strcmp(identifier, "and") == 0)
      {
        free(identifier);
        Token t;
        t.type = And;
        t.value = NULL;
        push(&tokens, &t);
        continue;
      }
      else if (strcmp(identifier, "or") == 0)
      {
        free(identifier);
        Token t;
        t.type = Or;
        t.value = NULL;
        push(&tokens, &t);
        continue;
      }
      else if (strcmp(identifier, "not") == 0)
      {
        free(identifier);
        Token t;
        t.type = Not;
        t.value = NULL;
        push(&tokens, &t);
        continue;
      }
      else
      {
        Token t;
        t.type = Identifier;
        t.value = identifier;
        push(&tokens, &t);
        continue;
      }
    }
    else
    {
      printf("Invalid Character While Tokenizing: %c\n", c);
      i++;
      continue;
    }
  }

  Token t;
  t.type = EndOfFile;
  t.value = NULL;
  push(&tokens, &t);

  return tokens;
}