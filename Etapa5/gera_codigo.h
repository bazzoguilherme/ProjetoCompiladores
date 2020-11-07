
#ifndef GERA_COD_H
#define GERA_COD_H

#include "ast.h"

#define MAX_TAM_REGIS 12
#define MAX_TAM_LABEL 12
#define INT_SIZE_STR 12

#define NULL_LABEL -1
#define NULL_REGIS -1

#define RBSS_name "rbss"
#define RFP_name  "rfp"
#define RSP_name  "rsp"
#define RPC_name  "rpc"

#define RBSS -2
#define RFP  -3
#define RSP  -4
#define RPC  -5

#define REMENDO_T 1
#define REMENDO_F 0

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
struct code *gera_inicializacao(struct AST *init);

struct code *gera_expressao_bin(struct valor_lexico_t *operacao, struct AST *f1, struct AST *f2, int dest);
void gera_unario(struct AST *exp_un);
void gera_relop(struct AST *node);
void gera_and(struct AST *node);
void gera_or(struct AST *node);
void gera_not(struct AST *node);

struct code *gera_IF(struct AST *cond, struct AST *bloco, struct AST *else_opt);
struct code *gera_WHILE(struct AST *cond, struct AST *bloco);
struct code *gera_FOR(struct AST *atrib1, struct AST *cond, struct AST *atrib2, struct AST *bloco);

struct code *gera_halt();
struct code *instrucoes_iniciais();

struct code *rot();
struct l_remendo *lista_rem(int *pos_remendo);
void remenda(struct AST *ast, int t_f, int label);
void remenda_lista(struct l_remendo *lista_remendo, int label);
struct l_remendo *concat_remendo(struct l_remendo *r1, struct l_remendo *r2);

OP op_operacao(struct valor_lexico_t *operacao);
OP op_simples(char op);
OP op_composta(char *op);

void update_current_fun_name(struct valor_lexico_t *fun);

void print_code(struct code *codigo);

void traduz_op(struct code *c);
void traduz_regis(int reg_id);
void traduz_label(int label);
char *regis_especial(int reg);
void print_rr_d(struct code *c);
void print_rr_d_cmp(struct code *c);
void print_rc_d(struct code *c);
void print_r_d(struct code *c);
void print_c_d(struct code *c);
void print_r_dr(struct code *c);
void print_r_dc(struct code *c);
void print_r_LL(struct code *c);
void print_r(struct code *c);
void print_L(struct code *c);
void print_r_jmp(struct code *c);
void print_L_jmp(struct code *c);

void libera_codigo(struct code *c);
void libera_remendo(struct l_remendo *r);
#endif