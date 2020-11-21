#include "stdio.h"
#include "ast.h"
#include "asm.h"
#include "symbol_table.h"

extern struct stack_symbol_table *stack;

void print_initial_info() {
    printf("\t.file\t\"programa.c\"\n");
    printf("\t.text\n");
}

void print_global_info() {
    struct elem_table *elemento = stack->topo;
    while(elemento!=NULL){
        if (elemento->natureza == NAT_variavel){
            printf("\t.comm\t%s,4\n", elemento->key);
        } else if (elemento->natureza == NAT_funcao){
            printf("\t.globl\t%s\n", elemento->key);
            printf("\t.type\t%s, @function\n", elemento->key);
        } 
        elemento = elemento->next_elem;
    }
}

void generateAsm(struct code *c) {
    print_initial_info();
    print_global_info();
    print_AsmCode(c);
}

char *converte_AsmReg(int reg) {
    switch (reg)
    {
    case RBSS:
        return "rip";
        break;
    case RFP:
        return "rbp";
        break;
    case RSP:
        return "rsp";
        break;
    default:
        return "";
        break;
    }
}

void pop_Asm(char *reg) {
    printf("\tmovl\t(%%rsp), %%%s\n", reg); // Tira da pilha
    printf("\taddq\t$4, %%rsp\n");
}

void push_Asm() {
    printf("\tsubq\t$4, %%rsp\n");
    printf("\tmovl\t%%eax, (%%rsp)\n"); // Bota na pilha
}

void push_val(int value) {
    printf("\tsubq\t$4, %%rsp\n");
    printf("\tmovl\t$%d, (%%rsp)\n", value); // Bota na pilha
}

void opBin_Asm(char *op) {
    pop_Asm("edx");
    pop_Asm("eax");
    printf("\t%s\t%%edx, %%eax\n", op); // OP
    push_Asm();
}

void opDiv_Asm(char *op) {
    pop_Asm("edx");
    pop_Asm("eax");
    printf("\tcdq\n"); // Extende sinal
    printf("\t%s\t%%edx, %%eax\n", op); // OP
    push_Asm();
}

void opLoadI_Asm(struct code *c) {
    if (c->dest1 < 0) {
        printf("\tmovl\t$%d, %%%s\n", c->arg1, converte_AsmReg(c->dest1));
    } else {
        push_val(c->arg1);
    }
}

void opLoadAI_Asm(struct code *c) {
    if (c->dest1 < 0) {
        if (c->arg1 == RBSS)
            printf("\tmovl\t%s(%%rip), %%%s\n", var_globl_desloc(c->arg2), converte_AsmReg(c->dest1));
        else
            printf("\tmovl\t%d(%%%s), %%%s\n", c->arg2, converte_AsmReg(c->arg1), converte_AsmReg(c->dest1));
    } else {
        if (c->arg1 == RBSS)
            printf("\tmovl\t%s(%%rip), %%eax\n", var_globl_desloc(c->arg2));
        else
            printf("\tmovl\t%d(%%%s), %%eax\n", c->arg2, converte_AsmReg(c->arg1));
        push_Asm();
    }
}

void jmpCond_Asm(char *jump_cond, int label) {
    pop_Asm("edx");
    pop_Asm("eax");
    printf("\tcmpl\t%%edx, %%eax\n");
    printf("\t%s\t.L%d\n", jump_cond, label);
}

void callFunction_Asm(int label_fun) {
    printf("\tcall\t%s\n", get_function_name(label_fun));
}


