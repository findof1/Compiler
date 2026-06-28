#pragma once
#include "IRTypes.h"
#include "ast.h"

Operand generateStatement(IRProgram *program, ASTNode *stat);
Operand generateExpression(IRProgram *program, ASTNode *expr);
