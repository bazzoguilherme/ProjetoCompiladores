filename=etapa2
BISON=bison
LEX=flex
GCC=gcc

proj: comp.o
	$(GCC) -o $(filename) main.o lex.yy.o parser.tab.o -lfl

comp.o: lex.yy.c parser.tab.c
	$(GCC) -c main.c lex.yy.c parser.tab.c

parser.tab.c: parser.y
	$(BISON) -d parser.y

lex.yy.c: scanner.l
	$(LEX) scanner.l

report: parser.y
	$(BISON) --report-file="report.txt" parser.y

parser_table: parser.y
	$(BISON) -v parser.y

clean:
	rm $(filename) lex.yy.* parser.tab.* main.o