void print_AsmCode(struct code *c) {
    if (c == NULL) return;
    if (c->label != NULL_LABEL) {
        printf(".L%d:\n", c->label);
        printa_label_fun(c->label);
    }

    if (c->tipo == code_preparacao_chamada) {
        // Ignora store de RSP e RFP
        c = c->prox->prox;
    }

    if (c->tipo == code_load_retorno_funcao) {
        push_Asm();
        c = c->prox;
    }

    if (c->tipo == code_returno_funcao) {
        printf("\tmovl\t(%%rsp), %%eax\n");
        printf("\taddq\t$4, %%rsp\n");
        c = c->prox;
    }

    if (c->tipo == code_saida_fun_main) {
        printf("\tleave\n");
        printf("\tret\n");
        return; // Fim do programa - sem nada depois da main
    }

    if (c->tipo == code_saida_funcao) {
        printf("\tleave\n");
        printf("\tret\n");
        c = c->prox->prox->prox;
    }

    switch (c->operation)
    {
    case op_add:
        opBin_Asm("addl");
        break;
    case op_sub:
        opBin_Asm("subl");
        break;
    case op_mult:
        opBin_Asm("imull");
        break;
    case op_div:
        opDiv_Asm("idivl");
        break;
    case op_addI:
        if (c->arg1 == RPC){
            callFunction_Asm(c->prox->prox->dest1);
            c = c->prox->prox;
        } else if (c->arg1 == c->dest1 && c->arg1 < 0) { // Sendo utilizado para inicio de funcao 
            printf("\taddq\t$%d, %%%s\n", c->arg2, converte_AsmReg(c->arg1));
        } else {
            if (c->arg1 < 0) {
                printf("\tmovl\t%%%s, %%eax\n", converte_AsmReg(c->arg1));
            } else {
                pop_Asm("eax");
            }
            printf("\taddl\t$%d, %%eax\n", c->arg2);
            if (c->dest1 < 0) {
                printf("\tmovl\t%%eax, %%%s\n", converte_AsmReg(c->dest1));
            } else {
                push_Asm();
            }
        }
        break;
    case op_subI:
        if (c->arg1 == c->dest1 && c->arg1 < 0) { // Sendo utilizado para inicio de funcao 
            printf("\tsubq\t$%d, %%%s\n", c->arg2, converte_AsmReg(c->arg1));
        } else {
            if (c->arg1 < 0) {
                printf("\tmovl\t%%%s, %%eax\n", converte_AsmReg(c->arg1));
            } else {
                pop_Asm("eax");
            }
            printf("\taddl\t$%d, %%eax\n", c->arg2);
            if (c->dest1 < 0) {
                printf("\tmovl\t%%eax, %%%s\n", converte_AsmReg(c->dest1));
            } else {
                push_Asm();
            }
        }
        break;
    case op_rsubI:
        printf("\tneg\t(%%rsp)\n");
        break;
    case op_multI:
        // printf("multI");
        break;
    case op_divI:
        // printf("divI");
        break;
    case op_loadI:
        opLoadI_Asm(c);
        break;
    case op_loadAI:
        opLoadAI_Asm(c);
        break;
    case op_storeAI:
        // Tirar da pilha + atualizar pilha
        pop_Asm("eax");
        // Store
        if (c->dest1 == RBSS)
            printf("\tmovl\t%%eax, %s(%%rip)\n", var_globl_desloc(c->dest2));
        else
            printf("\tmovl\t%%eax, %d(%%%s)\n", c->dest2, converte_AsmReg(c->dest1));
        break;
    case op_i2i:
        if (c->arg1 < 0 && c->dest1 < 0) {
            printf("\tmovq\t%%%s, %%%s\n", converte_AsmReg(c->arg1), converte_AsmReg(c->dest1));
        } else { // Not currently on use
            // printf("\tmovl\t(%%rsp), %%eax\n");
            // printf("\tmovl\t$4, %%rsp\n");
            // printf("\tsubl\t$4, %%rsp\n");
            // printf("\tmovl\t%%eax, (%%rsp)\n");
        }
        break;
    case op_cmp_LT: 
        jmpCond_Asm("jge", c->prox->dest2);
        c = c->prox;
        break;
    case op_cmp_LE:
        jmpCond_Asm("jg", c->prox->dest2);
        c = c->prox;
        break;
    case op_cmp_EQ:
        jmpCond_Asm("jne", c->prox->dest2);
        c = c->prox;
        break;
    case op_cmp_GE:
        jmpCond_Asm("jl", c->prox->dest2);
        c = c->prox;
        break;
    case op_cmp_GT:
        jmpCond_Asm("jle", c->prox->dest2);
        c = c->prox;
        break;
    case op_cmp_NE:
        jmpCond_Asm("je", c->prox->dest2);
        c = c->prox;
        break;
    // case op_jump:
    //     // printf("jump");
    //     // print_r_jmp(c);
    //     break;
    case op_jumpI:
        printf("\tjmp\t.L%d\n", c->dest1);
        break;
    default:
        break;
    }
    print_AsmCode(c->prox);
}

