#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "errors.h"

extern int get_line_number(void);

struct stack_symbol_table *stack = NULL;
struct elem_table *lista_aux = NULL;

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

struct elem_table *create_elem(char *key, int loc, Type_Natureza nat, Type tipo, int tam, union val_lex dado) {
    struct elem_table *elemento = new_elem_table();
    elemento->key = key;
    elemento->localizacao = loc;
    elemento->tamanho = tam;
    elemento->natureza = nat;
    elemento->tipo = tipo;
    elemento->dado = dado;
    return elemento;
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

struct elem_table *recupera_ultimo_elemento_global(struct stack_symbol_table *stack) {
    while(stack->down_table != NULL) {
        stack = stack->down_table;
    }
    return recupera_ultimo_elemento(stack->topo);
}


struct elem_table *encontra_literal_tabela(struct elem_table *tabela_atual, char *key, Type tipo_lit) {
    while ( tabela_atual != NULL && (strcmp(tabela_atual->key, key) != 0 || tipo_lit != tabela_atual->tipo)) {
        tabela_atual = tabela_atual->next_elem;
    }
    // if(tabela_atual != NULL)
    // {
    //     printf("%s\n",tabela_atual->key);
    //     printf("Procurando %s e parou em %s com tipo %d\n", key, tabela_atual->key, tabela_atual->tipo);
    // }
    // else{
    //     printf("Nao encontrei nenhum %s\n", key);
    // }
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

void new_escopo() {
    struct stack_symbol_table *novo_escopo = new_stack();
    novo_escopo->topo = NULL;
    novo_escopo->down_table = stack; // Coloca stack como escopo antigo
    stack = novo_escopo; // Atualiza stack
}

void delete_escopo() {
    struct stack_symbol_table *novo_topo = stack->down_table;
    // Free old table
    stack = novo_topo;
}


void insere_simbolo(struct valor_lexico_t *symbol, Type_Natureza nat, Type tipo) {
    if (stack == NULL) {
        stack = new_stack();
    }
    
    struct elem_table *elemento = NULL;
    
    if (stack != NULL && (elemento = encontra_elemento_tabela(stack->topo, symbol->valor.val_str)) != NULL) {
        erro_declaracao(ERR_DECLARED, symbol->valor.val_str, elemento->localizacao);
    }

    elemento = stack->topo;

    if (elemento == NULL) {
        elemento = create_elem(strdup(symbol->valor.val_str), 
                                symbol->linha, nat, tipo, tamanho_byte(tipo), 
                                symbol->valor);

        stack->topo = elemento;
    } else {
        while(elemento->next_elem != NULL) {
            elemento = elemento->next_elem;
        }
        struct elem_table *aux = create_elem(strdup(symbol->valor.val_str), 
                                    symbol->linha, nat, tipo, tamanho_byte(tipo), 
                                    symbol->valor);
        elemento->next_elem = aux;
    }
}

void insere_literal(struct valor_lexico_t *literal, Type_Natureza nat, Type tipo) {
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
            elemento = create_elem(key_lit, literal->linha, nat, tipo, 
                                tamanho_byte(tipo) * ((tipo == TYPE_STRING) ? strlen(literal->valor.val_str) : 1), // Define tamanho 
                                literal->valor);
            stack->topo = elemento;
        } else {
            while(elemento->next_elem != NULL) {
                elemento = elemento->next_elem;
            }
            struct elem_table *aux = create_elem(key_lit, literal->linha, nat, tipo, 
                                tamanho_byte(tipo) * ((tipo == TYPE_STRING) ? strlen(literal->valor.val_str) : 1), // Define tamanho 
                                literal->valor);
            elemento->next_elem = aux;
        }
    }
}

void adiciona_lista_elem_comTipo(Type tipo_) {

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
        lista_aux->tamanho *= ((lista_aux->natureza == NAT_variavel && tipo_ == TYPE_STRING) ? -1 : tamanho_byte(tipo_));
        lista_aux = lista_aux->next_elem;
    }
    lista_aux = NULL; // Limpa lista
}

