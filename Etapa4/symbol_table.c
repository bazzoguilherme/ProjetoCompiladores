#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

int tamanho_byte(Type tipo_v){
    switch (tipo_v)
    {
    case TYPE_INT:
        return 4;
        break;
    case TYPE_FLOAT:
        return 8;
        break;
    case TYPE_CHAR:
    case TYPE_STRING:
        return 1;
        break;
    case TYPE_BOOL:
        return 1;
        break;
    
    default:
        break;
    }
}

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


struct stack_symbol_table *insere_simbolo(struct stack_symbol_table *stack, struct valor_lexico_t *symbol, Type_Natureza nat, Type tipo, struct AST *tamanho){
    struct symbol_table *escopo_atual = stack->topo;

    if (escopo_atual == NULL) {
        escopo_atual = new_table();
        escopo_atual->key = symbol->valor.val_str;
        escopo_atual->localizacao = symbol->linha;
        escopo_atual->natureza = nat;
        stack->topo = escopo_atual;
    } else {
        while(escopo_atual->next_elem != NULL) {
            escopo_atual = escopo_atual->next_elem;
        }
        struct symbol_table *aux = new_table();

        aux->tamanho = return_size(tamanho) * tamanho_byte(tipo);
        aux->key = symbol->valor.val_str;
        aux->localizacao = symbol->linha;
        aux->natureza = nat;
        escopo_atual->next_elem = aux;
    }
}