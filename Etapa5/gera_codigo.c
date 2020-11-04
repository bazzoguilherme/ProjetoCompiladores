#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ast.h"
#include "symbol_table.h"
#include "gera_codigo.h"


int regis = 0, label = 0;

int gera_regis() {
    return regis++;
}

int gera_label() {
    return label++;
}

struct code *alloc_code() {
    return (struct code *) malloc(sizeof(struct code));
}

char *int2str(int val) {
    char *new_key = (char *) malloc (INT_SIZE_STR);
    snprintf(new_key, INT_SIZE_STR, "%d", val);
    return new_key;
}

int lex2int(struct valor_lexico_t *literal) {
    return literal->valor.val_int;
}

struct code *concat(struct code *code1, struct code *code2, struct code *code3) {
    struct code *aux = code1;
    if (code1 == NULL)
        return NULL;

    while(aux->prox != NULL)
        aux = aux->prox;

    aux->prox = code2;

    while(aux->prox != NULL)
        aux = aux->prox;

    aux->prox = code3;
    return code1;
}

struct code *concat_codigos_ast(struct AST *ast1, struct AST *ast2, struct AST *ast3) {
    struct code *c1 = NULL, *c2 = NULL, *c3 = NULL;
    if (ast1 != NULL) {
        c1 = ast1->codigo;
    }
    if (ast2 != NULL) {
        c2 = ast2->codigo;
    }
    if (ast3 != NULL) {
        c3 = ast3->codigo;
    }
    return concat(c1, c2, c3);
}

struct code *gera_code(int label, OP op, int arg1, int arg2, int dest1, int dest2, struct code *prox) {
    struct code *codigo = alloc_code();
    codigo->label = label;
    codigo->operation = op;
    codigo->arg1 = arg1;
    codigo->arg2 = arg2;
    codigo->dest1 = dest1;
    codigo->dest2 = dest2;
    codigo->prox = prox;
    // printf("cria_code: %s %s%c %s => %s%c %s\n", traduz_op(op), arg1, ((arg2 == NULL) ? ' ' : ','), ((arg2 == NULL) ? "" : arg2), dest1, ((dest2 == NULL) ? ' ' : ','), ((dest2 == NULL) ? "" : dest2));
    return codigo;
}

struct code *gera_loadI(OP op, struct valor_lexico_t *arg1, int dest){
    return gera_code(NULL_LABEL, op, lex2int(arg1), NULL_REGIS, dest, NULL_REGIS, NULL);
}

struct code *gera_loadI_sinal(OP op, struct valor_lexico_t *sinal, struct valor_lexico_t *arg1, int dest) {
    struct code *c = gera_loadI(op, arg1, dest);
    if (sinal != NULL && sinal->valor.val_char == '-') { // Inverte valor registrador
        struct code *sub = gera_code(NULL_LABEL, op_rsubI, 0, dest, dest, NULL_REGIS, NULL);
        c->prox = sub;
    }
    return c;
}

struct code *gera_load_var(OP op, struct AST *ast, int dest) {
    int desloc, escopo;
    desloc = deslocamento_symbol(ast->valor_lexico->valor.val_str, &escopo);
    return gera_code(NULL_LABEL, op, ((escopo == GLOBAL) ? RBSS:RFP), desloc, dest, NULL_REGIS, NULL);
}

struct code *gera_decl_funcao(struct valor_lexico_t *nome_funcao) {
    struct code *atualiza_rfp = gera_code(NULL_LABEL, op_i2i, RSP, NULL_REGIS, RFP, NULL_REGIS, NULL); // Atualiza RFP
    int desloc, escopo;
    desloc = deslocamento_funcao_atual();
    struct code *atualiza_rsp = gera_code(NULL_LABEL, op_addI, RSP, desloc, RSP, NULL_REGIS, NULL); // Atualiza RSP

    atualiza_rfp->prox = atualiza_rsp;
    
    return atualiza_rfp;
}

struct code *gera_args(struct AST *params) {
    struct AST *aux = params;
    struct code *c=NULL, *c_aux=NULL;
    int i;
    for (i = INIT_ESC_NOMEADO; aux != NULL; i += 4) {
        if (i == INIT_ESC_NOMEADO) {
            c = aux->codigo;
            c = concat(c, gera_code(NULL_LABEL, op_storeAI, aux->local, NULL_REGIS, RSP, i, NULL), NULL);
        } else {
            c_aux = aux->codigo;
            c = concat(c, c_aux, gera_code(NULL_LABEL, op_storeAI, aux->local, NULL_REGIS, RSP, i, NULL));
        }
        aux = aux->prox;
    }
    return c;
}

