#include <stdio.h>
#include <stdlib.h>
#include "exp_ast.h"


void libera(void *arvore) {
    libera_ast((struct AST *) arvore);
}

void libera_ast(struct AST *ast) {
    int i;
    for ( i = 0; i < MAX_FILHOS && ast->children[i] != NULL; i++) {
        libera_ast (ast->children[i]);
    }
    if (ast->prox != NULL)
        libera_ast(ast->prox);
    
    free_val_lex(ast->valor_lexico);
    free(ast);
}

void free_val_lex(struct valor_lexico_t *val_lex) {
    if (val_lex != NULL){
        if (val_lex->tipo == VAL_ESPECIAL || val_lex->tipo == VAL_OP_COMPOSTO || val_lex->tipo == VAL_IDENTIFICADOR || val_lex->tipo == VAL_STRING) 
            free(val_lex->valor.val_str);
        free(val_lex);
    }
}


void exporta (void *arvore){
	struct AST *ast = (struct AST*) arvore;
    print_pointers(ast);
    print_ast(ast);
}

void print_pointers(struct AST *ast) {
    int i = 0;
    if (ast != NULL) {
        for (i = 0; i < MAX_FILHOS && ast->children[i] != NULL; i++) {
            printf("%p, %p\n", ast, ast->children[i]);
        }
        if (ast->prox != NULL)
            printf("%p, %p\n", ast, ast->prox);

        for (i = 0; i < MAX_FILHOS && ast->children[i] != NULL; i++) {
            print_pointers(ast->children[0]);
        }
        if (ast->prox != NULL)
            print_pointers(ast->prox);
    }
}

void print_ast(struct AST *ast) {
    if (ast == NULL) {
        return;
    }
    // printf("Print - tipo: %d\n", ast->tipo);
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
    // case AST_IN:
    // case AST_OUT:
    //     print_IO(ast);
    //     break;
    case AST_BREAK:
    case AST_CONT:
        print_BR_CONT(ast);
        break;
    case AST_RETURN:
        print_RETURN(ast);
        break;
    case AST_IF:
        print_IF(ast);
        break;
    case AST_WHILE:
        print_WHILE(ast);
        break;
    case AST_FOR:
        print_FOR(ast);
        break;
    case AST_TERNARIO:
        print_TERNARIO(ast);
        break;
    case AST_OP_BIN:
        print_OP_BIN(ast);
        break;
    case AST_OP_UN:
        print_OP_UN(ast);
        break;
    case AST_FUN_CALL:
        print_FUN_CALL(ast);
        break;
    default:
        break;
    }
    print_ast(ast->prox);
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
        printf("%p [label=\"%g\"];\n", ast, ast->valor_lexico->valor.val_float);
        break;
    case VAL_BOOL:
        printf("%p [label=\"%s\"];\n", ast, (ast->valor_lexico->valor.val_int ? "true" : "false"));
        break;
    default:
        break;
    }
}

void print_ID(struct AST *ast) {
    print_LIT(ast);
}

void print_FUNCAO(struct AST *ast) {
    print_LIT(ast);
    print_ast(ast->children[0]);
}

void print_DECL_ASSIGN(struct AST *ast) {
    print_OP_BIN(ast);
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
    print_OP_BIN(ast);
}

void print_IO(struct AST *ast) {
    printf("%p [label=\"%s\"];\n", ast, (ast->tipo == AST_IN ? "input" : "output"));
    print_ast(ast->children[0]);
}

void print_BR_CONT(struct AST *ast) {
    printf("%p [label=\"%s\"];\n", ast, (ast->tipo == AST_BREAK ? "break" : "continue"));
}

void print_RETURN(struct AST *ast) {
    printf("%p [label=\"%s\"];\n", ast, "return");
    print_ast(ast->children[0]);
}


void print_IF(struct AST *ast) {
    printf("%p [label=\"%s\"];\n", ast, "if");
    print_ast(ast->children[0]);
    print_ast(ast->children[1]);
    print_ast(ast->children[2]);
}


void print_WHILE(struct AST *ast) {
    printf("%p [label=\"%s\"];\n", ast, "while");
    print_ast(ast->children[0]);
    print_ast(ast->children[1]);
}


void print_FOR(struct AST *ast) {
    printf("%p [label=\"%s\"];\n", ast, "for");
    print_ast(ast->children[0]);
    print_ast(ast->children[1]);
    print_ast(ast->children[2]);
    print_ast(ast->children[3]);
}

void print_TERNARIO(struct AST *ast) {
    printf("%p [label=\"%s\"];\n", ast, "?:");
    print_ast(ast->children[0]);
    print_ast(ast->children[1]);
    print_ast(ast->children[2]);
}

void print_OP_BIN(struct AST *ast) {
    print_LIT(ast);
    print_ast(ast->children[0]);
    print_ast(ast->children[1]);
}

void print_OP_UN(struct AST *ast) {
    print_LIT(ast);
    print_ast(ast->children[0]);
}

void print_FUN_CALL(struct AST *ast) {
    printf("%p [label=\"call %s\"];\n", ast, ast->valor_lexico->valor.val_str);
    print_ast(ast->children[0]);
}