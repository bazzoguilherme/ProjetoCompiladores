#include <stdio.h>
#include "exp_ast.h"

// printf("%p [label=\"%s\"]\n", ast, ast->children[0]->valor_lexico->valor.val_str);
void exporta (void *arvore){
	struct AST *ast = (struct AST*) arvore;
    print_ast(ast);
}

void print_ast(struct AST *ast) {
    if (ast == NULL) {
        return;
    }
    printf("Print - tipo: %d\n", ast->tipo);
    switch (ast->tipo)
    {
    case AST_LIT:
        print_LIT(ast);
        break;
    case AST_ID:
        print_ID(ast);
        break;
    case AST_FUNCAO:
        print_FUNCAO(ast);
        print_ast(ast->prox);
        break;
    case AST_NODE:
        print_ast(ast->children[0]);
        print_ast(ast->prox);
        break;
    case AST_DECL_ASSIGN:
        print_DECL_ASSIGN(ast);
        break;
    case AST_ASSIGN:
        print_ASSIGN(ast);
        break;
    case AST_VEC:
        print_VEC(ast);
        break;
    case AST_SHIFT:
        print_SHIFT(ast);
        break;
    case AST_IN:
    case AST_OUT:
        print_IO(ast);
        break;
    default:
        break;
    }
}


void print_LIT(struct AST *ast) {
    switch (ast->valor_lexico->tipo)
    {
    case VAL_ESPECIAL:
    case VAL_OP_COMPOSTO:
    case VAL_IDENTIFICADOR:
    case VAL_STRING:
        printf("%p [label=\"%s\"];\n", ast, ast->valor_lexico->valor.val_str);
        break;
    case VAL_CHAR:
        printf("%p [label=\"%c\"];\n", ast, ast->valor_lexico->valor.val_char);
        break;
    case VAL_INT:
        printf("%p [label=\"%d\"];\n", ast, ast->valor_lexico->valor.val_int);
        break;
    case VAL_FLOAT:
        printf("%p [label=\"%f\"];\n", ast, ast->valor_lexico->valor.val_float);
        break;
    case VAL_BOOL:
        printf("%p [label=\"%s\"];\n", ast, (ast->valor_lexico->valor.val_int ? "true" : "false"));
        break;
    default:
        break;
    }
}

void print_ID(struct AST *ast) {
    printf("%p [label=\"%s\"];\n", ast, ast->valor_lexico->valor.val_str);
}

void print_FUNCAO(struct AST *ast) {
    printf("%p [label=\"%s\"];\n", ast, ast->children[0]->valor_lexico->valor.val_str);
    print_ast(ast->children[0]->children[0]);
}

void print_DECL_ASSIGN(struct AST *ast) {
    print_LIT(ast);
    print_ast(ast->children[0]);
    print_ast(ast->children[1]);
}

void print_ASSIGN(struct AST *ast) {
    printf("%p [label=\"=\"];\n", ast);
    print_ast(ast->children[0]);
    print_ast(ast->children[1]);
}

void print_VEC(struct AST *ast) {
    printf("%p [label=\"[]\"];\n", ast);
    print_ast(ast->children[0]);
    print_ast(ast->children[1]);
}


void print_SHIFT(struct AST *ast) {
    print_LIT(ast);
    print_ast(ast->children[0]);
    print_ast(ast->children[1]);
}

void print_IO(struct AST *ast) {
    printf("%p [label=\"%s\"];\n", ast, (ast->tipo == AST_IN ? "input" : "output"));
    print_ast(ast->children[0]);
}