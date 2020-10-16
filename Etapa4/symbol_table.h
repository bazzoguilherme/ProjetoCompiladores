
#ifndef SYM_TABLE
#define SYM_TABLE

#include "ast.h"


// Definido por nós - valores a rever
typedef enum Type_Natureza {
    NAT_literal = 1,
    NAT_variavel,
    NAT_funcao,
    NAT_vetor,
} Type_Natureza;

int tamanho_byte(Type tipo_v);

// Linked-list para implementar tabela de simbolos
// Lista de valores dentro de tabela
struct elem_table {
    char *key;
    int localizacao;
    Type_Natureza natureza;
    Type tipo;
    int tamanho; // bytes
    struct elem_table *argumentos; // Funcao
    union val_lex dado;

    struct elem_table *next_elem;
};

struct stack_symbol_table {
    struct elem_table *topo;
    struct stack_symbol_table *down_table;
};


struct elem_table *new_elem_table();
struct elem_table *encontra_elemento_tabela(struct elem_table *tabela_atual, char *key);
struct elem_table *encontra_elemento_stack(struct stack_symbol_table *stack, char *key);
struct elem_table *recupera_ultimo_elemento(struct elem_table *tabela_atual);

struct elem_table *encontra_literal_tabela(struct elem_table *tabela_atual, char *key, Type tipo_lit);
struct elem_table *encontra_literal_stack(struct stack_symbol_table *stack, char *key, Type tipo_lit);

struct stack_symbol_table *new_stack();
struct stack_symbol_table *escopo_global();
struct stack_symbol_table *new_escopo(struct stack_symbol_table *stack_antigo);
struct stack_symbol_table *delete_stack();

struct stack_symbol_table *insere_simbolo(struct stack_symbol_table *stack, struct valor_lexico_t *symbol, Type_Natureza nat, Type tipo);
struct stack_symbol_table *insere_literal(struct stack_symbol_table *stack, struct valor_lexico_t *symbol, Type_Natureza nat, Type tipo);

struct stack_symbol_table *adiciona_lista_elem_comTipo(struct stack_symbol_table *stack, struct elem_table *lista_aux, Type tipo_);

struct elem_table *cria_simbolo_parcial(struct stack_symbol_table *stack, struct elem_table *lista_aux, struct valor_lexico_t *symbol, Type_Natureza nat, int tamanho_);

void adiciona_argumentos_funcao(struct stack_symbol_table *stack);

char *literal_key(struct valor_lexico_t* literal);


void print_stack_elements(struct stack_symbol_table *stack);
void print_table(struct elem_table *table);

void verifica_existencia(struct stack_symbol_table *stack, struct valor_lexico_t *dado);
void verif_utilizacao_identificador(struct stack_symbol_table *stack, struct valor_lexico_t *dado, Type_Natureza nat_utilizacao);

Type get_tipo_elemento_tabela(struct stack_symbol_table *stack, struct valor_lexico_t *dado);

char *nome_tipo(Type tipo);
char *nome_tipo_nat(Type_Natureza nat);

int uso_incorreto_erro(Type_Natureza nat);

Type define_tipo_expr(Type expr1, Type expr2, int linha);
void verifica_expr_un_tipo(Type tipo_exp, int linha);
void verifica_tipo_atribuicao(Type tipo_var, Type tipo_attrib, int linha);

void verifica_atrib_string(struct stack_symbol_table *stack, char *nome_var, struct AST *expr, int linha);
int calcula_tamanho_str_expr(struct stack_symbol_table *stack, struct AST *expr);

void verifica_chamada_funcao(struct stack_symbol_table *stack, struct valor_lexico_t *funcao, struct AST *parametros);

void verifica_tipo_input(struct stack_symbol_table *stack, struct valor_lexico_t *input_var);

int erro_semantico(int err);

void erro_declaracao(int err, char *var_nome, int linha_atual, int linha_decl);
void erro_nao_declaracao(int err, char *var_nome, int linha_atual);
void erro_uso_incorreto(int err, int linha_erro, char *nome_id, char *tipo_utilizacao, char *tipo_decl, int linha_decl);
void erro_converte_string_char(int err, int linha, Type tipo_atual, Type convertendo_para);
void erro_attrib_incompativel(int err, int linha, Type tipo_var, Type tipo_attrib);
void erro_tam_incompativel(int err, int linha, char *nome_var);
void erro_args_funcao_tipo(int err, int linha, char *nome_fun, int pos_erro);
void erro_args_funcao(int err, int linha, char *nome_fun, char *motivo);
void erro_input(int err, int linha, char *nome_var, Type tipo_var);

#endif