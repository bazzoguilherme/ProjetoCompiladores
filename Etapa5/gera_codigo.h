
#ifndef GERA_COD_H
#define GERA_COD_H

#include "ast.h"

char *gera_regis();
char *gera_label();

struct code *alloc_code();

char *int2str(int val);
char *lex2str(struct valor_lexico_t *literal);

struct code *concat(struct code *code1, struct code *code2, struct code *code3);
struct code *concat_codigos_ast(struct AST *ast1, struct AST *ast2, struct AST *ast3);

struct code *gera_code(char *label, OP op, char* arg1, char *arg2, char *dest1, char* dest2, struct code *prox);

struct code *gera_loadI(OP op, struct valor_lexico_t *arg1, char *dest);
struct code *gera_loadI_sinal(OP op, struct valor_lexico_t *sinal, struct valor_lexico_t *arg1, char *dest);
struct code *gera_decl_funcao(struct valor_lexico_t *nome_funcao);

OP op_operacao(struct valor_lexico_t *operacao);
OP op_simples(char op);
OP op_composta(char *op);


char *traduz_op(OP op);

#endif