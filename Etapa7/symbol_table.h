
#ifndef SYM_TABLE
#define SYM_TABLE

#include "ast.h"

/*
 * Para Funções
 * em 0 : Posição de retorno de funcao
 * em 4 : Retorno da funcao
 * em 8 : rfp \ Realizar o 'pop' da pilha
 * em 12: rsp /
 *  -- Motivo: facilidade no controle
 */
#define INIT_ESC_NOMEADO 16
#define LOCAL_RETORNO 0
#define RETORNO_FUNC 4
#define DESL_RFP 8
#define DESL_RSP 12

#define TRUE 1
#define FALSE 0

#define GLOBAL 1
#define LOCAL 0

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
    int deslocamento;
    int label;

    struct elem_table *next_elem;
};

struct stack_symbol_table {
    struct elem_table *topo;
    int deslocamento;
    int nomeado;
    struct stack_symbol_table *down_table;
};


struct elem_table *new_elem_table();
struct elem_table *create_elem(char *key, int loc, Type_Natureza nat, Type tipo, int tam, union val_lex dado);

struct elem_table *encontra_elemento_tabela(struct elem_table *tabela_atual, char *key);
struct elem_table *encontra_elemento_tabela_tipoNat(struct elem_table *tabela_atual, char *key, Type_Natureza nat);
struct elem_table *encontra_elemento_stack(struct stack_symbol_table *stack, char *key);
struct elem_table *encontra_elemento_stack_tipoNat(struct stack_symbol_table *stack, char *key, Type_Natureza nat);
struct elem_table *recupera_ultimo_elemento(struct elem_table *tabela_atual);
struct elem_table *recupera_ultimo_elemento_global(struct stack_symbol_table *stack);

struct elem_table *encontra_literal_tabela(struct elem_table *tabela_atual, char *key, Type tipo_lit);
struct elem_table *encontra_literal_stack(struct stack_symbol_table *stack, char *key, Type tipo_lit);

struct stack_symbol_table *new_stack();
struct stack_symbol_table *escopo_global();
void new_escopo();
void new_escopo_funcao();
void delete_escopo();
void free_table(struct elem_table *table);
void free_stack(struct stack_symbol_table *stack);

struct stack_symbol_table *get_escopo_global();

union val_lex dupl_union(union val_lex dado, Type_Natureza nat, Type tipo);

void insere_simbolo(struct valor_lexico_t *symbol, Type_Natureza nat, Type tipo);
void insere_literal(struct valor_lexico_t *symbol, Type_Natureza nat, Type tipo);

void adiciona_lista_elem_comTipo(Type tipo_);

void cria_simbolo_parcial(struct valor_lexico_t *symbol, Type_Natureza nat, int tamanho_);

void adiciona_argumentos_funcao();

char *literal_key(struct valor_lexico_t* literal);


void print_stack_elements(struct stack_symbol_table *stack);
void print_table(struct elem_table *table);

void verifica_existencia(struct valor_lexico_t *dado);
void verif_utilizacao_identificador(struct valor_lexico_t *dado, Type_Natureza nat_utilizacao);

Type get_tipo_elemento(struct valor_lexico_t *dado);

char *nome_tipo(Type tipo);
char *nome_tipo_nat(Type_Natureza nat);

int tipos_compativeis(Type tipo1, Type tipo2);

int uso_incorreto_erro(Type_Natureza nat);

Type define_tipo_expr(Type expr1, Type expr2);
Type tipo_op_bin(struct valor_lexico_t *val_lex, Type tipo_nodos);
void verifica_bool_condition(Type expr2, char *lugar, int linha);

void verifica_tipo_atribuicao(Type tipo_var, Type tipo_attrib);

void verifica_atrib_string(struct AST *ast, struct AST *expr);
int calcula_tamanho_str_expr(struct stack_symbol_table *stack, struct AST *expr);

void verifica_chamada_funcao(struct valor_lexico_t *funcao, struct AST *parametros);

void verifica_tipo_input(struct valor_lexico_t *input_var);
void verifica_tipo_output(struct valor_lexico_t *output_var);
void verifica_tipo_output_lit(struct AST *lit);

void verifica_shift(struct AST *lit);

void verifica_retorno_funcao(struct AST *expr_retorno);

int deslocamento_symbol(char *symbol, int *escopo);
int deslocamento_funcao_atual();
int label_funcao(char *fun_name);
char *retorna_label_fun(int labl);
char *var_globl_desloc(int desloc);
char *get_function_name(int label_fun);
struct elem_table *get_elem_function(int label);

int erro_semantico(int err);

void erro_declaracao(int err, char *var_nome, int linha_decl);
void erro_declaracao_function(int err, char *var_nome, int linha_decl);
void erro_nao_declaracao(int err, char *var_nome);
void erro_uso_incorreto(int err, char *nome_id, char *tipo_utilizacao, char *tipo_decl, int linha_decl);
void erro_converte_string_char(int err, Type tipo_atual, Type convertendo_para);
void erro_str_ch_paraBool_controle_fluxo(int err, int linha, Type tipo_atual, char *operacao);
void erro_attrib_incompativel(int err, Type tipo_var, Type tipo_attrib);
void erro_tam_incompativel(int err, char *nome_var);
void erro_args_funcao_tipo(int err, char *nome_fun, int pos_erro, Type tipo_passado, Type tipo_esperado);
void erro_args_funcao(int err, char *nome_fun, char *motivo);
void erro_input(int err, char *nome_var, Type tipo_var);
void erro_output(int err, char *nome_var, Type tipo_var);
void erro_output_lit(int err, Type tipo_var);
void erro_shift(int err, int err_int);
void erro_return(int err, char *fun_name, Type tipo_fun, Type tipo_ret);

void exp_table();
void print_stack(struct stack_symbol_table *stack);
void print_table(struct elem_table *table);

#endif