void cria_simbolo_parcial(struct valor_lexico_t *symbol, Type_Natureza nat, int tamanho_) {
    struct elem_table *aux;

    struct elem_table *elemento = NULL;

    if (stack != NULL && (elemento = encontra_elemento_tabela(stack->topo, symbol->valor.val_str)) != NULL) {
        erro_declaracao(ERR_DECLARED, symbol->valor.val_str, elemento->localizacao);
    } 
    if (lista_aux != NULL && encontra_elemento_tabela(lista_aux, symbol->valor.val_str) != NULL) {
        erro_declaracao(ERR_DECLARED, symbol->valor.val_str, symbol->linha);
    }

    if (lista_aux == NULL) {
        struct elem_table *new_elem = create_elem(strdup(symbol->valor.val_str), 
                                symbol->linha, nat, TYPE_NO_VAL, tamanho_, 
                                symbol->valor);
        lista_aux = new_elem;
        return;
    } else {
        aux = lista_aux;
        while(aux->next_elem != NULL) {
            aux = aux->next_elem;
        }
    }

    struct elem_table *new_elem = create_elem(strdup(symbol->valor.val_str), 
                                symbol->linha, nat, TYPE_NO_VAL, tamanho_, 
                                symbol->valor);

    aux->next_elem = new_elem;

}


