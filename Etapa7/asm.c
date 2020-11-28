#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ast.h"
#include "asm.h"
#include "symbol_table.h"

extern struct stack_symbol_table *stack;
extern struct ASM *initial;
extern struct ASM *current_asm;

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
    initial = gera_vazio();
    current_asm = initial;
    print_initial_info();
    print_global_info();
    gera_AsmCode(c);
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
    char *new_reg = (char *) malloc (5);
    snprintf(new_reg, 5, "%%%s", reg);
    struct ASM *mov = gera_asm(ASM_movl, "(%%rsp)", new_reg);
    // printf("\tmovl\t(%%rsp), %%%s\n", reg); // Tira da pilha
    struct ASM *add = gera_asm(ASM_addq, "$4", "%%rsp");
    // printf("\taddq\t$4, %%rsp\n");
    current_asm->prox = mov;
    mov->prox = add;
    current_asm = add;
}

void push_Asm() {
    // printf("\tsubq\t$4, %%rsp\n");
    struct ASM *sub = gera_asm(ASM_subq, "$4", "%%rsp");
    // printf("\tmovl\t%%eax, (%%rsp)\n"); // Bota na pilha
    struct ASM *mov = gera_asm(ASM_movl, "%%eax", "(%%rsp)");
    current_asm->prox = sub;
    sub->prox = mov;
    current_asm = mov;
}

void push_val(int value) {
    // printf("\tsubq\t$4, %%rsp\n");
    struct ASM *sub = gera_asm(ASM_subq, "$4", "%%rsp");
    // printf("\tmovl\t$%d, (%%rsp)\n", value); // Bota na pilha
    char *new_val = (char *) malloc (6);
    snprintf(new_val, 6, "$%d", value);
    struct ASM *mov = gera_asm(ASM_movl, new_val, "(%%rsp)");
    current_asm->prox = sub;
    sub->prox = mov;
    current_asm = mov;
}


// void opBin_Asm(char *op) {
void opBin_Asm(ASM_OP op) {
    pop_Asm("edx");
    pop_Asm("eax");
    // printf("\t%s\t%%edx, %%eax\n", op); // OP
    struct ASM *bin = gera_asm(op, "%%edx", "%%eax");
    current_asm->prox = bin;
    current_asm = bin;
    push_Asm();
}

// void opDiv_Asm(char *op) {
void opDiv_Asm(ASM_OP op) {
    pop_Asm("ebx");
    pop_Asm("eax");
    // printf("\tcdq\n"); // Extende sinal
    struct ASM *cdq = gera_asm(ASM_cdq, "", "");
    // printf("\t%s\t%%ebx\n", op); // OP
    struct ASM *div = gera_asm(op, "%%ebx", "");
    current_asm->prox = cdq;
    cdq->prox = div;
    current_asm = div;
    push_Asm();
}

void opI_Asm(char *op, struct code *c) {
    char *oper1, *oper2;
    char *oper1_1;
    char *oper1_2;
    char *oper2_2;
    if (c->arg1 == c->dest1 && c->arg1 < 0) { // Sendo utilizado para inicio de funcao 
        // printf("\t%sq\t$%d, %%%s\n", op, c->arg2, converte_AsmReg(c->arg1));
        oper1 = (char *) malloc (6);
        oper2 = (char *) malloc (5);
        snprintf(oper1, 6, "$%d", c->arg2);
        snprintf(oper2, 5, "%%%s", converte_AsmReg(c->arg1));
        current_asm->prox = gera_asm((strcmp("add", op) == 0 ? ASM_addq : ASM_subq), oper1, oper2);
        current_asm = current_asm->prox;
    } else {
        if (c->arg1 < 0) {
            // printf("\tmovl\t%%%s, %%eax\n", converte_AsmReg(c->arg1));
            oper1_1 = (char *) malloc (5);
            snprintf(oper1_1, 5, "%%%s", converte_AsmReg(c->arg1));
            current_asm->prox = gera_asm(ASM_movl, oper1_1, "%%eax");
            current_asm = current_asm->prox;
        } else {
            pop_Asm("eax");
        }
        // printf("\t%sl\t$%d, %%eax\n", op, c->arg2);
        oper1 = (char *) malloc (6);
        snprintf(oper1, 6, "$%d", c->arg2);
        current_asm->prox = gera_asm((strcmp("add", op) == 0 ? ASM_addl : ASM_subl), oper1, "%%eax");
        current_asm = current_asm->prox;
        if (c->dest1 < 0) {
            // printf("\tmovl\t%%eax, %%%s\n", converte_AsmReg(c->dest1));
            oper2_2 = (char *) malloc (5);
            snprintf(oper2_2, 6, "%%%s", converte_AsmReg(c->dest1));
            current_asm->prox = gera_asm(ASM_movl, "%%eax", oper2_2);
            current_asm = current_asm->prox;
        } else {
            push_Asm();
        }
    }
}

