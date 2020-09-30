#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

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

struct valor_lexico_t *lex_id_esp(char *val, Tipo_val_lex tipo_tk, int linha) {
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
    return lex_id_esp(id_val, tipo_tk, linha);
}

struct valor_lexico_t *lex_especial(char *esp_val, Tipo_val_lex tipo_tk, int linha) {
    return lex_id_esp(esp_val, tipo_tk, linha);
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

struct AST *create_AST(Type ast_type, struct valor_lexico_t *val_lex, struct AST *f1, struct AST *f2, struct AST *f3, struct AST *f4, struct AST *next_node) {
    struct AST *new_node = (struct AST*) malloc (sizeof(struct AST));
    new_node->tipo = ast_type;
    new_node->valor_lexico = val_lex;
    new_node->children[0] = f1;
    new_node->children[1] = f2;
    new_node->children[2] = f3;
    new_node->children[3] = f4;
    new_node->prox = next_node;
    return new_node;
}

struct AST *create_LIT(Type ast_type, struct valor_lexico_t *val_lex) {
    return create_AST(ast_type, val_lex, NULL, NULL, NULL, NULL, NULL);
}

struct AST *create_ID(Type ast_type, struct valor_lexico_t *val_lex) {
    return create_AST(ast_type, val_lex, NULL, NULL, NULL, NULL, NULL);
}

struct AST *create_NODE(struct AST *f1, struct AST *next) {
    if (f1 == NULL){
        return next;
    } else if (f1->tipo == AST_DECL_ASSIGN) {
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

struct AST *create_FUNCAO(Type ast_type, struct valor_lexico_t *val_lex, struct AST *f1) {
    return create_AST(ast_type, val_lex, f1, NULL, NULL, NULL, NULL);
}

struct AST *create_EXPRESSAO(Type ast_type, struct AST *f1, struct AST *f2) {
    return create_AST(ast_type, NULL, f1, f2, NULL, NULL, NULL);
}

struct AST *create_EXPRESSAO_BIN(Type ast_type, struct valor_lexico_t *val_lex, struct AST *f1, struct AST *f2) {
    return create_AST(ast_type, val_lex, f1, f2, NULL, NULL, NULL);
}

struct AST *create_EXPRESSAO_UN(Type ast_type, struct valor_lexico_t *val_lex, struct AST *f1) {
    return create_AST(ast_type, val_lex, f1, NULL, NULL, NULL, NULL);
}

struct AST *create_EXPRESSAO_UN_LIT(Type ast_type, struct valor_lexico_t *val_lex, struct valor_lexico_t *lit) {
    struct AST *f1 = create_LIT(AST_LIT, lit);
    if (val_lex != NULL) {
        return create_AST(ast_type, val_lex, f1, NULL, NULL, NULL, NULL);
    }
    return f1;
}

struct AST *create_IO(Type ast_type, struct AST *f1) {
    return create_AST(ast_type, NULL, f1, NULL, NULL, NULL, NULL);
}

struct AST *create_IO_id(Type ast_type, struct valor_lexico_t *val_lex) {
    struct AST *f1 = create_LIT(AST_LIT, val_lex);
    return create_AST(ast_type, NULL, f1, NULL, NULL, NULL, NULL);
}

struct AST *create_CONT_BREAK(Type ast_type) {
    return create_AST(ast_type, NULL, NULL, NULL, NULL, NULL, NULL);
}

struct AST *create_RETURN(Type ast_type, struct AST *f1) {
    return create_AST(ast_type, NULL, f1, NULL, NULL, NULL, NULL);
}

struct AST *create_SHIFT(Type ast_type, struct valor_lexico_t *val_lex, struct AST *f1, struct AST *f2) {
    return create_AST(ast_type, val_lex, f1, f2, NULL, NULL, NULL);
}

struct AST *create_VEC(Type ast_type, struct valor_lexico_t *val_lex, struct AST *f2) {
    struct AST* f1 = create_ID(AST_ID, val_lex);
    return create_AST(ast_type, NULL, f1, f2, NULL, NULL, NULL);
}

struct AST *create_TERNARIO(Type ast_type, struct AST *f1, struct AST *f2, struct AST *f3) {
    return create_AST(ast_type, NULL, f1, f2, f3, NULL, NULL);
}

struct AST *create_ASSIGN(Type ast_type, struct AST *f1, struct AST *f2) {
    return create_AST(ast_type, NULL, f1, f2, NULL, NULL, NULL);
}

struct AST *create_DECL_ASSIGN(Type ast_type, struct valor_lexico_t *val_lex, struct valor_lexico_t *id, struct AST *f2) {
    struct AST* f1 = create_ID(AST_ID, id);
    return create_AST(ast_type, val_lex, f1, f2, NULL, NULL, NULL);
}

struct AST *create_DECL_ASSIGN_id(Type ast_type, struct valor_lexico_t *val_lex, struct valor_lexico_t *id, struct valor_lexico_t *id2) {
    struct AST* f1 = create_ID(AST_ID, id);
    struct AST* f2 = create_ID(AST_ID, id2);
    return create_AST(ast_type, val_lex, f1, f2, NULL, NULL, NULL);
}

struct AST *create_IF(Type ast_type, struct AST *f1, struct AST *f2, struct AST *f3) {
    return create_AST(ast_type, NULL, f1, f2, f3, NULL, NULL);
}

struct AST *create_WHILE(Type ast_type, struct AST *f1, struct AST *f2) {
    return create_AST(ast_type, NULL, f1, f2, NULL, NULL, NULL);
}

struct AST *create_FOR(Type ast_type, struct AST *f1, struct AST *f2, struct AST *f3, struct AST *f4) {
    return create_AST(ast_type, NULL, f1, f2, f3, f4, NULL);
}

struct AST *create_FUN_CALL(Type ast_type, struct valor_lexico_t *val_lex, struct AST *f1) {
    return create_AST(ast_type, val_lex, f1, NULL, NULL, NULL, NULL);
}
