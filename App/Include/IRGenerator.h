#pragma once
#include "IRTypes.h"
#include "ast.h"

void printIR(const IRProgram *program);
void generateProgram(IRProgram *program, ASTNode *rootNode);
void generateStatement(IRProgram *program, ASTNode *stat);
Operand generateExpression(IRProgram *program, ASTNode *expr);
