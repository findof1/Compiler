#include "AsmGenerator.h"

int getStackOffset(Operand op)
{
  return (op.id + 1) * 4;
}

void emitLoadEAX(FILE *out, Operand op)
{
  if (op.type == OperandConst)
  {
    fprintf(out, "    mov eax, %d\n", op.intValue);
  }
  else
  {
    fprintf(out, "    mov eax, DWORD PTR [rbp-%d]\n", getStackOffset(op));
  }
}

void emitStoreEAX(FILE *out, Operand op)
{
  if (op.type == OperandTemp || op.type == OperandVar)
  {
    fprintf(out, "    mov DWORD PTR [rbp-%d], eax\n", getStackOffset(op));
  }
  else
  {
    printf("Asm Generation Error: Destination operand is not of type Temp or Var\n");
  }
}

void emitAddEAX(FILE *out, Operand op)
{
  if (op.type == OperandConst)
  {
    fprintf(out, "    add eax, %d\n", op.intValue);
  }
  else
  {
    fprintf(out, "    add eax, DWORD PTR [rbp-%d]\n", getStackOffset(op));
  }
}

void generateAssembly(IRProgram *program, FILE *out)
{
  fprintf(out, ".intel_syntax noprefix\n\n");

  fprintf(out, ".text\n");

  fprintf(out, ".globl main\n\n");

  fprintf(out, "main:\n");

  fprintf(out, "    push rbp\n");
  fprintf(out, "    mov rbp, rsp\n");

  fprintf(out, "    sub rsp, 64\n\n");

  for (int i = 0; i < (int)program->instructions.currentSize; i++)
  {
    IRInstruction *instr = getItem(&program->instructions, i);

    switch (instr->op)
    {
    case IRAdd:
      emitLoadEAX(out, instr->left);
      emitAddEAX(out, instr->right);
      emitStoreEAX(out, instr->destination);
      break;
    default:
      break;
    }
  }
}