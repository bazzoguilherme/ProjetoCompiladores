
#ifndef GERA_COD_H
#define GERA_COD_H

#include "ast.h"

char *gera_regis();
char *gera_label();

struct code *alloc_code();

char *int2str(struct valor_lexico_t *literal);

struct code *gera_code(OP op, char* arg1, char *arg2, char *dest);

OP op_operacao(struct valor_lexico_t *operacao);
OP op_simples(char op);
OP op_composta(char *op);

#endif