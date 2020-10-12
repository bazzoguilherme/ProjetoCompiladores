#include "ast.h"

#ifndef SYM_TABLE
#define SYM_TABLE

// Definido por nós - valores a rever
typedef enum Type_Natureza {
    literal = 1,
    variavel,
    funcao,
} Type_Natureza;

// Linked-list para implementar tabela de simbolos
// Lista de valores dentro de tabela
struct symbol_table {
    char *key;
    int localizacao;
    Type_Natureza natureza;
    Type tipo;
    int tamanho; // bytes
    // x argumentos; // Funcao
    struct valor_lexico_t *dados;

    struct symbol_table *next_elem;
};

struct symbol_table *new_table();

struct stack_symbol_table {
    struct symbol_table *topo;
    struct stack_symbol_table *down_table;
};

struct stack_symbol_table *new_stack();
struct stack_symbol_table *escopo_global();
struct stack_symbol_table *new_escopo(struct stack_symbol_table *stack_antigo);
struct stack_symbol_table *delete_stack();

void insere_simbolo(struct stack_symbol_table *stack, char *symbol);

#endif