void adiciona_argumentos_funcao() {
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
        new_key = (char *) malloc (12);
        snprintf(new_key, 12, "%d", literal->valor.val_int);
        break;
    case VAL_FLOAT:
        new_key = (char *) malloc (30);
        snprintf(new_key, 30, "%.6f", literal->valor.val_float);
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
void verifica_existencia(struct valor_lexico_t *dado) {
    if (encontra_elemento_stack(stack, dado->valor.val_str) == NULL) { // achou elemento - pode user
        erro_nao_declaracao(ERR_UNDECLARED, dado->valor.val_str);
    }
}

void verif_utilizacao_identificador(struct valor_lexico_t *dado, Type_Natureza nat_utilizacao) {
    struct elem_table *elemento = NULL;
    if ((elemento = encontra_elemento_stack(stack, dado->valor.val_str)) == NULL) { // achou elemento - pode user
        erro_nao_declaracao(ERR_UNDECLARED, dado->valor.val_str);
    }
    if (nat_utilizacao != elemento->natureza) {
        erro_uso_incorreto(uso_incorreto_erro(elemento->natureza), elemento->key, nome_tipo_nat(nat_utilizacao), nome_tipo_nat(elemento->natureza), elemento->localizacao);
    }
}

Type get_tipo_elemento(struct valor_lexico_t *dado) {
    struct elem_table *elemento = NULL;
    if ((elemento = encontra_elemento_stack(stack, dado->valor.val_str)) == NULL) { // achou elemento - pode user
        erro_nao_declaracao(ERR_UNDECLARED, dado->valor.val_str);
    }
    return elemento->tipo;
}

char *nome_tipo(Type tipo) {
    switch (tipo) {
    case TYPE_INT:
        return "int";
        break;
    case TYPE_FLOAT:
        return "float";
        break;
    case TYPE_CHAR:
        return "char";
        break;
    case TYPE_STRING:
        return "string";
        break;
    case TYPE_BOOL:
        return "bool";
        break;
    
    default:
        break;
    }
    return "? Type";
}

char *nome_tipo_nat(Type_Natureza nat) {
    switch (nat) {
    case NAT_variavel:
        return "Variable";
        break;
    case NAT_vetor:
        return "Vector";
        break;
    case NAT_funcao:
        return "Function";
        break;
    
    default:
        break;
    }
    return "Undeclared Type";
}

int tipos_compativeis(Type tipo1, Type tipo2) {
    if (tipo1 == tipo2) {
        return tipo1;
    } else if (tipo1 == TYPE_STRING || tipo1 == TYPE_CHAR ||
                tipo2 == TYPE_STRING || tipo2 == TYPE_CHAR) {
        return 0;
    } else if (tipo1 == TYPE_FLOAT || tipo2 == TYPE_FLOAT) {
        return TYPE_FLOAT;
    } else if (tipo1 == TYPE_INT || tipo2 == TYPE_INT) {
        return TYPE_INT;
    } else {
        return TYPE_BOOL;
    } 
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

Type define_tipo_expr(Type expr1, Type expr2) {
    if (expr1 == expr2) { // int-int, float-float, bool-bool (aceita char-char e string-string)
        return expr1;
    } else if (expr1 == TYPE_STRING || expr2 == TYPE_STRING) {
        erro_converte_string_char(ERR_STRING_TO_X, TYPE_STRING, (expr1 != TYPE_STRING ? expr1 : expr2));
    } else if (expr1 == TYPE_CHAR || expr2 == TYPE_CHAR) {
        erro_converte_string_char(ERR_CHAR_TO_X, TYPE_CHAR, (expr1 != TYPE_CHAR ? expr1 : expr2));
    } else if (expr1 == TYPE_FLOAT || expr2 == TYPE_FLOAT) {
        return TYPE_FLOAT;
    } else if (expr1 == TYPE_INT || expr2 == TYPE_INT) {
        return TYPE_INT;
    } else {
        return TYPE_BOOL;
    }
}

void verifica_tipo_atribuicao(Type tipo_var, Type tipo_attrib) {
    if (tipo_var == tipo_attrib) { // Após isso, nodo fica do tipo da var que está recendo
        return;
    }
    // Erro apenas se tiver que fazer conversao para char ou string
    //  demais tipos (int, float, bool) permitem conversao
    if (tipo_var == TYPE_CHAR || tipo_var == TYPE_STRING ||
        tipo_attrib == TYPE_CHAR || tipo_attrib == TYPE_STRING) { 
        erro_attrib_incompativel(ERR_WRONG_TYPE, tipo_var, tipo_attrib);
    }
}

void verifica_atrib_string(char *nome_var, struct AST *expr) {
    struct elem_table *var = encontra_elemento_stack(stack, nome_var);
    int tam_inserindo = calcula_tamanho_str_expr(stack, expr);
    if (var->tamanho == -1){
        var->tamanho = tam_inserindo;
    } else if (tam_inserindo > var->tamanho) {
        erro_tam_incompativel(ERR_STRING_SIZE, nome_var);
    }
}

int calcula_tamanho_str_expr(struct stack_symbol_table *stack, struct AST *expr){
    if (expr == NULL) {
        return 0;
    }
    struct elem_table *elem = NULL;
    if (expr->tipo_exp == AST_LIT) {
        return strlen(expr->valor_lexico->valor.val_str);
    } else if (expr->tipo_exp == AST_ID) {
        elem = encontra_elemento_stack(stack, expr->valor_lexico->valor.val_str);
        return ( (elem->tamanho != -1) ? elem->tamanho : 0 );
    } else if (expr->tipo_exp == AST_OP_BIN) {
        return calcula_tamanho_str_expr(stack, expr->children[0]) + calcula_tamanho_str_expr(stack, expr->children[1]);
    }
}

void verifica_chamada_funcao(struct valor_lexico_t *funcao, struct AST *parametros) {
    struct elem_table *fun_args = encontra_elemento_stack(stack, funcao->valor.val_str)->argumentos;
    int pos_arg = 1;
    while(fun_args != NULL && parametros != NULL) {
        if (fun_args->tipo != parametros->tipo) { // WRONG TYPE
            erro_args_funcao_tipo(ERR_WRONG_TYPE_ARGS, funcao->valor.val_str, pos_arg);
        }
        fun_args = fun_args->next_elem;
        parametros = parametros->prox;
        pos_arg++;
    }
    if (fun_args != NULL) { // MISSING
        erro_args_funcao(ERR_MISSING_ARGS, funcao->valor.val_str, "missing arguments");
    } else if (parametros != NULL) { // EXCESS
        erro_args_funcao(ERR_MISSING_ARGS, funcao->valor.val_str, "excess of arguments");
    }
}

void verifica_tipo_input(struct valor_lexico_t *input_var) {
    struct elem_table *elemento = encontra_elemento_stack(stack, input_var->valor.val_str);
    if (elemento->tipo != TYPE_INT && elemento->tipo != TYPE_FLOAT) {
        erro_input(ERR_WRONG_PAR_INPUT, input_var->valor.val_str, elemento->tipo);
    }
}

void verifica_tipo_output(struct valor_lexico_t *output_var) {
    struct elem_table *elemento = encontra_elemento_stack(stack, output_var->valor.val_str);
    if (elemento->tipo != TYPE_INT && elemento->tipo != TYPE_FLOAT) {
        erro_output(ERR_WRONG_PAR_OUTPUT, output_var->valor.val_str, elemento->tipo);
    }
}

void verifica_tipo_output_lit(struct AST *lit) {
    if (lit->tipo != TYPE_INT && lit->tipo != TYPE_FLOAT) {
        erro_output_lit(ERR_WRONG_PAR_OUTPUT, lit->tipo);
    }
}

void verifica_shift(struct AST *lit) {
    if (lit->tipo_exp == AST_OP_UN) {
        lit = lit->children[0];
    }
    // lit como literal apenas
    if (lit->valor_lexico->valor.val_int > 16) {
        erro_shift(ERR_WRONG_PAR_SHIFT);
    }
}

void verifica_retorno_funcao(struct AST *expr_retorno) {
    struct elem_table *elemento_fun = recupera_ultimo_elemento_global(stack);
    if (!tipos_compativeis(elemento_fun->tipo, expr_retorno->tipo)) {
        erro_return(ERR_WRONG_PAR_RETURN, elemento_fun->dado.val_str, elemento_fun->tipo, expr_retorno->tipo);
    }
}

int erro_semantico(int err) {
    printf("ERRO: %d\n", err);
    exit(err);
}

void erro_declaracao(int err, char *var_nome, int linha_decl) {
    printf("In line %2d | Previous declaration of identifier \"%s\" in line %d.\n", get_line_number(), var_nome, linha_decl);
    exit(err);
}

void erro_nao_declaracao(int err, char *var_nome) {
    printf("In line %2d | Variable \"%s\" was not declared.\n", get_line_number(), var_nome);
    exit(err);
}

void erro_uso_incorreto(int err, char *nome_id, char *tipo_utilizacao, char *tipo_decl, int linha_decl) {
    printf("In line %2d | Identifier \"%s\" used as %s, but declared as %s in line %d.\n", get_line_number(), nome_id, tipo_utilizacao, tipo_decl, linha_decl);
    exit(err);
}

void erro_converte_string_char(int err, Type tipo_atual, Type convertendo_para) {
    printf("In line %2d | Error in conversion of type %s to type %s.\n", get_line_number(), nome_tipo(tipo_atual), nome_tipo(convertendo_para));
    exit(err);
}

void erro_attrib_incompativel(int err, Type tipo_var, Type tipo_attrib) {
    printf("In line %2d | Error to assign %s to variable of type %s.\n", get_line_number(), nome_tipo(tipo_attrib), nome_tipo(tipo_var));
    exit(err);
}

void erro_tam_incompativel(int err, char *nome_var) {
    printf("In line %2d | Error to assign string literal to variable %s due to lack of space.\n", get_line_number(), nome_var);
    exit(err);
}

void erro_args_funcao_tipo(int err, char *nome_fun, int pos_erro) {
    printf("In line %2d | Error in function call of \"%s\" due to type error in argumento %d.\n", get_line_number(), nome_fun, pos_erro);
    exit(err);
}

void erro_args_funcao(int err, char *nome_fun, char *motivo) {
    printf("In line %2d | Error in function call of \"%s\" due to %s.\n", get_line_number(), nome_fun, motivo);
    exit(err);
}

void erro_input(int err, char *nome_var, Type tipo_var) {
    printf("In line %2d | Error in input statement. Expected variable of type int or float, but variable \"%s\" of type %s found.\n", get_line_number(), nome_var, nome_tipo(tipo_var));
    exit(err);
}

void erro_output(int err, char *nome_var, Type tipo_var) {
    printf("In line %2d | Error in output statement. Expected variable of type int or float, but variable \"%s\" of type %s found.\n", get_line_number(), nome_var, nome_tipo(tipo_var));
    exit(err);
}

void erro_output_lit(int err, Type tipo_lit) {
    printf("In line %2d | Error in output statement. Expected literal of type int or float, but found type %s.\n", get_line_number(), nome_tipo(tipo_lit));
    exit(err);
}

void erro_shift(int err) {
    printf("In line %2d | Error in value used to shift variable, please use positive integer less or equal to 16.\n", get_line_number());
    exit(err);
}

void erro_return(int err, char *fun_name, Type tipo_fun, Type tipo_ret) {
    printf("In line %2d | Error in return type of function \"%s\". Function declared of type %s but returning type %s.\n", get_line_number(), fun_name, nome_tipo(tipo_fun), nome_tipo(tipo_ret));
    exit(err);
}