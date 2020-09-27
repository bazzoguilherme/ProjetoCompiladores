
#define MAX_FILHOS 4

typedef enum Tipo_val_lex {
    VAL_ESPECIAL = 1,
    VAL_OP_COMPOSTO,
    VAL_IDENTIFICADOR,
    VAL_INT,
    VAL_FLOAT,
    VAL_CHAR,
    VAL_STRING,
    VAL_BOOL,
} Tipo_val_lex;

struct valor_lexico_t {
    int linha;
    Tipo_val_lex tipo;
    union {
        int val_int;
        float val_float;
        char *val_str;
        char val_char;
    } valor;
};

struct valor_lexico_t *lex_int(int int_value, Tipo_val_lex tipo_tk, int linha);
struct valor_lexico_t *lex_bool(int bool_val, Tipo_val_lex tipo_tk, int linha);
struct valor_lexico_t *lex_float(float float_val, Tipo_val_lex tipo_tk, int linha);
struct valor_lexico_t *lex_char(char char_val, Tipo_val_lex tipo_tk, int linha);
struct valor_lexico_t *lex_str(char *str_val, Tipo_val_lex tipo_tk, int linha);
struct valor_lexico_t *lex_id(char *id_val, Tipo_val_lex tipo_tk, int linha);
struct valor_lexico_t *lex_especial(char *esp_val, Tipo_val_lex tipo_tk, int linha);

void remove_quotes(char *st);

typedef enum Type {
    AST_LIT = 1,
    AST_ID,
    AST_FUNCAO, // Funcao
    AST_NODE, // [;,] Prox Comando
    AST_VEC, // []
    AST_DECL_ASSING, // <=
    AST_ASSING, // =
    AST_IF, // if
    AST_IN, // input
    AST_OUT, // output
    AST_CONT, // continue
    AST_BREAK, // break
    AST_RETURN, // return
    AST_SHIFT, // shift
    AST_TERNARIO, // op Ternario
    AST_OP_BIN, // Op Binaria (f1 op f2)
    AST_OP_UN, // Op Unaria (op f1)
} Type;

typedef struct AST {
    Type tipo;
    struct valor_lexico_t *valor_lexico;
    struct AST *children[MAX_FILHOS];
    struct AST *prox;
} AST;

struct AST *create_AST(Type ast_type, struct valor_lexico_t *val_lex, struct AST *f1, struct AST *f2, struct AST *f3, struct AST *f4, struct AST *next);

struct AST *create_LIT(Type ast_type, struct valor_lexico_t *val_lex);

struct AST *create_ID(Type ast_type, struct valor_lexico_t *val_lex);

struct AST *create_NODE(Type ast_type, struct AST *f1, struct AST *next);

struct AST *create_FUNCAO(Type ast_type, struct valor_lexico_t *val_lex, struct AST *f1);

struct AST *create_COMANDO(Type ast_type, struct AST *f1, struct AST *next);

struct AST *create_EXPRESSAO(Type ast_type, struct AST *f1, struct AST *f2);
struct AST *create_EXPRESSAO_BIN(Type ast_type, struct valor_lexico_t *val_lex, struct AST *f1, struct AST *f2);
struct AST *create_EXPRESSAO_UN(Type ast_type, struct valor_lexico_t *val_lex, struct AST *f1);

struct AST *create_IO(Type ast_type, struct AST *f1);
struct AST *create_IO_id(Type ast_type, struct valor_lexico_t *val_lex);

struct AST *create_CONT_BREAK(Type ast_type);

struct AST *create_RETURN(Type ast_type, struct AST *f1);

struct AST *create_SHIFT(Type ast_type, struct valor_lexico_t *val_lex, struct AST *f1, struct AST *f2);

struct AST *create_VEC(Type ast_type, struct valor_lexico_t *val_lex, struct AST *f2);

struct AST *create_TERNARIO(Type ast_type, struct AST *f1, struct AST *f2, struct AST *f3);

