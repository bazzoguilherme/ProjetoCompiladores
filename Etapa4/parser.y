
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
#include "exp_ast.h"
#include "symbol_table.h"

int yylex(void);
int yyerror (char const *s);

extern int get_line_number(void);

extern void *arvore;
extern void exporta(void *arvore);
extern void libera(void *arvore);

struct stack_symbol_table *stack_table = NULL;

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
%token<valor_lexico> TK_OC_LE
%token<valor_lexico> TK_OC_GE
%token<valor_lexico> TK_OC_EQ
%token<valor_lexico> TK_OC_NE
%token<valor_lexico> TK_OC_AND
%token<valor_lexico> TK_OC_OR
%token<valor_lexico> TK_OC_SL
%token<valor_lexico> TK_OC_SR
%token TK_OC_FORWARD_PIPE
%token TK_OC_BASH_PIPE
%token<valor_lexico> TK_LIT_INT
%token<valor_lexico> TK_LIT_FLOAT
%token<valor_lexico> TK_LIT_FALSE
%token<valor_lexico> TK_LIT_TRUE
%token<valor_lexico> TK_LIT_CHAR
%token<valor_lexico> TK_LIT_STRING
%token<valor_lexico> TK_IDENTIFICADOR
%token TOKEN_ERRO

%type<ast> programa
%type<ast> declaracao_funcao
%type<ast> bloco
%type<ast> comando
%type<ast> comandos
%type<ast> int_positivo
%type<ast> io_dados
%type<ast> entrada
%type<ast> saida
%type<ast> continue
%type<ast> break
%type<ast> retorno
%type<ast> comando_shift
%type<ast> id_ou_vet_expr
%type<ast> expressao
%type<ast> expr
%type<ast> expr_log_comp
%type<ast> expr_log
%type<ast> expr_comp
%type<ast> expr_soma
%type<ast> expr_produto
%type<ast> expr_expoente
%type<ast> F
%type<ast> expr_arit
%type<ast> expr_arit_B
%type<ast> expr_arit_C
%type<ast> expr_log_literal
%type<ast> literal
%type<ast> literal_char_str
%type<ast> id_local
%type<ast> lista_var_local
%type<ast> declaracao_local
%type<ast> atribuicao
%type<ast> controle_fluxo
%type<ast> else_opt
%type<ast> while
%type<ast> for
%type<ast> possivel_parametro
%type<ast> lista_parametro_chamada_funcao
%type<ast> parametro_chamada_funcao
%type<ast> chamada_funcao

%type<valor_lexico> declaracao_header
%type<valor_lexico> op_shift
%type<valor_lexico> op_logica_comparacao
%type<valor_lexico> op_binaria_logica
%type<valor_lexico> op_binaria_soma
%type<valor_lexico> op_binaria_produto
%type<valor_lexico> op_binaria_expoente
%type<valor_lexico> op_binaria_comparacao
%type<valor_lexico> mais_menos
%type<valor_lexico> op_unaria
%type<valor_lexico> op_unaria_prio_dir
%type<valor_lexico> assign

%%

programa: declaracao_global programa { $$ = $2; }
	| declaracao_funcao programa { $$ = create_NODE($1, $2); arvore = (void*) $$; }
	| { $$ = NULL; }
	;

declaracao_global: TK_PR_STATIC tipo id_ou_vetor lista_var_global 
	| tipo id_ou_vetor lista_var_global;

lista_var_global: ',' id_ou_vetor lista_var_global
	| ';';

id_ou_vetor: TK_IDENTIFICADOR { 
		if (stack_table == NULL) {
			stack_table = escopo_global();
		} 
		insere_simbolo(stack_table, $1, NAT_variavel);
		free_val_lex($1); }
	| TK_IDENTIFICADOR '[' int_positivo ']' { free_val_lex($1); libera_ast($3); } ;

