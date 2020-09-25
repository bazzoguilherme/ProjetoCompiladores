
#define MAX_N 5

struct valor_lexico_t {
    int linha;
    int tipo;
    union {
        int val_int;
        float val_float;
        char *val_str;
        char val_char;
    } valor;
};

struct valor_lexico_t *lex_int(int int_value, int tipo_tk, int linha);
struct valor_lexico_t *lex_bool(int bool_val, int tipo_tk, int linha);
struct valor_lexico_t *lex_float(float float_val, int tipo_tk, int linha);
struct valor_lexico_t *lex_char(char char_val, int tipo_tk, int linha);
struct valor_lexico_t *lex_str(char *str_val, int tipo_tk, int linha);
struct valor_lexico_t *lex_id(char *id_val, int tipo_tk, int linha);

void remove_quotes(char *st);

typedef enum Type {
    AST_LIT = 1,
    AST_VEC, // []
    AST_DECL_ASSING, // <=
    AST_ASSING, // =
    AST_SL, // <<
    AST_SR, // >>
    AST_IF, // if
} Type;

typedef struct AST {
    Type tipo;
    struct AST *children[MAX_N];
} AST;
