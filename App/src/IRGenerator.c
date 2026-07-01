#include "IRGenerator.h"

void printOperand(const Operand *op)
{
  switch (op->type)
  {
  case OperandNone:
    printf("_");
    break;

  case OperandVar:
    printf("v%d", op->id);
    break;

  case OperandTemp:
    printf("t%d", op->id);
    break;

  case OperandLabel:
    printf("L%d", op->id);
    break;

  case OperandConst:
    switch (op->dataType)
    {
    case TypeInt:
      printf("%d", op->intValue);
      break;

    case TypeFloat:
      printf("%f", op->floatValue);
      break;

    case TypeDouble:
      printf("%lf", op->doubleValue);
      break;

    case TypeBool:
      printf(op->boolValue ? "true" : "false");
      break;

    case TypeChar:
      printf("'%c'", op->charValue);
      break;

    case TypeStr:
      printf("\"%s\"", op->strValue);
      break;
    }
    break;
  }
}

void printIR(const IRProgram *program)
{
  for (size_t i = 0; i < program->instructions.currentSize; i++)
  {
    IRInstruction *inst = getItem(&program->instructions, (int)i);

    switch (inst->op)
    {
    case IRAdd:
      printOperand(&inst->destination);
      printf(" = ");
      printOperand(&inst->left);
      printf(" + ");
      printOperand(&inst->right);
      break;

    case IRSub:
      printOperand(&inst->destination);
      printf(" = ");
      printOperand(&inst->left);
      printf(" - ");
      printOperand(&inst->right);
      break;

    case IRMul:
      printOperand(&inst->destination);
      printf(" = ");
      printOperand(&inst->left);
      printf(" * ");
      printOperand(&inst->right);
      break;

    case IRDiv:
      printOperand(&inst->destination);
      printf(" = ");
      printOperand(&inst->left);
      printf(" / ");
      printOperand(&inst->right);
      break;

    case IRAssign:
      printOperand(&inst->destination);
      printf(" = ");
      printOperand(&inst->left);
      break;

    case IRLessThan:
      printOperand(&inst->destination);
      printf(" = ");
      printOperand(&inst->left);
      printf(" < ");
      printOperand(&inst->right);
      break;

    case IRLessThanEq:
      printOperand(&inst->destination);
      printf(" = ");
      printOperand(&inst->left);
      printf(" <= ");
      printOperand(&inst->right);
      break;

    case IRGreaterThan:
      printOperand(&inst->destination);
      printf(" = ");
      printOperand(&inst->left);
      printf(" > ");
      printOperand(&inst->right);
      break;

    case IRGreaterThanEq:
      printOperand(&inst->destination);
      printf(" = ");
      printOperand(&inst->left);
      printf(" >= ");
      printOperand(&inst->right);
      break;

    case IREquals:
      printOperand(&inst->destination);
      printf(" = ");
      printOperand(&inst->left);
      printf(" == ");
      printOperand(&inst->right);
      break;

    case IRNotEquals:
      printOperand(&inst->destination);
      printf(" = ");
      printOperand(&inst->left);
      printf(" != ");
      printOperand(&inst->right);
      break;

    case IRAnd:
      printOperand(&inst->destination);
      printf(" = ");
      printOperand(&inst->left);
      printf(" && ");
      printOperand(&inst->right);
      break;

    case IROr:
      printOperand(&inst->destination);
      printf(" = ");
      printOperand(&inst->left);
      printf(" || ");
      printOperand(&inst->right);
      break;

    case IRNot:
      printOperand(&inst->destination);
      printf(" = !");
      printOperand(&inst->left);
      break;

    case IRJump:
      printf("goto ");
      printOperand(&inst->destination);
      break;

    case IRJumpIfFalse:
      printf("ifFalse ");
      printOperand(&inst->left);
      printf(" goto ");
      printOperand(&inst->destination);
      break;

    case IRLabel:
      printOperand(&inst->destination);
      printf(":");
      break;

    case IRReturn:
      printf("return ");
      printOperand(&inst->left);
      break;

    default:
      printf("<unknown instruction>");
      break;
    }

    printf("\n");
  }
}

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
  case BoolType:
    return TypeBool;
  default:
    printf("Token Type %i Cannot Be Converted Into Value Type", type);
    return TypeInt;
  }
}

void generateProgram(IRProgram *program, ASTNode *rootNode)
{

  if (rootNode->type != Program)
  {
    printf("IR Generation Error: Node passed into IR Generation is not of type program");
    return;
  }

  for (int i = 0; i < (int)rootNode->program.statements.currentSize; i++)
  {
    ASTNode **statPtr = getItem(&rootNode->program.statements, i);
    generateStatement(program, *statPtr);
  }
}