struct code *gera_chamada_funcao(struct valor_lexico_t *fun_name, struct AST *params, int dest) {
    // posicao retorno
    int reg = gera_regis();
    struct code *load_returned_value = gera_code(NULL_LABEL, op_loadAI, RSP, RETORNO_FUNC, dest, NULL_REGIS, NULL);
    struct code *jump_fun = gera_code(NULL_LABEL, op_jumpI, NULL_REGIS, NULL_REGIS, label_funcao(fun_name->valor.val_str), NULL_REGIS, load_returned_value);
    struct code *store_pos_retorno = gera_code(NULL_LABEL, op_storeAI, reg, NULL_REGIS, RSP, LOCAL_RETORNO, jump_fun);
    struct code *pos_retorno = gera_code(NULL_LABEL, op_addI, RPC, 3, reg, NULL_REGIS, store_pos_retorno);

    // argumentos
    struct code *store_args = gera_args(params);

    store_args = concat(store_args, pos_retorno, NULL);

    struct code *store_rfp = gera_code(NULL_LABEL, op_storeAI, RFP, NULL_REGIS, RSP, DESL_RFP, store_args);
    struct code *store_rsp = gera_code(NULL_LABEL, op_storeAI, RSP, NULL_REGIS, RSP, DESL_RSP, store_rfp);

    return store_rsp;
}

struct code *gera_retorno(struct AST *retorno) {
    struct code *c = gera_code(NULL_LABEL, op_storeAI, retorno->local, NULL_REGIS, RFP, RETORNO_FUNC, NULL);
    return concat(c, retorno_funcao(), NULL);
}

struct code *retorno_funcao() {
    int reg_retorno = gera_regis();
    struct code *jump = gera_code(NULL_LABEL, op_jump, NULL_REGIS, NULL_REGIS, reg_retorno, NULL_REGIS, NULL);
    struct code *rfp_salvo = gera_code(NULL_LABEL, op_loadAI, RFP, DESL_RFP, RFP, NULL_REGIS, jump);
    struct code *rsp_salvo = gera_code(NULL_LABEL, op_loadAI, RFP, DESL_RSP, RSP, NULL_REGIS, rfp_salvo);
    struct code *end_retorno = gera_code(NULL_LABEL, op_loadAI, RFP, 0, reg_retorno, NULL_REGIS, rsp_salvo);
    return end_retorno;
}

struct code *gera_atribuicao(struct AST *atrib, struct AST *expr) {
    int desloc, escopo;
    desloc = deslocamento_symbol(atrib->valor_lexico->valor.val_str, &escopo);
    return gera_code(NULL_LABEL, op_storeAI, expr->local, NULL_REGIS, ((escopo == GLOBAL) ? RBSS:RFP), desloc, NULL);
}

struct code *gera_expressao_bin(struct valor_lexico_t *operacao, struct AST *f1, struct AST *f2, int dest) {
    struct code *c = gera_code(NULL_LABEL, op_operacao(operacao), f1->local, f2->local, dest, NULL_REGIS, NULL);
    return concat(f1->codigo, f2->codigo, c);
}

OP op_operacao(struct valor_lexico_t *operacao) {
    if (operacao->tipo == VAL_ESPECIAL) { // char
        return op_simples(operacao->valor.val_char);
    } else { // Op-composto
        // return op_composta(operacao->valor.val_str);
    }
}

OP op_simples(char op) {
    switch (op)
    {
    case '+':
        return op_add;
        break;
    case '-':
        return op_sub;
        break;
    case '*':
        return op_mult;
        break;
    case '/':
        return op_div;
        break;
    // case '<':
    //     return op_cmp_LT;
    //     break;
    // case '>':
    //     return op_cmp_GT;
    //     break;
    default:
        break;
    }
}

// OP op_composta(char *op) {
//     if (strcmp(op, "<=") == 0) {
//         return op_cmp_LE;
//     } else if (strcmp(op, ">=") == 0) {
//         return op_cmp_GE;
//     } else if (strcmp(op, "==") == 0) {
//         return op_cmp_EQ;
//     } else if (strcmp(op, "!=") == 0) {
//         return op_cmp_NE;
//     } else if (strcmp(op, "&&") == 0) {
//         return op_and;
//     } else if (strcmp(op, "||") == 0) {
//         return op_or;
//     }
// }

void print_code(struct code *codigo) {
    if (codigo == NULL) return;

    if (codigo->label != NULL_LABEL)
        printf("L%d: ", codigo->label);
    
    traduz_op(codigo);
    
    print_code(codigo->prox);
}

