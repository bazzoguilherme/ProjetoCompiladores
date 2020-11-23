/*
  Função principal para realização da E3.
  Não modifique este arquivo.
*/
#include <stdio.h>
#include <string.h>
extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;
void exporta (void *arvore, int opt);
void libera (void *arvore);

int main (int argc, char **argv)
{
  int opt = 0;
  if (argc > 1 && !strcmp("-O", argv[1]))
      opt = 1;

  int ret = yyparse(); 
  exporta (arvore, opt);
  libera(arvore);
  arvore = NULL;
  yylex_destroy();
  return ret;
}
