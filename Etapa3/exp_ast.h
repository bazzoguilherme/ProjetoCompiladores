#include "ast.h"

void exporta (void *arvore);
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
void print_OP_BIN(struct AST *ast);