void traduz_op(struct code *c) {
    switch (c->operation)
    {
    case nop:
        printf("nop");
        break;
    case op_add:
        printf("add");
        print_rr_d(c);
        break;
    case op_sub:
        printf("sub");
        print_rr_d(c);
        break;
    case op_mult:
        printf("mult");
        print_rr_d(c);
        break;
    case op_div:
        printf("div");
        print_rr_d(c);
        break;
    case op_addI:
        printf("addI");
        print_rc_d(c);
        break;
    case op_subI:
        printf("subI");
        print_rc_d(c);
        break;
    case op_rsubI:
        printf("rsubI");
        print_rc_d(c);
        break;
    case op_multI:
        printf("multI");
        print_rc_d(c);
        break;
    case op_divI:
        printf("divI");
        print_rc_d(c);
        break;
    case op_and:
        printf("and");
        print_rr_d(c);
        break;
    case op_andI:
        printf("andI");
        print_rc_d(c);
        break;
    case op_or:
        printf("or");
        print_rr_d(c);
        break;
    case op_orI:
        printf("orI");
        print_rc_d(c);
        break;
    case op_load:
        printf("load");
        print_r_d(c);
        break;
    case op_loadI:
        printf("loadI");
        print_c_d(c);
        break;
    case op_loadAI:
        printf("loadAI");
        print_rc_d(c);
        break;
    case op_loadAO:
        printf("loadAO");
        print_rr_d(c);
        break;
    case op_cload:
        printf("cload");
        break;
    case op_cloadAI:
        printf("cloadAI");
        break;
    case op_cloadAO:
        printf("cloadAO");
        break;
    case op_store:
        printf("store");
        print_r_d(c);
        break;
    case op_storeAI:
        printf("storeAI");
        print_r_dc(c);
        break;
    case op_storeAO:
        printf("storeAO");
        print_r_dr(c);
        break;
    case op_cstore:
        printf("cstore");
        break;
    case op_cstoreAI:
        printf("cstoreAI");
        break;
    case op_cstoreAO:
        printf("cstoreAO");
        break;
    case op_i2i:
        printf("i2i");
        print_r_d(c);
        break;
    case op_c2c:
        printf("c2c");
        break;
    case op_c2i:
        printf("c2i");
        break;
    case op_i2c:
        printf("i2c");
        break;
    case op_cmp_LT:
        printf("cmp_LT");
        print_rr_d(c);
        break;
    case op_cmp_LE:
        printf("cmp_LE");
        print_rr_d(c);
        break;
    case op_cmp_EQ:
        printf("cmp_EQ");
        print_rr_d(c);
        break;
    case op_cmp_GE:
        printf("cmp_GE");
        print_rr_d(c);
        break;
    case op_cmp_GT:
        printf("cmp_GT");
        print_rr_d(c);
        break;
    case op_cmp_NE:
        printf("cmp_NE");
        print_rr_d(c);
        break;
    case op_cbr:
        printf("cbr");
        print_r_LL(c);
        break;
    case op_jump:
        printf("jump");
        print_r(c);
        break;
    case op_jumpI:
        printf("jumpI");
        print_L(c);
        break;
    default:
        printf("halt");
        break;
    }
    printf("\n");
}

void traduz_regis(int reg_id) {
    if (reg_id > NULL_REGIS) {
        printf("r%d", reg_id);
    } else if (reg_id < NULL_REGIS) {
        printf("%s", regis_especial(reg_id));
    }
}

void traduz_label(int label) {
    printf("L%d", label);
}

char *regis_especial(int reg) {
    switch (reg)
    {
    case RBSS:
        return RBSS_name;
        break;
    case RFP:
        return RFP_name;
        break;
    case RSP:
        return RSP_name;
        break;
    case RPC:
        return RPC_name;
        break;
    default:
        break;
    }
}

void print_rr_d(struct code *c) {
    printf(" ");
    traduz_regis(c->arg1);
    printf(", ");
    traduz_regis(c->arg2);
    printf(" => ");
    traduz_regis(c->dest1);
}

void print_rc_d(struct code *c) {
    printf(" ");
    traduz_regis(c->arg1);
    printf(", ");
    printf("%d", c->arg2);
    printf(" => ");
    traduz_regis(c->dest1);
}

void print_r_d(struct code *c) {
    printf(" ");
    traduz_regis(c->arg1);
    printf(" => ");
    traduz_regis(c->dest1);
}

void print_c_d(struct code *c) {
    printf(" ");
    printf("%d", c->arg1);
    printf(" => ");
    traduz_regis(c->dest1);
}

void print_r_dr(struct code *c) {
    printf(" ");
    traduz_regis(c->arg1);
    printf(" => ");
    traduz_regis(c->dest1);
    printf(", ");
    traduz_regis(c->dest2);
}

void print_r_dc(struct code *c) {
    printf(" ");
    traduz_regis(c->arg1);
    printf(" => ");
    traduz_regis(c->dest1);
    printf(", ");
    printf("%d", c->dest2);
}

void print_r_LL(struct code *c) {
    printf(" ");
    traduz_regis(c->arg1);
    printf(" => ");
    traduz_label(c->dest1);
    printf(", ");
    traduz_label(c->dest2);
}

void print_r(struct code *c) {
    printf(" => ");
    traduz_regis(c->dest1);
}

void print_L(struct code *c) {
    printf(" => ");
    traduz_label(c->dest1);
}