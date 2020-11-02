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