void opLoadI_Asm(struct code *c) {
    char *oper1, *oper2;
    if (c->dest1 < 0) {
        // printf("\tmovl\t$%d, %%%s\n", c->arg1, converte_AsmReg(c->dest1));
        oper1 = (char *) malloc (6);
        oper2 = (char *) malloc (6);
        snprintf(oper1, 6, "$%d", c->arg1);
        snprintf(oper2, 6, "%%%s", converte_AsmReg(c->dest1));
        current_asm->prox = gera_asm(ASM_movl, oper1, oper2);
        current_asm = current_asm->prox;
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

void opStoreAI_Asm(struct code *c) {
    // Tirar da pilha + atualizar pilha
    pop_Asm("eax");
    // Store
    if (c->dest1 == RBSS)
        printf("\tmovl\t%%eax, %s(%%rip)\n", var_globl_desloc(c->dest2));
    else
        printf("\tmovl\t%%eax, %d(%%%s)\n", c->dest2, converte_AsmReg(c->dest1));
}

void jmpCond_Asm(char *jump_cond, int label) {
    pop_Asm("edx");
    pop_Asm("eax");
    printf("\tcmpl\t%%edx, %%eax\n");
    printf("\t%s\t.L%d\n", jump_cond, label);
}

void callFunction_Asm(int label_fun) {
    // printf("\tcall\t%s\n", get_function_name(label_fun));
    current_asm->prox = gera_asm(ASM_call, get_function_name(label_fun), "");
    current_asm = current_asm->prox;
}

int load_parameters(int label_fun) {
    struct elem_table *elemento = get_elem_function(label_fun);
    struct elem_table *args = elemento->argumentos;
    if (args == NULL) return 0;
    int var_local = 16;
    int val_pilha = 16;
    int tot_var_local = 0;
    struct elem_table *args_aux = args;
    while(args_aux != NULL) {
        val_pilha += 4;
        tot_var_local++;
        args_aux = args_aux->next_elem;
    }

    while(args!=NULL) {
        printf("\tmovl\t-%d(%%rbp), %%eax\n", val_pilha);
        printf("\tmovl\t%%eax, %d(%%rbp)\n", var_local);
        var_local -= 4;
        val_pilha -= 4;
        args = args->next_elem;
    }
    return tot_var_local;
}
// int pop_from_return_function;
void gera_AsmCode(struct code *c) {
    if (c == NULL) return;
    if (c->label != NULL_LABEL) {
        if (printa_label_fun(c->label)){
            // pop_from_return_function = load_parameters(c->label);
        } else
            printf(".L%d:\n", c->label);
    }

    if (c->tipo == code_preparacao_chamada) {
        // Ignora store de RSP e RFP
        gera_AsmCode(c->prox->prox);
        return;
    }

    if (c->tipo == code_load_retorno_funcao) {
        push_Asm();
        gera_AsmCode(c->prox);
        return;
    }

    if (c->tipo == code_returno_funcao) {
        printf("\tmovl\t(%%rsp), %%eax\n");
        printf("\taddq\t$4, %%rsp\n");
        gera_AsmCode(c->prox);
        return;
    }

    if (c->tipo == code_saida_fun_main) {
        printf("\tleave\n");
        printf("\tret\n");
        return; // Fim do programa - sem nada depois da main
    }

    if (c->tipo == code_saida_funcao) {
        printf("\tleave\n");
        printf("\tret\n");
        gera_AsmCode(c->prox->prox->prox);
        return;
    }

    switch (c->operation)
    {
    case op_add:
        // opBin_Asm("addl");
        opBin_Asm(ASM_addl);
        break;
    case op_sub:
        // opBin_Asm("subl");
        opBin_Asm(ASM_subl);
        break;
    case op_mult:
        // opBin_Asm("imull");
        opBin_Asm(ASM_imull);
        break;
    case op_div:
        // opDiv_Asm("idivl");
        opBin_Asm(ASM_idivl);
        break;
    case op_addI:
        if (c->arg1 == RPC){
            callFunction_Asm(c->prox->prox->dest1);
            c = c->prox->prox;
        } else 
            opI_Asm("add", c);
        break;
    case op_subI:
        opI_Asm("sub", c);
        break;
    case op_rsubI:
        pop_Asm("eax");
        printf("\tnegl\t%%eax\n");
        push_Asm();
        break;
    case op_multI:
        break;
    case op_divI:
        break;
    case op_loadI:
        opLoadI_Asm(c);
        break;
    case op_loadAI:
        opLoadAI_Asm(c);
        break;
    case op_storeAI:
        opStoreAI_Asm(c);
        break;
    case op_i2i:
        if (c->arg1 < 0 && c->dest1 < 0)
            printf("\tmovq\t%%%s, %%%s\n", converte_AsmReg(c->arg1), converte_AsmReg(c->dest1));
        // "else" Not currently on use
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
    //     break;
    case op_jumpI:
        printf("\tjmp\t.L%d\n", c->dest1);
        break;
    default:
        break;
    }
    gera_AsmCode(c->prox);
}

struct ASM *gera_asm(ASM_OP op, char *oper1, char *oper2) {
    struct ASM *asm_code = (struct ASM *) malloc (sizeof(struct ASM));
    asm_code->label = -1;
    asm_code->asm_operation = op;
    asm_code->operador1 = strdup(oper1);
    asm_code->operador2 = strdup(oper2);
    asm_code->prox = NULL;
    return asm_code;
}

struct ASM *gera_vazio() {
    return gera_asm(ASM_nop, "", "");
}