
%define parse.error custom
%locations
%define parse.lac full

%{
#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)                  \
   fprintf (File, "%d.%d-%d.%d",                      \
           (Loc).first_line, (Loc).first_column,     \
           (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "ast.h"

int yylex(void);
int yyerror (char const *s);

extern int get_line_number(void);

extern void *arvore;

%}
%union {
	struct valor_lexico_t *valor_lexico;
	struct AST *ast;
}

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
%token<valor_lexico> TK_LIT_INT
%token<valor_lexico> TK_LIT_FLOAT
%token<valor_lexico> TK_LIT_FALSE
%token<valor_lexico> TK_LIT_TRUE
%token<valor_lexico> TK_LIT_CHAR
%token<valor_lexico> TK_LIT_STRING
%token TK_IDENTIFICADOR
%token TOKEN_ERRO
%%

programa: declaracao_global programa
	| declaracao_funcao programa
	| 
	;

declaracao_global: TK_PR_STATIC tipo id_ou_vetor lista_var_global 
	| tipo id_ou_vetor lista_var_global;

lista_var_global: ',' id_ou_vetor lista_var_global
	| ';';

id_ou_vetor: TK_IDENTIFICADOR
	| TK_IDENTIFICADOR '[' int_positivo ']';

declaracao_local: TK_PR_STATIC tipo id_local lista_var_local
	| TK_PR_STATIC TK_PR_CONST tipo id_local lista_var_local
	| TK_PR_CONST tipo id_local lista_var_local
	| tipo id_local lista_var_local;

lista_var_local: ',' id_local lista_var_local
	| ;

id_local: TK_IDENTIFICADOR
	| TK_IDENTIFICADOR assign literal
	| TK_IDENTIFICADOR assign TK_IDENTIFICADOR;

assign: TK_OC_LE;

atribuicao: TK_IDENTIFICADOR '=' expressao
	| TK_IDENTIFICADOR '=' literal_char_str
	| TK_IDENTIFICADOR '[' expressao ']' '=' expressao
	| TK_IDENTIFICADOR '[' expressao ']' '=' literal_char_str;

io_dados: entrada
	| saida;

entrada: TK_PR_INPUT TK_IDENTIFICADOR;

saida: TK_PR_OUTPUT TK_IDENTIFICADOR
    | TK_PR_OUTPUT literal;

tipo: TK_PR_INT
	| TK_PR_FLOAT
	| TK_PR_BOOL
	| TK_PR_CHAR
	| TK_PR_STRING;

expressao: expr;

expr: expr_log_comp
	| expr_log_comp '?' expressao ':' expressao;

expr_log_comp: expr_log_comp op_logica_comparacao expr_log
	| expr_log;

expr_log: expr_log op_binaria_logica expr_comp
	| expr_comp;
        
expr_comp: expr_comp op_binaria_comparacao expr_soma
	| expr_soma;

expr_soma: expr_soma op_binaria_soma expr_produto
	| expr_produto;

expr_produto: expr_produto op_binaria_produto expr_expoente
	| expr_expoente;

expr_expoente: expr_expoente op_binaria_expoente F
	| F;

F: '(' expressao ')'
	| expr_arit
	| expr_log_literal
	| op_unaria expr_arit
	| op_unaria expr_log_literal
	| op_unaria_prio_dir '(' expressao ')';

expr_arit: expr_arit_A
	| expr_arit_B
	| expr_arit_C;

expr_arit_A: TK_IDENTIFICADOR
	| TK_IDENTIFICADOR '[' expressao ']';

expr_arit_B: TK_LIT_INT
	| TK_LIT_FLOAT;

expr_arit_C: chamada_funcao ; // Chamada de funcao

expr_log_literal: TK_LIT_TRUE
	| TK_LIT_FALSE;

literal: mais_menos TK_LIT_INT
   | mais_menos TK_LIT_FLOAT
   | literal_char_str
   | TK_LIT_TRUE
   | TK_LIT_FALSE;


literal_char_str: 
	  TK_LIT_CHAR  { printf("%c\n", yylval.valor_lexico->valor.val_char); }
	| TK_LIT_STRING	{ printf("%s\n", yylval.valor_lexico->valor.val_str); }
	;




declaracao_funcao: declaracao_header bloco;

declaracao_header: TK_PR_STATIC tipo TK_IDENTIFICADOR '(' parametro ')'
	| tipo TK_IDENTIFICADOR '(' parametro ')';

tipo_const: TK_PR_CONST
	| ;

parametro: tipo_const tipo TK_IDENTIFICADOR lista_parametro
	| ;

lista_parametro: ',' parametro
	| ;

bloco: '{' comandos '}';

comandos: comando comandos
	| ;

comando: declaracao_local ';'
	| atribuicao ';'
	| io_dados ';'
	| chamada_funcao ';'
	| comando_shift ';'
	| retorno ';'
	| continue ';'
	| break ';'
	| controle_fluxo ';'
	| while ';'
	| for ';'
	| bloco ';';

controle_fluxo: TK_PR_IF '(' expressao ')' bloco else_opt;

else_opt: TK_PR_ELSE bloco
	| ;

while: TK_PR_WHILE '(' expressao ')' TK_PR_DO bloco;

for: TK_PR_FOR '(' atribuicao ':' expressao ':' atribuicao ')' bloco;

retorno: TK_PR_RETURN expressao
   | TK_PR_RETURN literal_char_str;

continue: TK_PR_CONTINUE;

break: TK_PR_BREAK;


chamada_funcao: TK_IDENTIFICADOR '(' parametro_chamada_funcao ')';

parametro_chamada_funcao: expressao lista_parametro_chamada_funcao
	| literal_char_str lista_parametro_chamada_funcao
	| ;

lista_parametro_chamada_funcao: ',' parametro_chamada_funcao
	| ;


comando_shift: TK_IDENTIFICADOR op_shift int_positivo
	| TK_IDENTIFICADOR '[' expressao ']' op_shift int_positivo;


op_unaria: '+'
	| '-'
	| '!'
	| '&'
	| '*'
	| '?'
	| '#';

op_unaria_prio_dir: '+'
	| '-'
	| '!'
	| '?';

op_binaria_soma: '+'
	| '-';

op_binaria_produto: '*'
	| '/'
	| '%';

op_binaria_expoente: '^';

op_binaria_logica: '|'
	| '&';

op_logica_comparacao: TK_OC_AND
	| TK_OC_OR;

op_binaria_comparacao: '<'
	| '>'
	| TK_OC_LE
	| TK_OC_GE
	| TK_OC_EQ
	| TK_OC_NE;

op_shift: TK_OC_SR
	| TK_OC_SL;

mais_menos: '+'
	| '-'
	| ;

int_positivo: TK_LIT_INT {  }
	| '+' TK_LIT_INT {  }
	;

%%

int yyerror (char const *s) {
    printf("%5d | %s\n", get_line_number(), s); 
	return 1;
}

static int yyreport_syntax_error (const yypcontext_t *ctx)
{
  int res = 0;
  const YYLTYPE *loc = yypcontext_location (ctx);
  YY_LOCATION_PRINT (stderr, *loc);
  fprintf (stderr, "In line %d: syntax error", get_line_number());
  // Report the tokens expected at this point.
  {
    enum { TOKENMAX = 5 };
    yysymbol_kind_t expected[TOKENMAX];
    int n = yypcontext_expected_tokens (ctx, expected, TOKENMAX);
    if (n < 0)
      // Forward errors to yyparse.
      res = n;
    else
      for (int i = 0; i < n; ++i)
        fprintf (stderr, "%s %s",
                 i == 0 ? ": expected" : " or", yysymbol_name (expected[i]));
  }
  // Report the unexpected token.
  {
    yysymbol_kind_t lookahead = yypcontext_token (ctx);
    if (lookahead != YYSYMBOL_YYEMPTY)
      fprintf (stderr, " before %s", yysymbol_name (lookahead));
  }
  fprintf (stderr, "\n");
  return 1;
}


void exporta (void *arvore){

}

void libera (void *arvore){
	
}