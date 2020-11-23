filename=etapa7
BISON=bison
LEX=flex
GCC=gcc

all: comp.o
	$(GCC) -o $(filename) main.o lex.yy.o parser.tab.o ast.o exp_ast.o  symbol_table.o gera_codigo.o asm.o -lfl

comp.o: main.c lex.yy.c parser.tab.c ast.c exp_ast.c symbol_table.c gera_codigo.c asm.c
	$(GCC) -c main.c lex.yy.c parser.tab.c ast.c exp_ast.c symbol_table.c gera_codigo.c asm.c

parser.tab.c: parser.y
	$(BISON) -d parser.y

lex.yy.c: scanner.l
	$(LEX) scanner.l

report: parser.y
	$(BISON) --report-file="report.txt" parser.y

parser_table: parser.y
	$(BISON) -v parser.y

clean:
	rm $(filename) lex.yy.c parser.tab.c parser.tab.h *.o