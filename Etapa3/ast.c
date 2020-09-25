#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

struct valor_lexico_t *lex_int(int int_value, int tipo_tk, int linha) {
    struct valor_lexico_t *val_lex = (struct valor_lexico_t*) malloc (sizeof(struct valor_lexico_t));
    val_lex->valor.val_int = int_value;
    val_lex->tipo = tipo_tk;
    val_lex->linha = linha;
    return val_lex;
}

struct valor_lexico_t *lex_bool(int bool_val, int tipo_tk, int linha) {
    return lex_int(bool_val, tipo_tk, linha);
}

struct valor_lexico_t *lex_float(float float_val, int tipo_tk, int linha) {
    struct valor_lexico_t *val_lex = (struct valor_lexico_t*) malloc (sizeof(struct valor_lexico_t));
    val_lex->valor.val_float = float_val;
    val_lex->tipo = tipo_tk;
    val_lex->linha = linha;
    return val_lex;
}

struct valor_lexico_t *lex_char(char char_val, int tipo_tk, int linha) {
    struct valor_lexico_t *val_lex = (struct valor_lexico_t*) malloc (sizeof(struct valor_lexico_t));
    val_lex->valor.val_char = char_val;
    val_lex->tipo = tipo_tk;
    val_lex->linha = linha;
    return val_lex;
}

struct valor_lexico_t *lex_str(char *str_val, int tipo_tk, int linha) {
    struct valor_lexico_t *val_lex = (struct valor_lexico_t*) malloc (sizeof(struct valor_lexico_t));
    val_lex->valor.val_str = strdup(str_val);
    remove_quotes(val_lex->valor.val_str);
    val_lex->tipo = tipo_tk;
    val_lex->linha = linha;
    return val_lex;
}

struct valor_lexico_t *lex_id(char *id_val, int tipo_tk, int linha) {
    return lex_str(id_val, tipo_tk, linha);
}

struct valor_lexico_t *lex_especial(char *esp_val, int tipo_tk, int linha) {
    return lex_str(esp_val, tipo_tk, linha);
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