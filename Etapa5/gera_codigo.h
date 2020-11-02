
#ifndef GERA_COD_H
#define GERA_COD_H

typedef enum OP {
    nop = 0,
    op_add = 1,
    op_sub,
    op_mult,
    op_div,
    op_addI,
    op_subI,
    op_multI,
    op_divI,
    op_load, 
    op_loadAI, 
    op_loadAO, 
    op_cload,
    op_cloadAI,
    op_cloadAO,
    op_store, 
    op_storeAI, 
    op_storeAO, 
    op_cstore, 
    op_cstoreAI, 
    op_cstoreAO, 
    op_i2i,
    op_c2c,
    op_c2i,
    op_i2c,
    op_cmp_LT,
    op_cmp_LE,
    op_cmp_EQ,
    op_cmp_GE,
    op_cmp_GT,
    op_cmp_NE,
    op_cbr,
    op_jump,
    op_jumpI,
} OP;

struct code {
    char* label; // NULL or Lx
    OP operation; // e.g. add r1, r2 => r3
    char* arg1;
    char* arg2;
    char* dest;
    struct code *prox; // montagem de "cabeça para baixo"
};

char *gera_regis();
char *gera_label();


#endif