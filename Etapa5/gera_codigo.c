#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ast.h"
#include "symbol_table.h"
#include "gera_codigo.h"

#define MAX_TAM_REGIS 12
#define MAX_TAM_LABEL 12
#define INT_SIZE_STR 12

int regis = 0, label = 0;

char *gera_regis() {
    char *new_regis = (char *) malloc (MAX_TAM_REGIS);
    snprintf(new_regis, MAX_TAM_REGIS, "r%d", regis++);
    return new_regis;
}

char *gera_label() {
    char *new_label = (char *) malloc (MAX_TAM_LABEL);
    snprintf(new_label, MAX_TAM_LABEL, "L%d", label++);
    return new_label;
}

struct code *alloc_code() {
    return (struct code *) malloc(sizeof(struct code));
}

char *int2str(int val) {
    char *new_key = (char *) malloc (INT_SIZE_STR);
    snprintf(new_key, INT_SIZE_STR, "%d", val);
    return new_key;
}

char *lex2str(struct valor_lexico_t *literal) {
    char *new_key = (char *) malloc (INT_SIZE_STR);
    snprintf(new_key, INT_SIZE_STR, "%d", literal->valor.val_int);
    return new_key;
}

struct code *concat(struct code *code1, struct code *code2, struct code *code3) {
    struct code *aux = code1;
    if (code1 == NULL)
        return NULL;

    while(aux->prox != NULL)
        aux = aux->prox;

    aux->prox = code2;

    while(aux->prox != NULL)
        aux = aux->prox;

    aux->prox = code3;
    return code1;
}

struct code *concat_codigos_ast(struct AST *ast1, struct AST *ast2, struct AST *ast3) {
    struct code *c1 = NULL, *c2 = NULL, *c3 = NULL;
    if (ast1 != NULL) {
        c1 = ast1->codigo;
    }
    if (ast2 != NULL) {
        c2 = ast2->codigo;
    }
    if (ast3 != NULL) {
        c3 = ast3->codigo;
    }
    return concat(c1, c2, c3);
}

struct code *gera_code(char *label, OP op, char* arg1, char *arg2, char *dest1, char *dest2, struct code *prox) {
    struct code *codigo = alloc_code();
    codigo->label = label;
    codigo->operation = op;
    codigo->arg1 = arg1;
    codigo->arg2 = arg2;
    codigo->dest1 = dest1;
    codigo->dest2 = dest2;
    codigo->prox = prox;
    printf("cria_code: %s %s, %s => %s\n", traduz_op(op), arg1, ((arg2 == NULL) ? "" : arg2), dest1);
    return codigo;
}

struct code *gera_loadI(OP op, struct valor_lexico_t *arg1, char *dest){
    return gera_code(NULL, op, lex2str(arg1), NULL, dest, NULL, NULL);
}

struct code *gera_loadI_sinal(OP op, struct valor_lexico_t *sinal, struct valor_lexico_t *arg1, char *dest) {
    struct code *c = gera_loadI(op, arg1, dest);
    if (sinal != NULL && sinal->valor.val_char == '-') { // Inverte valor registrador
        struct code *sub = gera_code(NULL, op_rsubI, "0", dest, dest, NULL, c);
        c = sub;
    }
    return c;
}

struct code *gera_load_var(OP op, struct AST *ast, char *dest) {
    int desloc, escopo;
    desloc = deslocamento_symbol(ast->valor_lexico->valor.val_str, &escopo);
    return gera_code(NULL, op, ((escopo == GLOBAL) ? "rbss":"rfp"), int2str(desloc), dest, NULL, NULL);
}

struct code *gera_decl_funcao(struct valor_lexico_t *nome_funcao) {
    struct code *atualiza_rfp = gera_code(NULL, op_i2i, "rsp", NULL, "rfp", NULL, NULL); // Atualiza RFP
    int desloc, escopo;
    desloc = deslocamento_funcao_atual();
    struct code *atualiza_rsp = gera_code(NULL, op_addI, "rsp", int2str(desloc), "rsp", NULL, NULL); // Atualiza RSP

    atualiza_rfp->prox = atualiza_rsp;
    
    return atualiza_rfp;
}

OP op_operacao(struct valor_lexico_t *operacao) {
    if (operacao->tipo == VAL_ESPECIAL) { // char
        return op_simples(operacao->valor.val_char);
    } else { // Op-composto
        // return op_composta(operacao->valor.val_str);
    }
}

OP op_simples(char op) {
    switch (op)
    {
    case '+':
        return op_add;
        break;
    case '-':
        return op_sub;
        break;
    case '*':
        return op_mult;
        break;
    case '/':
        return op_div;
        break;
    // case '<':
    //     return op_cmp_LT;
    //     break;
    // case '>':
    //     return op_cmp_GT;
    //     break;
    default:
        break;
    }
}

// OP op_composta(char *op) {
//     if (strcmp(op, "<=") == 0) {
//         return op_cmp_LE;
//     } else if (strcmp(op, ">=") == 0) {
//         return op_cmp_GE;
//     } else if (strcmp(op, "==") == 0) {
//         return op_cmp_EQ;
//     } else if (strcmp(op, "!=") == 0) {
//         return op_cmp_NE;
//     } else if (strcmp(op, "&&") == 0) {
//         return op_and;
//     } else if (strcmp(op, "||") == 0) {
//         return op_or;
//     }
// }

char *traduz_op(OP op) {
    switch (op)
    {
    case nop:
        return "nop";
        break;
    case op_add:
        return "add";
        break;
    case op_sub:
        return "sub";
        break;
    case op_mult:
        return "mult";
        break;
    case op_div:
        return "div";
        break;
    case op_addI:
        return "addI";
        break;
    case op_subI:
        return "subI";
        break;
    case op_rsubI:
        return "rsubI";
        break;
    case op_multI:
        return "multI";
        break;
    case op_divI:
        return "divI";
        break;
    case op_and:
        return "and";
        break;
    case op_andI:
        return "andI";
        break;
    case op_or:
        return "or";
        break;
    case op_orI:
        return "orI";
        break;
    case op_load:
        return "load";
        break;
    case op_loadI:
        return "loadI";
        break;
    case op_loadAI:
        return "loadAI";
        break;
    case op_loadAO:
        return "loadAO";
        break;
    case op_cload:
        return "cload";
        break;
    case op_cloadAI:
        return "cloadAI";
        break;
    case op_cloadAO:
        return "cloadAO";
        break;
    case op_store:
        return "store";
        break;
    case op_storeAI:
        return "storeAI";
        break;
    case op_storeAO:
        return "storeAO";
        break;
    case op_cstore:
        return "cstore";
        break;
    case op_cstoreAI:
        return "cstoreAI";
        break;
    case op_cstoreAO:
        return "cstoreAO";
        break;
    case op_i2i:
        return "i2i";
        break;
    case op_c2c:
        return "c2c";
        break;
    case op_c2i:
        return "c2i";
        break;
    case op_i2c:
        return "i2c";
        break;
    case op_cmp_LT:
        return "cmp_LT";
        break;
    case op_cmp_LE:
        return "cmp_LE";
        break;
    case op_cmp_EQ:
        return "cmp_EQ";
        break;
    case op_cmp_GE:
        return "cmp_GE";
        break;
    case op_cmp_GT:
        return "cmp_GT";
        break;
    case op_cmp_NE:
        return "cmp_NE";
        break;
    case op_cbr:
        return "cbr";
        break;
    case op_jump:
        return "jump";
        break;
    case op_jumpI:
        return "jumpI";
        break;
    default:
        return "halt";
        break;
    }
}