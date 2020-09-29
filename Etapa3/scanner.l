%{
#include "parser.tab.h"
#include "ast.h"

int line_counter = 1;

int get_line_number();
%}
%x comment 

digito [0-9]
letra [a-zA-Z_]
branco [ \t]
escape_char [abenrtv0]
escape_symbol ['"]

%%

\n {line_counter++;}
"//"[^\n]* { }
"/*" { BEGIN(comment); }

<comment>[^*\n]*
<comment>[^*\n]*\n	   line_counter++;
<comment>"*"+[^*/\n]*
<comment>"*"+[^*/\n]*\n line_counter++;
<comment>"*"+"/"	   BEGIN(INITIAL);

"int" { return TK_PR_INT; }
"float" { return TK_PR_FLOAT; }
"bool" { return TK_PR_BOOL; }
"char" { return TK_PR_CHAR; }
"string" { return TK_PR_STRING; }
"if" { return TK_PR_IF; }
"then" { return TK_PR_THEN; }
"else" { return TK_PR_ELSE; }
"while" { return TK_PR_WHILE; }
"do" { return TK_PR_DO; }
"input" { return TK_PR_INPUT; }
"output" { return TK_PR_OUTPUT; }
"return" { return TK_PR_RETURN; }
"const" { return TK_PR_CONST; }
"static" { return TK_PR_STATIC; }
"foreach" { return TK_PR_FOREACH; }
"for" { return TK_PR_FOR; }
"switch" { return TK_PR_SWITCH; }
"case" { return TK_PR_CASE; }
"break" { return TK_PR_BREAK; }
"continue" { return TK_PR_CONTINUE; }
"class" { return TK_PR_CLASS; }
"private" { return TK_PR_PRIVATE; }
"public" { return TK_PR_PUBLIC; }
"protected" { return TK_PR_PROTECTED; }
"end" { return TK_PR_END; }
"default" { return TK_PR_DEFAULT; }

"," |
";" |
":" |
"(" |
")" |
"[" |
"]" |
"{" |
"}" |
"+" |
"-" |
"|" |
"*" |
"/" |
"<" |
">" |
"=" |
"!" |
"&" |
"%" |
"#" |
"^" |
"." |
"$" |
"@" |
"~" |
"`" |
"?" { 
    return yytext[0]; 
    }
"<=" { yylval.valor_lexico = lex_especial(yytext, VAL_OP_COMPOSTO, get_line_number()); return TK_OC_LE ; }
">=" { yylval.valor_lexico = lex_especial(yytext, VAL_OP_COMPOSTO, get_line_number()); return TK_OC_GE ; }
"==" { yylval.valor_lexico = lex_especial(yytext, VAL_OP_COMPOSTO, get_line_number()); return TK_OC_EQ ; }
"!=" { yylval.valor_lexico = lex_especial(yytext, VAL_OP_COMPOSTO, get_line_number()); return TK_OC_NE ; }
"&&" { yylval.valor_lexico = lex_especial(yytext, VAL_OP_COMPOSTO, get_line_number()); return TK_OC_AND; }
"||" { yylval.valor_lexico = lex_especial(yytext, VAL_OP_COMPOSTO, get_line_number()); return TK_OC_OR ; }
">>" { yylval.valor_lexico = lex_especial(yytext, VAL_OP_COMPOSTO, get_line_number()); return TK_OC_SR ; }
"<<" { yylval.valor_lexico = lex_especial(yytext, VAL_OP_COMPOSTO, get_line_number()); return TK_OC_SL ; }

"true" { 
        yylval.valor_lexico = lex_bool(1, VAL_BOOL, get_line_number());
        return TK_LIT_TRUE; 
    }
"false" { 
        yylval.valor_lexico = lex_bool(0, VAL_BOOL, get_line_number());
        return TK_LIT_FALSE; 
    }

{letra}+({digito}|{letra})* { 
        yylval.valor_lexico = lex_id(yytext, VAL_IDENTIFICADOR, get_line_number());
        return TK_IDENTIFICADOR; 
    }

{digito}+ {
        yylval.valor_lexico = lex_int(atoi(yytext), VAL_INT, get_line_number());
        return TK_LIT_INT; 
    }

{digito}+{letra}({letra}|{digito})* { return TOKEN_ERRO; }

{digito}+"."{digito}+(("E"|"e")[-+]?{digito}+)? { 
        yylval.valor_lexico = lex_float(atof(yytext), VAL_FLOAT, get_line_number());
        return TK_LIT_FLOAT; 
    }

{digito}+"."({digito}*{letra}({digito}|{letra})*)? { return TOKEN_ERRO; }
"."({digito}|{letra})* { return TOKEN_ERRO; }

"\'"[^'"\n]"\'" { 
        yylval.valor_lexico = lex_char(yytext[1], VAL_CHAR, get_line_number());
        return TK_LIT_CHAR; 
    }

"\'\\"({escape_char}|{escape_symbol})"\'" { 
        yylval.valor_lexico = lex_char(yytext[2], VAL_CHAR, get_line_number());
        return TK_LIT_CHAR; 
    }
"\'\'" { return TOKEN_ERRO; }


\"("\\\""|[^\n])*\" { 
        yylval.valor_lexico = lex_str(yytext, VAL_STRING, get_line_number());
        return TK_LIT_STRING; 
    }

{branco}+ { }
\r { }

"*/" { return TOKEN_ERRO; }

. { return TOKEN_ERRO; }

%%

int get_line_number(){
    return line_counter;
}
