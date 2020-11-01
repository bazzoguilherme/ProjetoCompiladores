#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gera_codigo.h"

#define MAX_TAM_REGIS 12
#define MAX_TAM_LABEL 12

int regis = 0, label = 0;

char *gera_regis() {
    char *new_regis = (char *) malloc (MAX_TAM_REGIS);
    snprintf(new_regis, MAX_TAM_REGIS, "r%d", regis++);
    return new_regis;
}

char *gera_label() {
    char *new_label = (char *) malloc (MAX_TAM_LABEL);
    snprintf(new_label, MAX_TAM_LABEL, "L%d", label++);
    return new_label;
}
