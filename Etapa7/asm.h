#ifndef ASM_H
#define ASM_H

#include "gera_codigo.h"

typedef enum ASM_OP {
    ASM_nop = 0,
    ASM_pushq = 1,
    ASM_pushl,
    ASM_movl,
    ASM_movq,
    ASM_addq,
    ASM_addl,
    ASM_subq,
    ASM_subl,
    ASM_imull,
    ASM_idivl,
    ASM_cdq,
    ASM_negl,
    ASM_cmpl,
    ASM_jge,
    ASM_jg,
    ASM_jne,
    ASM_jl,
    ASM_jle,
    ASM_je,
    ASM_jmp,
    ASM_call,
    ASM_leave,
    ASM_ret,
    ASM_endbr64,
} ASM_OP;

typedef enum ASM_REG {
    ASM_M_rsp = 1,
    ASM_rsp,
    ASM_M_rbp,
    ASM_rbp,
    ASM_M_rip,
    ASM_eax,
    ASM_ebx,
    ASM_ecx,
    ASM_edx,
} ASM_REG;

struct ASM {
    int label; // Label .Lx
    ASM_OP asm_operation; // Operation (movl, addl..)
    int I_value; // imediate value for an operation
    ASM_REG r1; // register1
    ASM_REG r2; // register2
    int desloc_r1; // desloc in memory register1
    int desloc_r2; // desloc in memory register2
    struct ASM *prox;
};

void generateAsm(struct code *c);
void print_AsmCode(struct code *c);
struct ASM *gera_asm(int label, ASM_OP op, int I_val, ASM_REG r1, ASM_REG r2, int desloc1, int desloc2);

#endif