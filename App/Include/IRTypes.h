#pragma once
#include "vector.h"
#include "tokenTypes.h"

typedef enum
{
  IRAdd,
  IRSub,
  IRMul,
  IRDiv,

  IRAssign,
  IRLoadInt,

  IRLessThan,
  IRLessThanEq,
  IRGreaterThan,
  IRGreaterThanEq,

  IREquals,
  IRNotEquals,

  IRAnd,
  IROr,
  IRNot,

  IRLabel,
  IRJump,
  IRJumpIfFalse,

  IRReturn
} IROp;

typedef enum
{
  OperandNone,
  OperandVar,
  OperandTemp,
  OperandConst,
  OperandLabel
} OperandType;

typedef enum
{
  TypeInt,
  TypeFloat,
  TypeDouble,
  TypeBool,
  TypeChar,
  TypeStr
} ValueType;

typedef struct
{
  OperandType type;
  ValueType dataType;
  union
  {
    int id;

    int intValue;
    float floatValue;
    double doubleValue;
    bool boolValue;
  };
} Operand;

typedef struct
{
  Operand destination;
  Operand left;
  Operand right;

  IROp op;
} IRInstruction;

typedef struct
{
  Vector instructions;
  int nextTemporary;
  int nextLabel;
} IRProgram;

IROp convertOperator(TokenType op);