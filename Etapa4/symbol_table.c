#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "errors.h"

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


struct elem_table *encontra_elemento_tabela(struct elem_table *tabela_atual, char *key) {
    // enquanto n for nulo, n for a key que queremos COM nat = literal
    while ( tabela_atual != NULL && !(strcmp(tabela_atual->key, key) == 0 && NAT_literal != tabela_atual->natureza)) {
        tabela_atual = tabela_atual->next_elem;
    }
    // if(tabela_atual != NULL)
    // {
    //     printf("%s\n",tabela_atual->key);
    //     printf("Procurando %s e parou em %s com nat %d\n", key, tabela_atual->key, tabela_atual->natureza);
    // }
    // else{
    //     printf("Nao encontrei nenhum %s\n", key);
    // }
    return tabela_atual;
}


struct elem_table *encontra_elemento_stack(struct stack_symbol_table *stack, char *key) {
    struct elem_table *elemento = NULL;

    while ( stack != NULL && elemento == NULL ) {
        elemento = encontra_elemento_tabela(stack->topo, key);
        stack = stack->down_table;
    }
    
    return elemento;
}


struct elem_table *recupera_ultimo_elemento(struct elem_table *tabela_atual) {
    while(tabela_atual->next_elem != NULL) {
        tabela_atual = tabela_atual->next_elem;
    }
    return tabela_atual;
}


struct elem_table *encontra_literal_tabela(struct elem_table *tabela_atual, char *key, Type tipo_lit) {
    while ( tabela_atual != NULL && !(strcmp(tabela_atual->key, key) == 0 && tipo_lit != tabela_atual->natureza)) {
        tabela_atual = tabela_atual->next_elem;
    }
    return tabela_atual;
}

struct elem_table *encontra_literal_stack(struct stack_symbol_table *stack, char *key, Type tipo_lit) {
        struct elem_table *elemento = NULL;

    while ( stack != NULL && elemento == NULL ) {
        elemento = encontra_literal_tabela(stack->topo, key, tipo_lit);
        stack = stack->down_table;
    }
    
    return elemento;
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
    
    struct elem_table *elemento = NULL;
    
    if (stack != NULL && (elemento = encontra_elemento_tabela(stack->topo, symbol->valor.val_str)) != NULL) {
        erro_declaracao(ERR_DECLARED, symbol->valor.val_str, symbol->linha, elemento->localizacao);
    }

    elemento = stack->topo;

    if (elemento == NULL) {
        elemento = new_elem_table();
        elemento->key = strdup(symbol->valor.val_str);
        elemento->localizacao = symbol->linha;
        elemento->tamanho = tamanho_byte(tipo);
        elemento->natureza = nat;
        elemento->tipo = tipo;
        elemento->dado = symbol->valor;
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
        aux->dado = symbol->valor;
        elemento->next_elem = aux;
    }
    return stack;
}

struct stack_symbol_table *insere_literal(struct stack_symbol_table *stack, struct valor_lexico_t *literal, Type_Natureza nat, Type tipo) {
    if (stack == NULL) {
        stack = new_stack();
    }
    

    struct elem_table *elemento = stack->topo;
    struct elem_table *busca_lit = NULL;

    char *key_lit = literal_key(literal);

