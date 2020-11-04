
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
#include "gera_codigo.h"

int yylex(void);
int yyerror (char const *s);

extern int get_line_number(void);

extern void *arvore;
extern void exporta(void *arvore);
extern void libera(void *arvore);

%}
%union {
	struct valor_lexico_t *valor_lexico;
	struct AST *ast;
	int tipo_valor;
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
%type<ast> bloco_end
%type<ast> bloco_funcao
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
%type<ast> expr_log_comp_or
%type<ast> expr_log_comp_and
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
%type<valor_lexico> op_binaria_logica
%type<valor_lexico> op_binaria_soma
%type<valor_lexico> op_binaria_produto
%type<valor_lexico> op_binaria_expoente
%type<valor_lexico> op_binaria_comparacao
%type<valor_lexico> mais_menos
%type<valor_lexico> op_unaria
%type<valor_lexico> op_unaria_prio_dir
%type<valor_lexico> assign

%type<tipo_valor> tipo

%%

programa: declaracao_global programa { $$ = $2; }
	| declaracao_funcao programa { $$ = create_NODE($1, $2); arvore = (void*) $$; }
	| { $$ = NULL; }
	;
	

declaracao_global: TK_PR_STATIC tipo id_ou_vetor lista_var_global {
		adiciona_lista_elem_comTipo($2);
	}
	| tipo id_ou_vetor lista_var_global {
		adiciona_lista_elem_comTipo($1);
	};

lista_var_global: ',' id_ou_vetor lista_var_global
	| ';';

id_ou_vetor: TK_IDENTIFICADOR { 
		cria_simbolo_parcial($1, NAT_variavel, 1);
		free_val_lex($1); }
	| TK_IDENTIFICADOR '[' int_positivo ']' { 
		cria_simbolo_parcial($1, NAT_vetor, return_size($3));
		free_val_lex($1); libera_ast($3); } ;

declaracao_local: TK_PR_STATIC tipo id_local lista_var_local { 
		adiciona_lista_elem_comTipo($2);
		$$ = create_NODE($3, $4);
		atualiza_tipo_nodos_decl($$, $2); }
	| TK_PR_STATIC TK_PR_CONST tipo id_local lista_var_local { 
		adiciona_lista_elem_comTipo($3);
		$$ = create_NODE($4, $5);
		atualiza_tipo_nodos_decl($$, $3); }
	| TK_PR_CONST tipo id_local lista_var_local { 
		adiciona_lista_elem_comTipo($2);
		$$ = create_NODE($3, $4);
		atualiza_tipo_nodos_decl($$, $2); }
	| tipo id_local lista_var_local { 
		adiciona_lista_elem_comTipo($1);
		$$ = create_NODE($2, $3);
		atualiza_tipo_nodos_decl($$, $1); };

lista_var_local: ',' id_local lista_var_local { $$ = create_NODE($2, $3); }
	| { $$ = NULL; };

id_local: TK_IDENTIFICADOR { 
		cria_simbolo_parcial($1, NAT_variavel, 1);
		$$ = NULL; free_val_lex($1); }
	| TK_IDENTIFICADOR assign literal { 
		cria_simbolo_parcial($1, NAT_variavel, 1);
		$$ = create_DECL_ASSIGN(AST_DECL_ASSIGN, $2, $1, $3); }
	| TK_IDENTIFICADOR assign TK_IDENTIFICADOR { 
		verif_utilizacao_identificador($3, NAT_variavel);
		cria_simbolo_parcial($1, NAT_variavel, 1);
		$$ = create_DECL_ASSIGN_id(AST_DECL_ASSIGN, $2, $1, $3); } ;

assign: TK_OC_LE { $$ = $1; };

atribuicao: id_ou_vet_expr '=' expressao { 
	$$ = create_ASSIGN(AST_ASSIGN, $1, $3);
	printf("\tAtrib\n");
	$$->codigo = gera_atribuicao($1, $3);
	print_code($$->codigo); };


io_dados: entrada { $$ = $1; }
	| saida { $$ = $1; }
	;

entrada: TK_PR_INPUT TK_IDENTIFICADOR { 
	verif_utilizacao_identificador($2, NAT_variavel);
	verifica_tipo_input($2);
	$$ = create_IO_id(AST_IN, $2); };

saida: TK_PR_OUTPUT TK_IDENTIFICADOR { 
		verif_utilizacao_identificador($2, NAT_variavel);
		verifica_tipo_output($2);
		$$ = create_IO_id(AST_OUT, $2); }
    | TK_PR_OUTPUT literal { 
		verifica_tipo_output_lit($2);
		$$ = create_IO(AST_OUT, $2); }
	;

tipo: TK_PR_INT { $$ = TYPE_INT; }
	| TK_PR_FLOAT { $$ = TYPE_FLOAT; }
	| TK_PR_BOOL { $$ = TYPE_BOOL; }
	| TK_PR_CHAR { $$ = TYPE_CHAR; }
	| TK_PR_STRING { $$ = TYPE_STRING; } ;

expressao: expr { $$ = $1; };

expr: expr_log_comp_or { $$ = $1; }
	| expr_log_comp_or '?' expressao ':' expressao { $$ = create_TERNARIO(AST_TERNARIO, $1, $3, $5); }
	;

expr_log_comp_or: expr_log_comp_or TK_OC_OR expr_log_comp_and { $$ = create_EXPRESSAO_BIN(AST_OP_BIN, $2, $1, $3); }
	| expr_log_comp_and { $$ = $1; };

expr_log_comp_and: expr_log_comp_and TK_OC_AND expr_log { $$ = create_EXPRESSAO_BIN(AST_OP_BIN, $2, $1, $3); }
	| expr_log { $$ = $1; };

expr_log: expr_log op_binaria_logica expr_comp { $$ = create_EXPRESSAO_BIN(AST_OP_BIN, $2, $1, $3); }
	| expr_comp { $$ = $1; };
        
expr_comp: expr_comp op_binaria_comparacao expr_soma { $$ = create_EXPRESSAO_BIN(AST_OP_BIN, $2, $1, $3); }
	| expr_soma { $$ = $1; };

expr_soma: expr_soma op_binaria_soma expr_produto { $$ = create_EXPRESSAO_BIN(AST_OP_BIN, $2, $1, $3); 
		$$->local = gera_regis();
		$$->codigo = gera_expressao_bin($2, $1, $3, $$->local);}
	| expr_produto { $$ = $1; };

expr_produto: expr_produto op_binaria_produto expr_expoente { 
		$$ = create_EXPRESSAO_BIN(AST_OP_BIN, $2, $1, $3);
		$$->local = gera_regis();
		$$->codigo = gera_expressao_bin($2, $1, $3, $$->local); }
	| expr_expoente { $$ = $1; };

expr_expoente: expr_expoente op_binaria_expoente F { $$ = create_EXPRESSAO_BIN(AST_OP_BIN, $2, $1, $3); }
	| F { $$ = $1; };

F: '(' expressao ')' { $$ = $2; }
	| expr_arit { $$ = $1;}
	| expr_log_literal { $$ = $1; }
	| literal_char_str { $$ = $1; }
	| op_unaria expr_arit { $$ = create_EXPRESSAO_UN(AST_OP_UN, $1, $2); }
	| op_unaria expr_log_literal { $$ = create_EXPRESSAO_UN(AST_OP_UN, $1, $2); }
	| op_unaria literal_char_str { $$ = create_EXPRESSAO_UN(AST_OP_UN, $1, $2); }
	| op_unaria_prio_dir '(' expressao ')' { $$ = create_EXPRESSAO_UN(AST_OP_UN, $1, $3); };

expr_arit: id_ou_vet_expr { 
		$$ = $1; 
		$$->local = gera_regis(); 
		$$->codigo = gera_load_var(op_loadAI, $1, $$->local); }
	| expr_arit_B { $$ = $1; }
	| expr_arit_C { $$ = $1; };

expr_arit_B: TK_LIT_INT { 
		insere_literal($1, NAT_literal, TYPE_INT);
		$$ = create_LIT(AST_LIT, $1);
		$$->local = gera_regis();
		$$->codigo = gera_loadI(op_loadI, $1, $$->local); }
	| TK_LIT_FLOAT { 
		insere_literal($1, NAT_literal, TYPE_FLOAT);
		$$ = create_LIT(AST_LIT, $1); } ;

expr_arit_C: chamada_funcao { $$ = $1; }; // Chamada de funcao

expr_log_literal: TK_LIT_TRUE { 
		insere_literal($1, NAT_literal, TYPE_BOOL);
		$$ = create_LIT(AST_LIT, $1); }
	| TK_LIT_FALSE { 
		insere_literal($1, NAT_literal, TYPE_BOOL);
		$$ = create_LIT(AST_LIT, $1); };

literal: mais_menos TK_LIT_INT { 
		insere_literal($2, NAT_literal, TYPE_INT);
		$$ = create_EXPRESSAO_UN_LIT(AST_OP_UN, $1, $2);
		$$->local = gera_regis();
		$$->codigo = gera_loadI_sinal(op_loadI, $1, $2, $$->local); 
		}
   | mais_menos TK_LIT_FLOAT { 
	   insere_literal($2, NAT_literal, TYPE_FLOAT);
	   $$ = create_EXPRESSAO_UN_LIT(AST_OP_UN, $1, $2); }
   | literal_char_str { 
	   $$ = $1; }
   | TK_LIT_TRUE { 
	   insere_literal($1, NAT_literal, TYPE_BOOL);
	   $$ = create_LIT(AST_LIT, $1); }
   | TK_LIT_FALSE { 
	   insere_literal($1, NAT_literal, TYPE_BOOL);
	   $$ = create_LIT(AST_LIT, $1); };


literal_char_str: 
	  TK_LIT_CHAR  { 
		insere_literal($1, NAT_literal, TYPE_CHAR);
		$$ = create_LIT(AST_LIT, $1); }
	| TK_LIT_STRING	{ 
		insere_literal($1, NAT_literal, TYPE_STRING);
		$$ = create_LIT(AST_LIT, $1); }
	;


declaracao_funcao: declaracao_header decl_header_parametros bloco_funcao { 
	$$ = create_FUNCAO(AST_FUNCAO, $1, $3); 
	$$->codigo = gera_decl_funcao($1);
	$$->codigo = concat_codigos_ast($$, $3, NULL);
	$$->codigo = concat($$->codigo, retorno_funcao(), NULL);
	print_code($$->codigo);
	printf("\n\n");
	delete_escopo();
	};

declaracao_header: TK_PR_STATIC tipo TK_IDENTIFICADOR { 
		insere_simbolo($3, NAT_funcao, $2);
		$$ = $3; }
	| tipo TK_IDENTIFICADOR { 
		insere_simbolo($2, NAT_funcao, $1);
		$$ = $2; } ;

tipo_const: TK_PR_CONST
	| ;

decl_header_parametros: decl_header_param_init decl_header_param_end 
	{ adiciona_argumentos_funcao(); }
;

decl_header_param_init: '(' { new_escopo_funcao(); };

decl_header_param_end: parametros ')';

parametros: lista_parametro
	| ;

lista_parametro: parametro ',' lista_parametro
	| parametro ;

parametro: tipo_const tipo TK_IDENTIFICADOR { 
	insere_simbolo($3, NAT_variavel, $2);
	free_val_lex($3); };


bloco_init: '{' { new_escopo(); }; 
bloco_end: comandos '}' { $$ = $1; delete_escopo(); };

bloco_funcao: '{' comandos '}' { $$ = $2; };

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
	| bloco_init bloco_end ';' { $$ = $2; };

controle_fluxo: TK_PR_IF '(' expressao ')' bloco_init bloco_end else_opt { $$ = create_IF(AST_IF, $3, $6, $7); };

else_opt: TK_PR_ELSE bloco_init bloco_end { $$ = $3; }
	| { $$ = NULL; } ;

while: TK_PR_WHILE '(' expressao ')' TK_PR_DO bloco_init bloco_end { $$ = create_WHILE(AST_WHILE, $3, $7); };

for: TK_PR_FOR '(' atribuicao ':' expressao ':' atribuicao ')' bloco_init bloco_end
	{ $$ = create_FOR(AST_FOR, $3, $5, $7, $10); } ;

retorno: TK_PR_RETURN expressao { 
	verifica_retorno_funcao($2);
	$$ = create_RETURN(AST_RETURN, $2);
	$$->codigo = gera_retorno($2);
	};

continue: TK_PR_CONTINUE { $$ = create_CONT_BREAK(AST_CONT); };

break: TK_PR_BREAK { $$ = create_CONT_BREAK(AST_BREAK); };


chamada_funcao: TK_IDENTIFICADOR '(' parametro_chamada_funcao ')' { 
	verif_utilizacao_identificador($1, NAT_funcao);
	verifica_chamada_funcao($1, $3);
	$$ = create_FUN_CALL(AST_FUN_CALL, $1, $3);
	$$->local = gera_regis();
	$$->codigo = gera_chamada_funcao($1, $3, $$->local);
	printf("\n\nCHAMA\n");
	print_code($$->codigo);
	};

parametro_chamada_funcao: lista_parametro_chamada_funcao { $$ = $1;}
	| { $$ = NULL; };

lista_parametro_chamada_funcao: possivel_parametro ',' parametro_chamada_funcao { $$ = create_NODE($1, $3); }
	| possivel_parametro { $$ = $1; };

possivel_parametro: expressao {$$ = $1;};

comando_shift: id_ou_vet_expr op_shift int_positivo { 
	verifica_shift($3);
	$$ = create_SHIFT(AST_SHIFT, $2, $1, $3); };

id_ou_vet_expr: TK_IDENTIFICADOR { 
		verif_utilizacao_identificador($1, NAT_variavel);
		$$ = create_ID(AST_ID, $1); }
	| TK_IDENTIFICADOR '[' expressao ']' { 
		verif_utilizacao_identificador($1, NAT_vetor);
		$$ = create_VEC(AST_VEC, $1, $3); };

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

int_positivo: TK_LIT_INT { 
		insere_literal($1, NAT_literal, TYPE_INT);
		$$ = create_LIT(AST_LIT, $1); }
	| '+' TK_LIT_INT { 
		insere_literal($2, NAT_literal, TYPE_INT);
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
