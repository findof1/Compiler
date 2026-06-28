#include "IRGenerator.h"

ValueType tokenTypeToValueType(TokenType type)
{
  switch (type)
  {
  case IntType:
    return TypeInt;
  case FloatType:
    return TypeFloat;
  case DoubleType:
    return TypeDouble;
  case StrType:
    return TypeStr;
  default:
    printf("Token Type %i Cannot Be Converted Into Value Type", type);
    return TypeInt;
  }
}

Operand generateStatement(IRProgram *program, ASTNode *stat)
{
  (void)program;
  (void)stat;
  if (stat->type == IfStatement)
  {
  }
  Operand temp;
  temp.type = OperandTemp;
  return temp;
}

Operand generateExpression(IRProgram *program, ASTNode *expr)
{

  if (expr->type == BinaryExpr)
  {
    Operand temp;
    temp.type = OperandTemp;
    temp.dataType = tokenTypeToValueType(expr->binaryExpr.resolvedType);
    temp.id = program->nextTemporary++;
    IRInstruction instruction;
    instruction.destination = temp;
    instruction.left = generateExpression(program, expr->binaryExpr.left);
    instruction.right = generateExpression(program, expr->binaryExpr.right);
    instruction.op = convertOperator(expr->binaryExpr.op);
    push(&program->instructions, &instruction);
    return temp;
  }

  if (expr->type == Identifier)
  {
    Operand op;
    op.type = OperandVar;
    if (expr->identifier.symbol == NULL)
    {
      printf("IR Generation Error: Identifier was not given a symbol in analysis.");
      getchar();
      exit(EXIT_FAILURE);
    }
    op.dataType = tokenTypeToValueType(expr->identifier.symbol->type);
    op.id = expr->identifier.varId;
    return op;
  }

  if (expr->type == NumberLiteral)
  {
    Operand op;
    op.type = OperandConst;
    switch (expr->number.type)
    {
    case Integer:
      op.dataType = TypeInt;
      op.intValue = expr->number.intValue;
      break;
    case Float:
      op.dataType = TypeFloat;
      op.floatValue = expr->number.floatValue;
      break;
    case Double:
      op.dataType = TypeDouble;
      op.doubleValue = expr->number.doubleValue;
      break;
    }
    return op;
  }

  printf("IR Generation Error: Unsupported expression type: %i.\n", expr->type);
  exit(EXIT_FAILURE);
}