void generateIfStatement(IRProgram *program, ASTNode *stat, Operand endLabel)
{

  bool hasElse = (stat->ifStatement.elseNode != NULL) && (stat->ifStatement.elseNode->type == IfStatement);
  if (hasElse)
  {
    Operand nextLabel;
    nextLabel.type = OperandLabel;
    nextLabel.id = program->nextLabel++;

    if (stat->ifStatement.conditional != NULL)
    {
      Operand condition = generateExpression(program, stat->ifStatement.conditional);
      IRInstruction jumpIfFalse;
      jumpIfFalse.destination = nextLabel;
      jumpIfFalse.left = condition;
      jumpIfFalse.right.type = OperandNone;
      jumpIfFalse.op = IRJumpIfFalse;
      push(&program->instructions, &jumpIfFalse);
    }

    for (int i = 0; i < (int)stat->ifStatement.body.currentSize; i++)
    {
      ASTNode **statPtr = getItem(&stat->ifStatement.body, i);
      generateStatement(program, *statPtr);
    }

    IRInstruction jumpToEnd;
    jumpToEnd.destination = endLabel;
    jumpToEnd.left.type = OperandNone;
    jumpToEnd.right.type = OperandNone;
    jumpToEnd.op = IRJump;
    push(&program->instructions, &jumpToEnd);

    IRInstruction label;
    label.op = IRLabel;
    label.destination = nextLabel;
    label.left.type = OperandNone;
    label.right.type = OperandNone;
    push(&program->instructions, &label);

    generateIfStatement(program, stat->ifStatement.elseNode, endLabel);
  }
  else
  {
    if (stat->ifStatement.conditional != NULL)
    {
      Operand condition = generateExpression(program, stat->ifStatement.conditional);
      IRInstruction jumpIfFalse;
      jumpIfFalse.destination = endLabel;
      jumpIfFalse.left = condition;
      jumpIfFalse.right.type = OperandNone;
      jumpIfFalse.op = IRJumpIfFalse;
      push(&program->instructions, &jumpIfFalse);
    }

    for (int i = 0; i < (int)stat->ifStatement.body.currentSize; i++)
    {
      ASTNode **statPtr = getItem(&stat->ifStatement.body, i);
      generateStatement(program, *statPtr);
    }

    IRInstruction jumpToEnd;
    jumpToEnd.destination = endLabel;
    jumpToEnd.left.type = OperandNone;
    jumpToEnd.right.type = OperandNone;
    jumpToEnd.op = IRJump;
    push(&program->instructions, &jumpToEnd);
  }
}

void generateStatement(IRProgram *program, ASTNode *stat)
{
  if (stat->type == IfStatement)
  {

    Operand endLabel;
    endLabel.type = OperandLabel;
    endLabel.id = program->nextLabel++;

    generateIfStatement(program, stat, endLabel);

    IRInstruction label;
    label.op = IRLabel;
    label.destination = endLabel;
    label.left.type = OperandNone;
    label.right.type = OperandNone;
    push(&program->instructions, &label);
    return;
  }

  if (stat->type == WhileStatement)
  {

    Operand startLabel;
    startLabel.type = OperandLabel;
    startLabel.id = program->nextLabel++;

    Operand endLabel;
    endLabel.type = OperandLabel;
    endLabel.id = program->nextLabel++;

    IRInstruction startInstr;
    startInstr.op = IRLabel;
    startInstr.destination = startLabel;
    startInstr.left.type = OperandNone;
    startInstr.right.type = OperandNone;
    push(&program->instructions, &startInstr);

    Operand condition = generateExpression(program, stat->whileStatement.conditional);

    IRInstruction jumpIfFalse;
    jumpIfFalse.destination = endLabel;
    jumpIfFalse.left = condition;
    jumpIfFalse.right.type = OperandNone;
    jumpIfFalse.op = IRJumpIfFalse;
    push(&program->instructions, &jumpIfFalse);

    for (int i = 0; i < (int)stat->whileStatement.body.currentSize; i++)
    {
      ASTNode **statPtr = getItem(&stat->whileStatement.body, i);
      generateStatement(program, *statPtr);
    }

    IRInstruction jump;
    jump.destination = startLabel;
    jump.left.type = OperandNone;
    jump.right.type = OperandNone;
    jump.op = IRJump;
    push(&program->instructions, &jump);

    IRInstruction endInstr;
    endInstr.op = IRLabel;
    endInstr.destination = endLabel;
    endInstr.left.type = OperandNone;
    endInstr.right.type = OperandNone;
    push(&program->instructions, &endInstr);
    return;
  }

  if (stat->type == VariableDeclaration)
  {
    if (stat->varDeclaration.expression != NULL)
    {
      Operand expression = generateExpression(program, stat->varDeclaration.expression);

      IRInstruction assign;
      assign.destination = generateExpression(program, stat->varDeclaration.identifier);
      assign.left = expression;
      assign.right.type = OperandNone;
      assign.op = IRAssign;
      push(&program->instructions, &assign);
    }
    return;
  }

  generateExpression(program, stat);
  return;
}

Operand generateExpression(IRProgram *program, ASTNode *expr)
{

  if (expr->type == AssignmentExpr)
  {
    Operand expression = generateExpression(program, expr->assignmentExpr.expr);
    Operand var = generateExpression(program, expr->assignmentExpr.target);
    IRInstruction assign;
    assign.destination = var;
    assign.left = expression;
    assign.right.type = OperandNone;
    assign.op = IRAssign;
    push(&program->instructions, &assign);
    return var;
  }

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
    op.id = expr->identifier.symbol->id;
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
  getchar();
  exit(EXIT_FAILURE);
}