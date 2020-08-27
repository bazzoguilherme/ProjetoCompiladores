%{
#include "tokens.h"
int line_counter = 1;

void read_multiline();

%}

digito [0-9]
letra [a-zA-Z_]
branco [ \t]

%%

\n {line_counter++;}
"//"({letra}|{digito}|{branco})* { }
"/*" { read_multiline(); }

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

"," { return (int)','; }
";" { return (int)';'; }
":" { return (int)':'; }
"\(" { return (int)'\('; }
"\)" { return (int)')'; }
"\[" { return (int)'\['; }
"]" { return (int)']'; }
"\{" { return (int)'\{'; }
"\}" { return (int)'}'; }
"\+" { return (int)'+'; }
"\-" { return (int)'-'; }
"|" { return (int)'|'; }
"*" { return (int)'*'; }
"\/" { return (int)'/'; }
"<" { return (int)'<'; }
">" { return (int)'>'; }
"=" { return (int)'='; }
"!" { return (int)'!'; }
"&" { return (int)'&'; }
"%" { return (int)'%'; }
"#" { return (int)'#'; }
"^" { return (int)'^'; }
"\." { return (int)'.'; }
"\$" { return (int)'$'; }
"<=" { return TK_OC_LE ; }
">=" { return TK_OC_GE ; }
"==" { return TK_OC_EQ ; }
"!=" { return TK_OC_NE ; }
"&&" { return TK_OC_AND ; }
"||" { return TK_OC_OR ; }
">>" { return TK_OC_SR ; }
"<<" { return TK_OC_SL ; }

"true" { return TK_LIT_TRUE; }
"false" { return TK_LIT_FALSE; }
{letra}+({digito}|{letra})* { return TK_IDENTIFICADOR; }
("\-"|"\+")?{digito}+ { return TK_LIT_INT; }
("\-"|"\+")?{digito}+"."{digito}+(("E"|"e")("\-")?{digito}+)? { return TK_LIT_FLOAT; }
("\-"|"\+")?{digito}+(("E"|"e")("\-")?{digito}+)? { return TK_LIT_FLOAT; }
"\'"."\'" { return TK_LIT_CHAR; }
\".+\" { return TK_LIT_STRING; }

{branco}+ { }

. { return TOKEN_ERRO; }

%%

int get_line_number(){
    return line_counter;
}

void read_multiline() {
    register int c;

    for ( ; ; ) {
        while ( (c = input()) != '*' && c != '\n' &&  c != EOF );
        if (c == '\n'){
            line_counter++;
        } else if ( c == '*' ) {
            while ( (c = input()) == '*' );
            if ( c == '/' )
                break;
        } else if ( c == EOF ) {
            break;
        }
    }
}