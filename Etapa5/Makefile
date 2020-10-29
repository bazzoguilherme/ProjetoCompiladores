filename=etapa5
BISON=bison
LEX=flex
GCC=gcc

all: comp.o
	$(GCC) -o $(filename) main.o lex.yy.o parser.tab.o ast.o exp_ast.o  symbol_table.o -lfl

comp.o: main.c lex.yy.c parser.tab.c ast.c
	$(GCC) -c main.c lex.yy.c parser.tab.c ast.c exp_ast.c symbol_table.c

parser.tab.c: parser.y
	$(BISON) -d parser.y

lex.yy.c: scanner.l
	$(LEX) scanner.l

ast: ast.c ast.h
	$(GCC) -c ast.c

report: parser.y
	$(BISON) --report-file="report.txt" parser.y

parser_table: parser.y
	$(BISON) -v parser.y

clean:
	rm $(filename) lex.yy.c parser.tab.c parser.tab.h *.o