declaracao_local: TK_PR_STATIC tipo id_local lista_var_local { $$ = create_NODE($3, $4); }
	| TK_PR_STATIC TK_PR_CONST tipo id_local lista_var_local { $$ = create_NODE($4, $5); }
	| TK_PR_CONST tipo id_local lista_var_local { $$ = create_NODE($3, $4); }
	| tipo id_local lista_var_local { $$ = create_NODE($2, $3); };

lista_var_local: ',' id_local lista_var_local { $$ = create_NODE($2, $3); }
	| { $$ = NULL; };

id_local: TK_IDENTIFICADOR { $$ = NULL; free_val_lex($1); }
	| TK_IDENTIFICADOR assign literal { $$ = create_DECL_ASSIGN(AST_DECL_ASSIGN, $2, $1, $3); }
	| TK_IDENTIFICADOR assign TK_IDENTIFICADOR { $$ = create_DECL_ASSIGN_id(AST_DECL_ASSIGN, $2, $1, $3); } ;

assign: TK_OC_LE { $$ = $1; };

atribuicao: id_ou_vet_expr '=' expressao { $$ = create_ASSIGN(AST_ASSIGN, $1, $3); }
	| id_ou_vet_expr '=' literal_char_str { $$ = create_ASSIGN(AST_ASSIGN, $1, $3); };


io_dados: entrada { $$ = $1; }
	| saida { $$ = $1; }
	;

entrada: TK_PR_INPUT TK_IDENTIFICADOR { $$ = create_IO_id(AST_IN, $2); };

saida: TK_PR_OUTPUT TK_IDENTIFICADOR { $$ = create_IO_id(AST_OUT, $2); }
    | TK_PR_OUTPUT literal { $$ = create_IO(AST_OUT, $2); }
	;

tipo: TK_PR_INT
	| TK_PR_FLOAT
	| TK_PR_BOOL
	| TK_PR_CHAR
	| TK_PR_STRING;

expressao: expr { $$ = $1; };

expr: expr_log_comp { $$ = $1; }
	| expr_log_comp '?' expressao ':' expressao { $$ = create_TERNARIO(AST_TERNARIO, $1, $3, $5); }
	;

expr_log_comp: expr_log_comp op_logica_comparacao expr_log { $$ = create_EXPRESSAO_BIN(AST_OP_BIN, $2, $1, $3); }
	| expr_log { $$ = $1; };

expr_log: expr_log op_binaria_logica expr_comp { $$ = create_EXPRESSAO_BIN(AST_OP_BIN, $2, $1, $3); }
	| expr_comp { $$ = $1; };
        
expr_comp: expr_comp op_binaria_comparacao expr_soma { $$ = create_EXPRESSAO_BIN(AST_OP_BIN, $2, $1, $3); }
	| expr_soma { $$ = $1; };

expr_soma: expr_soma op_binaria_soma expr_produto { $$ = create_EXPRESSAO_BIN(AST_OP_BIN, $2, $1, $3); }
	| expr_produto { $$ = $1; };

expr_produto: expr_produto op_binaria_produto expr_expoente { $$ = create_EXPRESSAO_BIN(AST_OP_BIN, $2, $1, $3); }
	| expr_expoente { $$ = $1; };

expr_expoente: expr_expoente op_binaria_expoente F { $$ = create_EXPRESSAO_BIN(AST_OP_BIN, $2, $1, $3); }
	| F { $$ = $1; };

F: '(' expressao ')' { $$ = $2; }
	| expr_arit { $$ = $1;}
	| expr_log_literal { $$ = $1; }
	| op_unaria expr_arit { $$ = create_EXPRESSAO_UN(AST_OP_UN, $1, $2); }
	| op_unaria expr_log_literal { $$ = create_EXPRESSAO_UN(AST_OP_UN, $1, $2); }
	| op_unaria_prio_dir '(' expressao ')' { $$ = create_EXPRESSAO_UN(AST_OP_UN, $1, $3); };

