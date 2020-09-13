%{
#include <stdio.h>

int yylex(void);
void yyerror (char const *s);
%}

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_STRING
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_DO
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_PR_CONST
%token TK_PR_STATIC
%token TK_PR_FOREACH
%token TK_PR_FOR
%token TK_PR_SWITCH
%token TK_PR_CASE
%token TK_PR_BREAK
%token TK_PR_CONTINUE
%token TK_PR_CLASS
%token TK_PR_PRIVATE
%token TK_PR_PUBLIC
%token TK_PR_PROTECTED
%token TK_PR_END
%token TK_PR_DEFAULT
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_OC_SL
%token TK_OC_SR
%token TK_OC_FORWARD_PIPE
%token TK_OC_BASH_PIPE
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_LIT_STRING
%token TK_IDENTIFICADOR
%token TOKEN_ERRO

%%

programa: EXPRESSAO;

DECL_GLOBAL: TK_PR_STATIC TYPE ID_or_VECT LIST_VAR 
           | TYPE ID_or_VECT LIST_VAR;

LIST_VAR: ',' ID_or_VECT LIST_VAR 
        | ';';

ID_or_VECT: TK_IDENTIFICADOR
          | TK_IDENTIFICADOR '[' TK_LIT_INT ']';

TYPE: TK_PR_INT
    | TK_PR_FLOAT
    | TK_PR_BOOL
    | TK_PR_CHAR
    | TK_PR_STRING;

EXPRESSAO: EXPR_OPERACAO    //Expressao Aritmetica
         | EXPR_LOGICA;

EXPR_OPERACAO: Expr_Sum;

Expr_Sum: EXPR_OPERACAO OP_BIN_Sum Expr_Prod
        | Expr_Prod;

Expr_Prod: EXPR_OPERACAO OP_BIN_Prod Expr_Exp
         | Expr_Exp;

Expr_Exp: EXPR_OPERACAO OP_BIN_Exp F
        | F;

F: '(' EXPR_OPERACAO ')'
 | EXPR_ARIT
 | OP_Sinal_Explicito EXPR_ARIT
 | OP_ACESSO_VAR EXPR_ARIT_A;

EXPR_ARIT: EXPR_ARIT_A
         | EXPR_ARIT_B;

EXPR_ARIT_A: TK_IDENTIFICADOR
           | TK_IDENTIFICADOR '[' EXPR_OPERACAO ']';

EXPR_ARIT_B: TK_LIT_INT
           | TK_LIT_FLOAT;

EXPR_ARIT_C: ;

EXPR_LOGICA: EXPRESSAO OP_BIN_Comp EXPRESSAO
           | EXPRESSAO OP_BIN_Logic EXPRESSAO;

OP_Sinal_Explicito: '+'
                  | '-';

OP_ACESSO_VAR: '&'
             | '*'
             | '#';

OP_BIN_Sum: '+'
          | '-';

OP_BIN_Prod: '*'
           | '/'
           | '%';

OP_BIN_Exp: '^';

OP_BIN_Logic: '|'
            | '&'
            | TK_OC_SR
            | TK_OC_SL;

OP_BIN_Comp: "<"
           | ">"
           | TK_OC_LE
           | TK_OC_GE
           | TK_OC_EQ
           | TK_OC_NE
           | TK_OC_OR
           | TK_OC_AND;

%%

void yyerror (char const *s) {
    printf("%s - error\n", s); // ONLY DEBUG - depois remover print e *include de stdio*
}