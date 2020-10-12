#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

struct symbol_table *new_table() {
    return (struct symbol_table *) malloc (sizeof(struct symbol_table));
}

struct stack_symbol_table *new_stack() {
    return (struct stack_symbol_table *) malloc(sizeof(struct stack_symbol_table));
}

struct stack_symbol_table *escopo_global() {
    struct stack_symbol_table *novo_escopo = new_stack();
    novo_escopo->topo = NULL;
    novo_escopo->down_table = NULL;
    return novo_escopo;
}

struct stack_symbol_table *new_escopo(struct stack_symbol_table *stack_antigo) {
    struct stack_symbol_table *novo_escopo = new_stack();
    novo_escopo->topo = NULL;
    novo_escopo->down_table = stack_antigo;
    return novo_escopo;
}

struct stack_symbol_table *delete_stack(struct stack_symbol_table *current_stack) {
    struct stack_symbol_table *novo_topo = current_stack->down_table;
    // Free old table
    return novo_topo;
}


void insere_simbolo(struct stack_symbol_table *stack, char *symbol){
    struct symbol_table *escopo_atual = stack->topo;

    if (escopo_atual == NULL) {
        escopo_atual = new_table();
        escopo_atual->key = strdup(symbol);
        stack->topo = escopo_atual;
    } else {
        // struct symbol_table *aux = escopo_atual;
        while(escopo_atual->next_elem != NULL) {
            escopo_atual = escopo_atual->next_elem;
        }
        struct symbol_table *aux = new_table();
        aux->key = strdup(symbol);
        escopo_atual->next_elem = aux;
    }
}