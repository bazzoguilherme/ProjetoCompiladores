
#ifndef GERA_COD_H
#define GERA_COD_H

#include "ast.h"

#define MAX_TAM_REGIS 12
#define MAX_TAM_LABEL 12
#define INT_SIZE_STR 12

#define NULL_LABEL -1
#define NULL_REGIS -1

#define RBSS_name "bss"
#define RFP_name  "fp"
#define RSP_name  "sp"
#define RPC_name  "pc"

#define RBSS -2
#define RFP  -3
#define RSP  -4
#define RPC  -5

int gera_regis();
int gera_label();

struct code *alloc_code();

char *int2str(int val);
int lex2int(struct valor_lexico_t *literal);

struct code *concat(struct code *code1, struct code *code2, struct code *code3);
struct code *concat_codigos_ast(struct AST *ast1, struct AST *ast2, struct AST *ast3);

struct code *gera_code(int label, OP op, int arg1, int arg2, int dest1, int dest2, struct code *prox);

struct code *gera_loadI(OP op, struct valor_lexico_t *arg1, int dest);
struct code *gera_loadI_sinal(OP op, struct valor_lexico_t *sinal, struct valor_lexico_t *arg1, int dest);
struct code *gera_load_var(OP op, struct AST *ast, int dest);
struct code *gera_decl_funcao(struct valor_lexico_t *nome_funcao);
struct code *gera_args(struct AST *params);
struct code *gera_chamada_funcao(struct valor_lexico_t *fun_name, struct AST *params, int dest);
struct code *gera_retorno(struct AST *retorno);
struct code *retorno_funcao();
struct code *gera_atribuicao(struct AST *atrib, struct AST *expr);

struct code *gera_expressao_bin(struct valor_lexico_t *operacao, struct AST *f1, struct AST *f2, int dest);

OP op_operacao(struct valor_lexico_t *operacao);
OP op_simples(char op);
OP op_composta(char *op);

void print_code(struct code *codigo);

void traduz_op(struct code *c);

#endif