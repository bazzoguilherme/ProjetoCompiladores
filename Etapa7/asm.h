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

typedef enum ASM_NOTES {
    NoNote = 0,
    Note_Push=1,
    Note_Pop,
} ASM_NOTES;

struct ASM {
    char *label; // Label .Lx
    ASM_OP asm_operation; // Operation (movl, addl..)
    char *operador1;
    char *operador2;
    ASM_NOTES note;
    struct ASM *prox;
};

struct ASM *initial;
struct ASM *current_asm;

struct ASM *generateAsm(struct code *c);
void gera_AsmCode(struct code *c);
struct ASM *gera_asm(ASM_OP op, char *oper1, char *oper2);

void export_Asm(struct ASM *assembly);

struct ASM *gera_vazio();
struct ASM *gera_label_asm(char *label);
struct ASM *gera_label_num_asm(int label_id);

struct ASM *optimize_assembly(struct ASM *assembly_code);

#endif