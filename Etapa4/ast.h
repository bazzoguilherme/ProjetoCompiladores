
#ifndef AST_H
#define AST_H

#define MAX_FILHOS 4

typedef enum Tipo_val_lex {
    VAL_INT = 1,
    VAL_FLOAT,
    VAL_CHAR,
    VAL_STRING,
    VAL_BOOL,
    VAL_ESPECIAL,
    VAL_OP_COMPOSTO,
    VAL_IDENTIFICADOR,
} Tipo_val_lex;

union val_lex {
    int val_int;
    float val_float;
    char *val_str;
    char val_char;
};

struct valor_lexico_t {
    int linha;
    Tipo_val_lex tipo;
    union val_lex valor;
};

struct valor_lexico_t *lex_int(int int_value, Tipo_val_lex tipo_tk, int linha);
struct valor_lexico_t *lex_bool(int bool_val, Tipo_val_lex tipo_tk, int linha);
struct valor_lexico_t *lex_float(float float_val, Tipo_val_lex tipo_tk, int linha);
struct valor_lexico_t *lex_char(char char_val, Tipo_val_lex tipo_tk, int linha);
struct valor_lexico_t *lex_str(char *str_val, Tipo_val_lex tipo_tk, int linha);
struct valor_lexico_t *lex_id(char *id_val, Tipo_val_lex tipo_tk, int linha);
struct valor_lexico_t *lex_especial(char esp_val, Tipo_val_lex tipo_tk, int linha);
struct valor_lexico_t *lex_composto(char *op_comp_val, Tipo_val_lex tipo_tk, int linha);

void remove_quotes(char *st);

typedef enum Type_Exp {
    AST_LIT = 1,
    AST_ID,
    AST_FUNCAO, // Funcao
    AST_DECL_ASSIGN, // <=
    AST_ASSIGN, // =
    AST_VEC, // []
    AST_IN, // input
    AST_OUT, // output
    AST_CONT, // continue
    AST_BREAK, // break
    AST_RETURN, // return
    AST_SHIFT, // shift
    AST_TERNARIO, // op Ternario
    AST_OP_BIN, // Op Binaria (f1 op f2)
    AST_OP_UN, // Op Unaria (op f1)
    AST_IF, // if
    AST_FOR, // For
    AST_WHILE, // While
    AST_FUN_CALL, // Chamada de Funcao
} Type_Exp;

typedef enum Type {
    TYPE_NO_VAL = 0,
    TYPE_INT = 1,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_STRING,
    TYPE_BOOL,
} Type;

typedef struct AST {
    Type tipo;
    Type_Exp tipo_exp;
    struct valor_lexico_t *valor_lexico;
    struct AST *children[MAX_FILHOS];
    struct AST *prox;
} AST;

struct AST *create_AST(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, Type tipo_nodo, struct AST *f1, struct AST *f2, struct AST *f3, struct AST *f4, struct AST *next);

struct AST *create_LIT(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex);

struct AST *create_ID(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex);

struct AST *create_NODE(struct AST *f1, struct AST *next);

struct AST *create_FUNCAO(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, struct AST *f1);

struct AST *create_EXPRESSAO(Type_Exp ast_type_exp, struct AST *f1, struct AST *f2);
struct AST *create_EXPRESSAO_BIN(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, struct AST *f1, struct AST *f2);
struct AST *create_EXPRESSAO_UN(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, struct AST *f1);
struct AST *create_EXPRESSAO_UN_LIT(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, struct valor_lexico_t *lit);

struct AST *create_IO(Type_Exp ast_type_exp, struct AST *f1);
struct AST *create_IO_id(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex);

struct AST *create_CONT_BREAK(Type_Exp ast_type_exp);

struct AST *create_RETURN(Type_Exp ast_type_exp, struct AST *f1);

struct AST *create_SHIFT(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, struct AST *f1, struct AST *f2);

struct AST *create_VEC(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, struct AST *f2);

struct AST *create_TERNARIO(Type_Exp ast_type_exp, struct AST *f1, struct AST *f2, struct AST *f3);

struct AST *create_ASSIGN(Type_Exp ast_type_exp, struct AST *f1, struct AST *f2);

struct AST *create_DECL_ASSIGN(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, struct valor_lexico_t *id, struct AST *f2);
struct AST *create_DECL_ASSIGN_id(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, struct valor_lexico_t *id, struct valor_lexico_t *id2);

struct AST *create_IF(Type_Exp ast_type_exp, struct AST *f1, struct AST *f2, struct AST *f3);
struct AST *create_WHILE(Type_Exp ast_type_exp, struct AST *f1, struct AST *f2);
struct AST *create_FOR(Type_Exp ast_type_exp, struct AST *f1, struct AST *f2, struct AST *f3, struct AST *f4);

struct AST *create_FUN_CALL(Type_Exp ast_type_exp, struct valor_lexico_t *val_lex, struct AST *f1);

void atualiza_tipo_nodos_decl(struct AST *nodo, Type tipo_nodo);

int return_size(struct AST *f1);

Type tipo_de_valor(struct valor_lexico_t *valor);

#endif