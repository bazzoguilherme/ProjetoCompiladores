#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

#include "symbol_table.h"

extern struct stack_symbol_table *stack_table;

struct valor_lexico_t *lex_int(int int_value, Tipo_val_lex tipo_tk, int linha) {
    struct valor_lexico_t *val_lex = (struct valor_lexico_t*) malloc (sizeof(struct valor_lexico_t));
    val_lex->valor.val_int = int_value;
    val_lex->tipo = tipo_tk;
    val_lex->linha = linha;
    return val_lex;
}

struct valor_lexico_t *lex_bool(int bool_val, Tipo_val_lex tipo_tk, int linha) {
    return lex_int(bool_val, tipo_tk, linha);
}

struct valor_lexico_t *lex_float(float float_val, Tipo_val_lex tipo_tk, int linha) {
    struct valor_lexico_t *val_lex = (struct valor_lexico_t*) malloc (sizeof(struct valor_lexico_t));
    val_lex->valor.val_float = float_val;
    val_lex->tipo = tipo_tk;
    val_lex->linha = linha;
    return val_lex;
}

struct valor_lexico_t *lex_char(char char_val, Tipo_val_lex tipo_tk, int linha) {
    struct valor_lexico_t *val_lex = (struct valor_lexico_t*) malloc (sizeof(struct valor_lexico_t));
    val_lex->valor.val_char = char_val;
    val_lex->tipo = tipo_tk;
    val_lex->linha = linha;
    return val_lex;
}

struct valor_lexico_t *lex_id_comp(char *val, Tipo_val_lex tipo_tk, int linha) {
    struct valor_lexico_t *val_lex = (struct valor_lexico_t*) malloc (sizeof(struct valor_lexico_t));
    val_lex->valor.val_str = strdup(val);
    val_lex->tipo = tipo_tk;
    val_lex->linha = linha;
    return val_lex;
}

struct valor_lexico_t *lex_str(char *str_val, Tipo_val_lex tipo_tk, int linha) {
    struct valor_lexico_t *val_lex = (struct valor_lexico_t*) malloc (sizeof(struct valor_lexico_t));
    val_lex->valor.val_str = strdup(str_val);
    remove_quotes(val_lex->valor.val_str);
    val_lex->tipo = tipo_tk;
    val_lex->linha = linha;
    return val_lex;
}

struct valor_lexico_t *lex_id(char *id_val, Tipo_val_lex tipo_tk, int linha) {
    return lex_id_comp(id_val, tipo_tk, linha);
}

struct valor_lexico_t *lex_composto(char *composto_val, Tipo_val_lex tipo_tk, int linha) {
    return lex_id_comp(composto_val, tipo_tk, linha);
}

struct valor_lexico_t *lex_especial(char esp_val, Tipo_val_lex tipo_tk, int linha) {
    return lex_char(esp_val, tipo_tk, linha);
}

void remove_quotes(char *st) {
    int len = strlen(st);
    int i, pos = 0;
    for (i = 0; i < len; i++) {
        if (i != 0 && i != len-1)
            st[pos++] = st[i];
    }
    st[pos] = '\0';
}

struct AST *create_AST(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, Type tipo_nodo, struct AST *f1, struct AST *f2, struct AST *f3, struct AST *f4, struct AST *next_node) {
    struct AST *new_node = (struct AST*) malloc (sizeof(struct AST));
    new_node->tipo_exp = ast_type_exp;
    new_node->valor_lexico = val_lex;
    new_node->tipo = tipo_nodo;
    new_node->children[0] = f1;
    new_node->children[1] = f2;
    new_node->children[2] = f3;
    new_node->children[3] = f4;
    new_node->prox = next_node;
    return new_node;
}

struct AST *create_LIT(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex) {
    return create_AST(ast_type_exp, val_lex, tipo_de_valor(val_lex), NULL, NULL, NULL, NULL, NULL);
}

struct AST *create_ID(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex) {
    return create_AST(ast_type_exp, val_lex, get_tipo_elemento_tabela(stack_table, val_lex), NULL, NULL, NULL, NULL, NULL);
}

struct AST *create_NODE(struct AST *f1, struct AST *next) {
    if (f1 == NULL){
        return next;
    } else if (f1->tipo_exp == AST_DECL_ASSIGN) {
        struct AST *aux = f1;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = next;
    } else {
        f1->prox = next;
    }
    return f1;
}

struct AST *create_FUNCAO(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, struct AST *f1) {
    return create_AST(ast_type_exp, val_lex, get_tipo_elemento_tabela(stack_table, val_lex), f1, NULL, NULL, NULL, NULL);
}

// struct AST *create_EXPRESSAO(Type_Exp ast_type_exp, struct AST *f1, struct AST *f2) {
//     return create_AST(ast_type_exp, NULL, TYPE_NO_VAL, f1, f2, NULL, NULL, NULL);
// }

struct AST *create_EXPRESSAO_BIN(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, struct AST *f1, struct AST *f2) {
    return create_AST(ast_type_exp, val_lex, define_tipo_expr(f1->tipo, f2->tipo, val_lex->linha), f1, f2, NULL, NULL, NULL);
}

