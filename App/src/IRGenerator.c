#include "IRGenerator.h"

Operand generateExpression(IRProgram *program, ASTNode *expr)
{

  if (expr->type == BinaryExpr)
  {
    Operand temp;
    temp.type = OperandTemp;
    temp.dataType = TypeInt; // ToDo: Generate resolved types in semantic analysis
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
    op.dataType = TypeInt; // ToDo: Generate resolved types in semantic analysis
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