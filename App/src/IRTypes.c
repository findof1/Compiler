#include "IRTypes.h"

IROp convertOperator(TokenType op)
{
  switch (op)
  {
  case Plus:
    return IRAdd;

  case Minus:
    return IRSub;

  case Times:
    return IRMul;

  case Slash:
    return IRDiv;

  case Equals:
    return IRAssign;

  case Ls:
    return IRLessThan;

  case LsEq:
    return IRLessThanEq;

  case Gr:
    return IRGreaterThan;

  case GrEq:
    return IRGreaterThanEq;

  case DoubleEquals:
    return IREquals;

  case NotEq:
    return IRNotEquals;

  case And:
    return IRAnd;

  case Or:
    return IROr;

  case Not:
    return IRNot;

  default:
    fprintf(stderr, "Unsupported operator in convertOperator: %d\n", op);
    exit(EXIT_FAILURE);
  }
}