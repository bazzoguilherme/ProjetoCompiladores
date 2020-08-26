LEX=flex
GCC=gcc
filename=etapa1

proj: lex.yy.c
	$(GCC) main.c lex.yy.c -o $(filename) -lfl

lex.yy.c:
	$(LEX) scanner.l


clean:
	rm $(filename) lex.yy.c