expr_arit: id_ou_vet_expr { $$ = $1; }
	| expr_arit_B { $$ = $1; }
	| expr_arit_C { $$ = $1; };

expr_arit_B: TK_LIT_INT { $$ = create_LIT(AST_LIT, $1); }
	| TK_LIT_FLOAT { $$ = create_LIT(AST_LIT, $1); } ;

expr_arit_C: chamada_funcao { $$ = $1; }; // Chamada de funcao

expr_log_literal: TK_LIT_TRUE { $$ = create_LIT(AST_LIT, $1); }
	| TK_LIT_FALSE { $$ = create_LIT(AST_LIT, $1); };

literal: mais_menos TK_LIT_INT { $$ = create_EXPRESSAO_UN_LIT(AST_OP_UN, $1, $2); }
   | mais_menos TK_LIT_FLOAT { $$ = create_EXPRESSAO_UN_LIT(AST_OP_UN, $1, $2); }
   | literal_char_str { $$ = $1; }
   | TK_LIT_TRUE { $$ = create_LIT(AST_LIT, $1); }
   | TK_LIT_FALSE { $$ = create_LIT(AST_LIT, $1); };


literal_char_str: 
	  TK_LIT_CHAR  { $$ = create_LIT(AST_LIT, $1); }
	| TK_LIT_STRING	{ $$ = create_LIT(AST_LIT, $1); }
	;


declaracao_funcao: declaracao_header bloco { $$ = create_FUNCAO(AST_FUNCAO, $1, $2); };

declaracao_header: TK_PR_STATIC tipo TK_IDENTIFICADOR '(' parametros ')' { $$ = $3; }
	| tipo TK_IDENTIFICADOR '(' parametros ')' { $$ = $2; } ;

tipo_const: TK_PR_CONST
	| ;

parametros: lista_parametro
	| ;

lista_parametro: parametro ',' lista_parametro
	| parametro ;

parametro: tipo_const tipo TK_IDENTIFICADOR { free_val_lex($3); };

bloco: '{' comandos '}' { $$ = $2; };

comandos: comando comandos { $$ = create_NODE($1, $2); }
	| { $$ = NULL; } ;

comando: declaracao_local ';' { $$ = $1; }
	| atribuicao ';' { $$ = $1; }
	| io_dados ';' { $$ = $1; }
	| chamada_funcao ';' { $$ = $1; }
	| comando_shift ';' { $$ = $1; }
	| retorno ';' { $$ = $1; }
	| continue ';' { $$ = $1; }
	| break ';' { $$ = $1; }
	| controle_fluxo ';' { $$ = $1; }
	| while ';' { $$ = $1; }
	| for ';' { $$ = $1; }
	| bloco ';' { $$ = $1; };

controle_fluxo: TK_PR_IF '(' expressao ')' bloco else_opt { $$ = create_IF(AST_IF, $3, $5, $6); };

else_opt: TK_PR_ELSE bloco { $$ = $2; }
	| { $$ = NULL; } ;

while: TK_PR_WHILE '(' expressao ')' TK_PR_DO bloco { $$ = create_WHILE(AST_WHILE, $3, $6); };

for: TK_PR_FOR '(' atribuicao ':' expressao ':' atribuicao ')' bloco 
	{ $$ = create_FOR(AST_FOR, $3, $5, $7, $9); } ;

retorno: TK_PR_RETURN expressao { $$ = create_RETURN(AST_RETURN, $2); }
   | TK_PR_RETURN literal_char_str { $$ = create_RETURN(AST_RETURN, $2); };

continue: TK_PR_CONTINUE { $$ = create_CONT_BREAK(AST_CONT); };

break: TK_PR_BREAK { $$ = create_CONT_BREAK(AST_BREAK); };


chamada_funcao: TK_IDENTIFICADOR '(' parametro_chamada_funcao ')' { $$ = create_FUN_CALL(AST_FUN_CALL, $1, $3); };

parametro_chamada_funcao: lista_parametro_chamada_funcao { $$ = $1;}
	| { $$ = NULL; };

