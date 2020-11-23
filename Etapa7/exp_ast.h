
#ifndef EXT_AST_H
#define EXT_AST_H
#include "ast.h"

void libera (void *arvore);
void libera_ast (struct AST *ast);

void free_val_lex(struct valor_lexico_t *val_lex);

void exporta (void *arvore, int opt);

void print_pointers(struct AST *ast);

void print_ast (struct AST *ast);

void print_LIT(struct AST *ast);
void print_ID(struct AST *ast);
void print_FUNCAO(struct AST *ast);
void print_DECL_ASSIGN(struct AST *ast);
void print_ASSIGN(struct AST *ast);
void print_VEC(struct AST *ast);
void print_SHIFT(struct AST *ast);
void print_IO(struct AST *ast);
void print_BR_CONT(struct AST *ast);
void print_RETURN(struct AST *ast);
void print_IF(struct AST *ast);
void print_WHILE(struct AST *ast);
void print_FOR(struct AST *ast);
void print_TERNARIO(struct AST *ast);
void print_OP_BIN(struct AST *ast);
void print_OP_UN(struct AST *ast);
void print_FUN_CALL(struct AST *ast);


#endif