#include <stdio.h>
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

struct elem_table *new_elem_table() {
    return (struct elem_table *) malloc (sizeof(struct elem_table));
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


struct stack_symbol_table *insere_simbolo(struct stack_symbol_table *stack, struct valor_lexico_t *symbol, Type_Natureza nat, Type tipo) {
    if (stack == NULL) {
        stack = new_stack();
    }
    
    struct elem_table *elemento = stack->topo;

    if (elemento == NULL) {
        elemento = new_elem_table();
        elemento->key = strdup(symbol->valor.val_str);
        elemento->localizacao = symbol->linha;
        elemento->tamanho = tamanho_byte(tipo);
        elemento->natureza = nat;
        elemento->tipo = tipo;
        stack->topo = elemento;
    } else {
        while(elemento->next_elem != NULL) {
            elemento = elemento->next_elem;
        }
        struct elem_table *aux = new_elem_table();

        aux->key = strdup(symbol->valor.val_str);
        aux->localizacao = symbol->linha;
        aux->tamanho = tamanho_byte(tipo);
        aux->natureza = nat;
        aux->tipo = tipo;
        elemento->next_elem = aux;
    }
    return stack;
}

struct stack_symbol_table *adiciona_lista_elem_comTipo(struct stack_symbol_table *stack, struct elem_table *lista_aux, Type tipo_) {

    if (stack == NULL) {
        stack = escopo_global();
    }

    struct elem_table *aux = stack->topo;
    if (aux == NULL) {
        stack->topo = lista_aux;
    } else {
        while(aux->next_elem != NULL) {
            aux = aux->next_elem;
        }
        aux->next_elem = lista_aux;
    }

    while (lista_aux != NULL) {
        lista_aux->tipo = tipo_;
        lista_aux->tamanho *= tamanho_byte(tipo_);
        lista_aux = lista_aux->next_elem;
    }

    return stack;
}

struct elem_table *cria_simbolo_parcial(struct elem_table *lista_aux, struct valor_lexico_t *symbol, Type_Natureza nat, int tamanho_) {
    struct elem_table *aux;

    if (lista_aux == NULL) {
        struct elem_table *new_elem = new_elem_table();
        new_elem->key = strdup(symbol->valor.val_str);
        new_elem->localizacao = symbol->linha;
        new_elem->natureza = nat;
        new_elem->tamanho = tamanho_;
        return new_elem;
    } else {
        aux = lista_aux;
        while(aux->next_elem != NULL) {
            aux = aux->next_elem;
        }
    }

    struct elem_table *new_elem = new_elem_table();
    new_elem->key = strdup(symbol->valor.val_str);
    new_elem->localizacao = symbol->linha;
    new_elem->natureza = nat;
    new_elem->tamanho = tamanho_;

    aux->next_elem = new_elem;

    return lista_aux;
}







void print_stack_elements(struct stack_symbol_table *stack) {
    printf("Stack - print\n");
    print_table(stack->topo);
    if (stack->down_table != NULL)
        print_stack_elements(stack->down_table);
}

void print_table(struct elem_table *table) {
    while(table != NULL) {
        printf("Elemento: %s\n", table->key);
        table = table->next_elem;
    }
}