lista_parametro_chamada_funcao: possivel_parametro ',' parametro_chamada_funcao { $$ = create_NODE($1, $3); }
	| possivel_parametro { $$ = $1; };

possivel_parametro: expressao {$$ = $1;}
	| literal_char_str { $$ = $1; } ;

comando_shift: id_ou_vet_expr op_shift int_positivo { $$ = create_SHIFT(AST_SHIFT, $2, $1, $3); };

id_ou_vet_expr: TK_IDENTIFICADOR { $$ = create_ID(AST_ID, $1); }
	| TK_IDENTIFICADOR '[' expressao ']' { $$ = create_VEC(AST_VEC, $1, $3); };

op_unaria: '+' { $$ = lex_especial('+', VAL_ESPECIAL, get_line_number()); }
	| '-' { $$ = lex_especial('-', VAL_ESPECIAL, get_line_number()); }
	| '!' { $$ = lex_especial('!', VAL_ESPECIAL, get_line_number()); }
	| '&' { $$ = lex_especial('&', VAL_ESPECIAL, get_line_number()); }
	| '*' { $$ = lex_especial('*', VAL_ESPECIAL, get_line_number()); }
	| '?' { $$ = lex_especial('?', VAL_ESPECIAL, get_line_number()); }
	| '#' { $$ = lex_especial('#', VAL_ESPECIAL, get_line_number()); };

op_unaria_prio_dir: '+' { $$ = lex_especial('+', VAL_ESPECIAL, get_line_number()); }
	| '-' { $$ = lex_especial('-', VAL_ESPECIAL, get_line_number()); }
	| '!' { $$ = lex_especial('!', VAL_ESPECIAL, get_line_number()); }
	| '?' { $$ = lex_especial('?', VAL_ESPECIAL, get_line_number()); };

op_binaria_soma: '+' { $$ = lex_especial('+', VAL_ESPECIAL, get_line_number()); }
	| '-' { $$ = lex_especial('-', VAL_ESPECIAL, get_line_number()); };

op_binaria_produto: '*' { $$ = lex_especial('*', VAL_ESPECIAL, get_line_number()); }
	| '/' { $$ = lex_especial('/', VAL_ESPECIAL, get_line_number()); }
	| '%' { $$ = lex_especial('%', VAL_ESPECIAL, get_line_number()); };

op_binaria_expoente: '^' { $$ = lex_especial('^', VAL_ESPECIAL, get_line_number()); };

op_binaria_logica: '|' { $$ = lex_especial('|', VAL_ESPECIAL, get_line_number()); }
	| '&' { $$ = lex_especial('&', VAL_ESPECIAL, get_line_number()); };

op_logica_comparacao: TK_OC_AND { $$ = $1; }
	| TK_OC_OR { $$ = $1; };

op_binaria_comparacao: '<' { $$ = lex_especial('<', VAL_ESPECIAL, get_line_number()); }
	| '>' { $$ = lex_especial('>', VAL_ESPECIAL, get_line_number()); }
	| TK_OC_LE { $$ = $1; }
	| TK_OC_GE { $$ = $1; }
	| TK_OC_EQ { $$ = $1; }
	| TK_OC_NE { $$ = $1; };

op_shift: TK_OC_SR { $$ = $1; }
	| TK_OC_SL { $$ = $1; };

mais_menos: '+' { $$ = lex_especial('+', VAL_ESPECIAL, get_line_number()); }
	| '-' { $$ = lex_especial('-', VAL_ESPECIAL, get_line_number()); }
	| { $$ = NULL; };

int_positivo: TK_LIT_INT { $$ = create_LIT(AST_LIT, $1); }
	| '+' TK_LIT_INT { 
		struct valor_lexico_t *val_lex = lex_especial('+', VAL_ESPECIAL, get_line_number());
		$$ = create_EXPRESSAO_UN_LIT(AST_OP_UN, val_lex, $2); 
		}
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
