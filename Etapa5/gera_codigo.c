#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ast.h"
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

char *int2str(struct valor_lexico_t *literal) {
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

struct code *gera_code(OP op, char* arg1, char *arg2, char *dest) {
    struct code *codigo = alloc_code();
    codigo->label = NULL;
    codigo->operation = op;
    codigo->arg1 = arg1;
    codigo->arg2 = arg2;
    codigo->dest = dest;
    codigo->prox = NULL;
    return codigo;
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