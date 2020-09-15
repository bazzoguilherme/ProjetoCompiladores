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

programa: DECL_GLOBAL programa
		| DECL_FUNC programa
		| ;

DECL_GLOBAL: TK_PR_STATIC TYPE ID_or_VECT LIST_VAR_GL 
           | TYPE ID_or_VECT LIST_VAR_GL;

LIST_VAR_GL: ',' ID_or_VECT LIST_VAR_GL
           | ';';

ID_or_VECT: TK_IDENTIFICADOR
          | TK_IDENTIFICADOR '[' TK_LIT_INT ']';

DECL_LOCAL: TK_PR_STATIC ID_LOCAL LIST_VAR_LOCAL
          | TK_PR_STATIC TK_PR_CONST ID_LOCAL LIST_VAR_LOCAL
          | TK_PR_CONST ID_LOCAL LIST_VAR_LOCAL
          | ID_LOCAL LIST_VAR_LOCAL;

LIST_VAR_LOCAL: ',' ID_LOCAL LIST_VAR_LOCAL
			  | ;

ID_LOCAL: TYPE TK_IDENTIFICADOR
        | TYPE TK_IDENTIFICADOR ASSIGN LIT
        | TYPE TK_IDENTIFICADOR ASSIGN TK_IDENTIFICADOR;

ASSIGN: TK_OC_LE;

ATRIBUICAO: TK_IDENTIFICADOR '=' EXPRESSAO
          | TK_IDENTIFICADOR '[' EXPRESSAO ']' '=' EXPRESSAO;

IO_Dados: ENTRADA
        | SAIDA;

ENTRADA: TK_PR_INPUT TK_IDENTIFICADOR;

SAIDA: TK_PR_OUTPUT TK_IDENTIFICADOR
     | TK_PR_OUTPUT LIT;

TYPE: TK_PR_INT
    | TK_PR_FLOAT
    | TK_PR_BOOL
    | TK_PR_CHAR
    | TK_PR_STRING;

EXPRESSAO: Expr_Log_Cmp;

Expr_Log_Cmp: Expr_Log_Cmp OP_LOG_Comp Expr_Log
            | Expr_Log;

Expr_Log: Expr_Log OP_BIN_Logic Expr_Cmp
        | Expr_Cmp;
        
Expr_Cmp: Expr_Cmp OP_BIN_Comp Expr_Sum
        | Expr_Sum;

Expr_Sum: Expr_Sum OP_BIN_Sum Expr_Prod
        | Expr_Prod;

Expr_Prod: Expr_Prod OP_BIN_Prod Expr_Exp
         | Expr_Exp;

Expr_Exp: Expr_Exp OP_BIN_Exp F
        | F;

F: '(' EXPRESSAO ')'
 | EXPR_ARIT
 | EXPR_LOG_LIT
 | OP_UNARIO EXPR_ARIT
 | OP_UNARIO EXPR_LOG_LIT;

EXPR_ARIT: EXPR_ARIT_A
         | EXPR_ARIT_B
		 | EXPR_ARIT_C;

EXPR_ARIT_A: TK_IDENTIFICADOR
           | TK_IDENTIFICADOR '[' EXPRESSAO ']';

EXPR_ARIT_B: TK_LIT_INT
           | TK_LIT_FLOAT;

EXPR_ARIT_C: CHAMA_FUNC ; // Chamada de funcao

EXPR_LOG_LIT: TK_LIT_TRUE
            | TK_LIT_FALSE;

LIT: TK_LIT_INT
   | TK_LIT_FLOAT
   | TK_LIT_CHAR
   | TK_LIT_STRING
   | TK_LIT_TRUE
   | TK_LIT_FALSE;

// EXPR_TERNARIA: EXPRESSAO '?' EXPRESSAO ':' EXPRESSAO;




DECL_FUNC: DECL_HEADER CORPO;

DECL_HEADER: TK_PR_STATIC TYPE TK_IDENTIFICADOR '(' PARAM ')'
		   | TYPE TK_IDENTIFICADOR '(' PARAM ')';

TYPE_CONST_PARAM: TK_PR_CONST
                | ;

PARAM: TYPE_CONST_PARAM TYPE TK_IDENTIFICADOR L_PARAM
     | ;

L_PARAM: ',' PARAM
       | ;

CORPO: BLOCO;

BLOCO: '{' COMANDOS '}';

COMANDOS: COMANDO COMANDOS
			   | ;

COMANDO: DECL_LOCAL ';'
	   | ATRIBUICAO ';'
	   | IO_Dados ';'
	   | CHAMA_FUNC ';'
	   | COM_SHIFT ';'
	   | RET ';'
	   | CONT ';'
	   | BREAK ';'
	   | CONTR_FLUXO
	   | WHILE
	   | FOR;

CONTR_FLUXO: TK_PR_IF '(' EXPRESSAO ')' BLOCO
           | TK_PR_IF '(' EXPRESSAO ')' BLOCO TK_PR_ELSE BLOCO;

WHILE: TK_PR_WHILE '(' EXPRESSAO ')' TK_PR_DO BLOCO;

FOR: TK_PR_FOR '(' ATRIBUICAO ':' EXPRESSAO ':' ATRIBUICAO ')' BLOCO;

RET: TK_PR_RETURN EXPRESSAO;

CONT: TK_PR_CONTINUE;

BREAK: TK_PR_BREAK;


CHAMA_FUNC: TK_IDENTIFICADOR '(' PARAM_F_CALL ')';

PARAM_F_CALL: EXPRESSAO L_PARAM_F_CALL
            | ;

L_PARAM_F_CALL: ',' PARAM_F_CALL
              | ;


COM_SHIFT: TK_IDENTIFICADOR OP_Shift EXPRESSAO
         | TK_IDENTIFICADOR '[' EXPRESSAO ']' OP_Shift EXPRESSAO;




OP_UNARIO: '+'
         | '-'
         | '!'
         | '&'
         | '*'
         | '?'
         | '#';

OP_BIN_Sum: '+'
          | '-';

OP_BIN_Prod: '*'
           | '/'
           | '%';

OP_BIN_Exp: '^';

OP_BIN_Logic: '|'
            | '&';

OP_LOG_Comp: TK_OC_AND
           | TK_OC_OR;

OP_BIN_Comp: '<'
           | '>'
           | TK_OC_LE
           | TK_OC_GE
           | TK_OC_EQ
           | TK_OC_NE;

OP_Shift: TK_OC_SR
        | TK_OC_SL;


%%

void yyerror (char const *s) {
    printf("%s - error\n", s); // ONLY DEBUG - depois remover print e *include de stdio*
}