    if ((busca_lit = encontra_literal_tabela(stack->topo, key_lit, tipo)) != NULL) {
       // Atualiza elemento 
       busca_lit->localizacao = literal->linha;
    } else {

        if (elemento == NULL) {
            elemento = new_elem_table();
            elemento->key = key_lit;
            elemento->localizacao = literal->linha;
            elemento->tamanho = tamanho_byte(tipo);
            elemento->natureza = nat;
            elemento->tipo = tipo;
            elemento->dado = literal->valor;
            stack->topo = elemento;
        } else {
            while(elemento->next_elem != NULL) {
                elemento = elemento->next_elem;
            }
            struct elem_table *aux = new_elem_table();

            aux->key = key_lit;
            aux->localizacao = literal->linha;
            aux->tamanho = tamanho_byte(tipo);
            aux->natureza = nat;
            aux->tipo = tipo;
            aux->dado = literal->valor;
            elemento->next_elem = aux;
        }
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

struct elem_table *cria_simbolo_parcial(struct stack_symbol_table *stack, struct elem_table *lista_aux, struct valor_lexico_t *symbol, Type_Natureza nat, int tamanho_) {
    struct elem_table *aux;

    struct elem_table *elemento = NULL;
    
    if (stack != NULL && (elemento = encontra_elemento_tabela(stack->topo, symbol->valor.val_str)) != NULL) {
        erro_declaracao(ERR_DECLARED, symbol->valor.val_str, symbol->linha, elemento->localizacao);
    }

    if (lista_aux == NULL) {
        struct elem_table *new_elem = new_elem_table();
        new_elem->key = strdup(symbol->valor.val_str);
        new_elem->localizacao = symbol->linha;
        new_elem->natureza = nat;
        new_elem->tamanho = tamanho_;
        new_elem->dado = symbol->valor;
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
    new_elem->dado = symbol->valor;

    aux->next_elem = new_elem;

    return lista_aux;
}


void adiciona_argumentos_funcao(struct stack_symbol_table *stack) {
    struct elem_table *escopo_funcao = stack->topo;
    struct stack_symbol_table *escopo_anterior = stack->down_table;

    // encontrar o elemento da funcao
    struct elem_table *funcao = recupera_ultimo_elemento(escopo_anterior->topo);
    
    // passar os elementos
    struct elem_table *anterior = NULL;
    if (funcao != NULL) {
        while(escopo_funcao != NULL) {
            struct elem_table *novo_param = new_elem_table();
            novo_param->key = strdup(escopo_funcao->key);
            novo_param->tipo = escopo_funcao->tipo;

            if (anterior == NULL) {
                anterior = novo_param;
                funcao->argumentos = novo_param;
            } else {
                anterior->next_elem = novo_param;
                anterior = novo_param;
            }

            escopo_funcao = escopo_funcao->next_elem;
        }
    }
}


char *literal_key(struct valor_lexico_t* literal) {
    char *new_key;
    switch (literal->tipo)
    {
    case VAL_STRING:
        new_key = strdup(literal->valor.val_str);
        break;
    case VAL_CHAR:
        new_key = (char *) malloc (2);
        new_key[0] = literal->valor.val_char;
        new_key[1] = '\0';
        break;
    case VAL_BOOL:
        new_key = (char *) malloc (6);
        new_key = ((literal->valor.val_int == 1) ? "true" : "false");
        break;
    case VAL_INT:
        sprintf(new_key, "%d", literal->valor.val_int);
        break;
    case VAL_FLOAT:
        sprintf(new_key, "%g", literal->valor.val_float);
        break;
    default:
        break;
    }
    return new_key;
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


/* Verifica se variavei ja existe em utilizacao
 * e.g. int a <= b; (verifica b)
 * e.g. a = 10; (verifica a)
 */
void verifica_existencia(struct stack_symbol_table *stack, struct valor_lexico_t *dado) {
    if (encontra_elemento_stack(stack, dado->valor.val_str) == NULL) { // achou elemento - pode user
        erro_nao_declaracao(ERR_UNDECLARED, dado->valor.val_str, dado->linha);
    }
}

void verif_utilizacao_identificador(struct stack_symbol_table *stack, struct valor_lexico_t *dado, Type_Natureza nat_utilizacao) {
    struct elem_table *elemento = NULL;
    if ((elemento = encontra_elemento_stack(stack, dado->valor.val_str)) == NULL) { // achou elemento - pode user
        erro_nao_declaracao(ERR_UNDECLARED, dado->valor.val_str, dado->linha);
    }
    if (nat_utilizacao != elemento->natureza) {
        erro_uso_incorreto(uso_incorreto_erro(elemento->natureza), dado->linha, elemento->key, nome_tipo(nat_utilizacao), nome_tipo(elemento->natureza), elemento->localizacao);
    }
}


char *nome_tipo(Type_Natureza nat) {
    switch (nat) {
    case NAT_variavel:
        return "Variavel";
        break;
    case NAT_vetor:
        return "Vetor";
        break;
    case NAT_funcao:
        return "Funcao";
        break;
    
    default:
        break;
    }
    return "Undeclared Type";
}

int uso_incorreto_erro(Type_Natureza nat) {
    switch (nat) {
    case NAT_variavel:
        return ERR_VARIABLE;
        break;
    case NAT_vetor:
        return ERR_VECTOR;
        break;
    case NAT_funcao:
        return ERR_FUNCTION;
        break;
    
    default:
        break;
    }
}

int erro_semantico(int err) {
    printf("ERRO: %d\n", err);
    exit(err);
}

void erro_declaracao(int err, char *var_nome, int linha_atual, int linha_decl) {
    printf("In line %2d | Previous declaration of variable \"%s\" in line %d.\n", linha_atual, var_nome, linha_decl);
    exit(err);
}

void erro_nao_declaracao(int err, char *var_nome, int linha_atual) {
    printf("In line %2d | Variable \"%s\" was not declared.\n", linha_atual, var_nome);
    exit(err);
}

void erro_uso_incorreto(int err, int linha_erro, char *nome_id, char *tipo_utilizacao, char *tipo_decl, int linha_decl) {
    printf("In line %2d | Identifier \"%s\" been used as %s, but declared as %s in line %d.\n", linha_erro, nome_id, tipo_utilizacao, tipo_decl, linha_decl);
    exit(err);
}