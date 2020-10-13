#include "ast.h"

#ifndef SYM_TABLE
#define SYM_TABLE


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
    // x argumentos; // Funcao
    struct valor_lexico_t *dados;

    struct elem_table *next_elem;
};

struct elem_table *new_elem_table();

struct stack_symbol_table {
    struct elem_table *topo;
    struct stack_symbol_table *down_table;
};

struct stack_symbol_table *new_stack();
struct stack_symbol_table *escopo_global();
struct stack_symbol_table *new_escopo(struct stack_symbol_table *stack_antigo);
struct stack_symbol_table *delete_stack();

struct stack_symbol_table *insere_simbolo(struct stack_symbol_table *stack, struct valor_lexico_t *symbol, Type_Natureza nat, Type tipo);

struct stack_symbol_table *adiciona_lista_elem_comTipo(struct stack_symbol_table *stack, struct elem_table *lista_aux, Type tipo_);

struct elem_table *cria_simbolo_parcial(struct elem_table *lista_aux, struct valor_lexico_t *symbol, Type_Natureza nat, int tamanho_);

void print_stack_elements(struct stack_symbol_table *stack);
void print_table(struct elem_table *table);

#endif