struct AST *create_EXPRESSAO_UN(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, struct AST *f1) {
    return create_AST(ast_type_exp, val_lex, f1->tipo, f1, NULL, NULL, NULL, NULL);
}

struct AST *create_EXPRESSAO_UN_LIT(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, struct valor_lexico_t *lit) {
    struct AST *f1 = create_LIT(AST_LIT, lit);
    if (val_lex != NULL) {
        return create_AST(ast_type_exp, val_lex, f1->tipo, f1, NULL, NULL, NULL, NULL);
    }
    return f1;
}

struct AST *create_IO(Type_Exp ast_type_exp, struct AST *f1) {
    return create_AST(ast_type_exp, NULL, f1->tipo, f1, NULL, NULL, NULL, NULL);
}

struct AST *create_IO_id(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex) {
    struct AST *f1 = create_LIT(AST_LIT, val_lex);
    return create_AST(ast_type_exp, NULL, f1->tipo, f1, NULL, NULL, NULL, NULL);
}

struct AST *create_CONT_BREAK(Type_Exp ast_type_exp) {
    return create_AST(ast_type_exp, NULL, TYPE_NO_VAL, NULL, NULL, NULL, NULL, NULL);
}

struct AST *create_RETURN(Type_Exp ast_type_exp, struct AST *f1) {
    return create_AST(ast_type_exp, NULL, f1->tipo, f1, NULL, NULL, NULL, NULL);
}

struct AST *create_SHIFT(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, struct AST *f1, struct AST *f2) {
    return create_AST(ast_type_exp, val_lex, f1->tipo, f1, f2, NULL, NULL, NULL);
}

struct AST *create_VEC(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, struct AST *f2) {
    struct AST* f1 = create_ID(AST_ID, val_lex);
    return create_AST(ast_type_exp, NULL, f1->tipo, f1, f2, NULL, NULL, NULL);
}

// CHECK dois retornos possiveis
struct AST *create_TERNARIO(Type_Exp ast_type_exp, struct AST *f1, struct AST *f2, struct AST *f3) {
    return create_AST(ast_type_exp, NULL, f2->tipo, f1, f2, f3, NULL, NULL);
}

struct AST *create_ASSIGN(Type_Exp ast_type_exp, struct AST *f1, struct AST *f2) {
    return create_AST(ast_type_exp, NULL, f1->tipo, f1, f2, NULL, NULL, NULL);
}

struct AST *create_DECL_ASSIGN(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, struct valor_lexico_t *id, struct AST *f2) {
    struct AST* f1 = create_ID(AST_ID, id);
    return create_AST(ast_type_exp, val_lex, f1->tipo, f1, f2, NULL, NULL, NULL);
}

struct AST *create_DECL_ASSIGN_id(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, struct valor_lexico_t *id, struct valor_lexico_t *id2) {
    struct AST* f1 = create_ID(AST_ID, id);
    struct AST* f2 = create_ID(AST_ID, id2);
    return create_AST(ast_type_exp, val_lex, f1->tipo, f1, f2, NULL, NULL, NULL);
}

struct AST *create_IF(Type_Exp ast_type_exp, struct AST *f1, struct AST *f2, struct AST *f3) {
    return create_AST(ast_type_exp, NULL, TYPE_NO_VAL, f1, f2, f3, NULL, NULL);
}

struct AST *create_WHILE(Type_Exp ast_type_exp, struct AST *f1, struct AST *f2) {
    return create_AST(ast_type_exp, NULL, TYPE_NO_VAL, f1, f2, NULL, NULL, NULL);
}

struct AST *create_FOR(Type_Exp ast_type_exp, struct AST *f1, struct AST *f2, struct AST *f3, struct AST *f4) {
    return create_AST(ast_type_exp, NULL, TYPE_NO_VAL, f1, f2, f3, f4, NULL);
}

struct AST *create_FUN_CALL(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, struct AST *f1) {
    return create_AST(ast_type_exp, val_lex, get_tipo_elemento_tabela(stack_table, val_lex), f1, NULL, NULL, NULL, NULL);
}

int return_size(struct AST *f1)  {
    if (f1->tipo_exp == AST_LIT)
    {
        return f1->valor_lexico->valor.val_int;
    }
    else if (f1->tipo_exp == AST_OP_UN)
    {
        return f1->children[0]->valor_lexico->valor.val_int;
    }
}

Type tipo_de_valor(struct valor_lexico_t *valor){
    switch (valor->tipo) {
    case VAL_INT:
        return TYPE_INT;
        break;
    case VAL_FLOAT:
        return TYPE_FLOAT;
        break;
    case VAL_CHAR:
        return TYPE_CHAR;
        break;
    case VAL_STRING:
        return TYPE_STRING;
        break;
    case VAL_BOOL:
        return TYPE_BOOL;
        break;
    
    default:
        return TYPE_NO_VAL;
        break;
    }
}