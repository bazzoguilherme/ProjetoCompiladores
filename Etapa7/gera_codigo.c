#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ast.h"
#include "symbol_table.h"
#include "gera_codigo.h"


int regis = 0, label = 1; // L0 reservado para halt
char *fun_atual;
extern struct stack_symbol_table *stack;

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
    if (code1 == NULL) {
        if (code2!=NULL) {
            return concat(code2, code3, NULL);
        }
        return code3;
    }
        

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
    codigo->tipo = code_normal;
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
        struct code *sub = gera_code(NULL_LABEL, op_rsubI, dest, 0, dest, NULL_REGIS, NULL);
        c->prox = sub;
    }
    return c;
}

struct code *gera_load_var(OP op, struct AST *ast, int dest) {
    int desloc, escopo;
    desloc = deslocamento_symbol(ast->valor_lexico->valor.val_str, &escopo);
    return gera_code(NULL_LABEL, op, ((escopo == GLOBAL) ? RBSS:RFP), ((escopo == GLOBAL) ? desloc : -desloc), dest, NULL_REGIS, NULL);
}

struct code *gera_decl_funcao(struct valor_lexico_t *nome_funcao) {
    struct code *atualiza_inicial;
    // if (strcmp("main", fun_atual) == 0)
    //     atualiza_inicial = gera_code(label_funcao(nome_funcao->valor.val_str), op_i2i, RFP, NULL_REGIS, RSP, NULL_REGIS, NULL); // Atualiza RFP
    // else
        atualiza_inicial = gera_code(label_funcao(nome_funcao->valor.val_str), op_i2i, RSP, NULL_REGIS, RFP, NULL_REGIS, NULL); // Atualiza RFP
    int desloc, escopo;
    desloc = deslocamento_funcao_atual();
    struct code *atualiza_rsp = gera_code(NULL_LABEL, op_subI, RSP, desloc, RSP, NULL_REGIS, NULL); // Atualiza RSP

    atualiza_inicial->prox = atualiza_rsp;
    
    return atualiza_inicial;
}

struct code *gera_args(struct AST *params) {
    struct AST *aux = params;
    struct code *c=NULL, *c_aux=NULL, *code_atual;
    int i;
    for (i = INIT_ESC_NOMEADO; aux != NULL; i += 4) {
        code_atual = gera_code(NULL_LABEL, op_storeAI, aux->local, NULL_REGIS, RSP, -(i+16), NULL);
        code_atual->tipo = code_adiciona_parametro;
        if (i == INIT_ESC_NOMEADO) {
            c = aux->codigo;
            c = concat(c, code_atual, NULL);
        } else {
            c_aux = aux->codigo;
            c = concat(c, c_aux, code_atual);
        }
        aux = aux->prox;
    }
    return c;
}

struct code *gera_chamada_funcao(struct valor_lexico_t *fun_name, struct AST *params, int dest) {
    // posicao retorno
    int reg = gera_regis();
    struct code *load_returned_value = gera_code(NULL_LABEL, op_loadAI, RSP, RETORNO_FUNC, dest, NULL_REGIS, NULL);
    load_returned_value->tipo = code_load_retorno_funcao;
    struct code *jump_fun = gera_code(NULL_LABEL, op_jumpI, NULL_REGIS, NULL_REGIS, label_funcao(fun_name->valor.val_str), NULL_REGIS, load_returned_value);
    struct code *store_pos_retorno = gera_code(NULL_LABEL, op_storeAI, reg, NULL_REGIS, RSP, LOCAL_RETORNO, jump_fun);
    struct code *pos_retorno = gera_code(NULL_LABEL, op_addI, RPC, 3, reg, NULL_REGIS, store_pos_retorno);

    // argumentos
    struct code *store_args = gera_args(params);

    store_args = concat(store_args, pos_retorno, NULL);

    struct code *store_rfp = gera_code(NULL_LABEL, op_storeAI, RFP, NULL_REGIS, RSP, DESL_RFP, store_args);
    struct code *store_rsp = gera_code(NULL_LABEL, op_storeAI, RSP, NULL_REGIS, RSP, DESL_RSP, store_rfp);
    store_rsp->tipo = code_preparacao_chamada;
    return store_rsp;
}

struct code *gera_retorno(struct AST *retorno) {
    struct code *c = gera_code(NULL_LABEL, op_storeAI, retorno->local, NULL_REGIS, RFP, RETORNO_FUNC, NULL);
    c->tipo = code_returno_funcao;
    return concat(retorno->codigo, c, retorno_funcao());
}

struct code *retorno_funcao() {
    if (strcmp(fun_atual, "main")==0) {
        struct code *ret_main = gera_code(NULL_LABEL, op_jumpI, NULL_REGIS, NULL_REGIS, 0, NULL_REGIS, NULL);
        ret_main->tipo = code_saida_fun_main;
        return ret_main;
    }
    int reg_retorno = gera_regis();
    struct code *jump = gera_code(NULL_LABEL, op_jump, NULL_REGIS, NULL_REGIS, reg_retorno, NULL_REGIS, NULL);;
    struct code *rfp_salvo = gera_code(NULL_LABEL, op_loadAI, RFP, DESL_RFP, RFP, NULL_REGIS, jump);
    struct code *rsp_salvo = gera_code(NULL_LABEL, op_loadAI, RFP, DESL_RSP, RSP, NULL_REGIS, rfp_salvo);
    struct code *end_retorno = gera_code(NULL_LABEL, op_loadAI, RFP, 0, reg_retorno, NULL_REGIS, rsp_salvo);
    end_retorno->tipo = code_saida_funcao;
    return end_retorno;
}

struct code *gera_atribuicao(struct AST *atrib, struct AST *expr) {
    int desloc, escopo;
    desloc = deslocamento_symbol(atrib->valor_lexico->valor.val_str, &escopo);
    return gera_code(NULL_LABEL, op_storeAI, expr->local, NULL_REGIS, ((escopo == GLOBAL) ? RBSS:RFP), ((escopo == GLOBAL) ? desloc : -desloc), NULL);
}

struct code *gera_inicializacao(struct AST *init) {
    if (init == NULL) {
        return NULL;
    }
    struct code *c = gera_atribuicao(init->children[0], init->children[1]);
    return concat(init->children[1]->codigo, c, gera_inicializacao(init->prox));
}

struct code *gera_expressao_bin(struct valor_lexico_t *operacao, struct AST *f1, struct AST *f2, int dest) {
    struct code *c = gera_code(NULL_LABEL, op_operacao(operacao), f1->local, f2->local, dest, NULL_REGIS, NULL);
    return concat(f1->codigo, f2->codigo, c);
}

void gera_unario(struct AST *exp_un) {
    char operador_unario = exp_un->valor_lexico->valor.val_char;
    if (operador_unario == '+') { // transfere para cima
        exp_un->local = exp_un->children[0]->local;
        exp_un->codigo = exp_un->children[0]->codigo;
    } else if (operador_unario == '-') { // adiciona inversao de sinal
        exp_un->local = gera_regis();
        struct code *sub = gera_code(NULL_LABEL, op_rsubI, exp_un->children[0]->local, 0, exp_un->local, NULL_REGIS, NULL);
        exp_un->codigo = concat(exp_un->children[0]->codigo, sub, NULL);
    } else if (operador_unario == '!') {
        gera_not(exp_un);
    }
}

void gera_relop(struct AST *node) {
    int dest = node->local;
    struct valor_lexico_t *val_lex = node->valor_lexico;

    struct code *cbr = gera_code(NULL_LABEL, op_cbr, dest, NULL_REGIS, 0, 0, NULL);
    struct code *comparacao = gera_code(NULL_LABEL, op_operacao(val_lex), node->children[0]->local, node->children[1]->local, dest, NULL_REGIS, cbr);

    node->codigo = concat(node->children[0]->codigo, node->children[1]->codigo, comparacao);

    node->tl = lista_rem(&cbr->dest1);
    node->fl = lista_rem(&cbr->dest2);
}

void gera_and(struct AST *node) {
    struct code *x = rot();
    remenda(node->children[0], REMENDO_T, x->label);
    node->tl = node->children[1]->tl;
    node->fl = concat_remendo(node->children[0]->fl, node->children[1]->fl);
    node->codigo = concat(node->children[0]->codigo, x, node->children[1]->codigo);
}

void gera_or(struct AST *node) {
    struct code *x = rot();
    remenda(node->children[0], REMENDO_F, x->label);
    node->fl = node->children[1]->fl;
    node->tl = concat_remendo(node->children[0]->tl, node->children[1]->tl);
    node->codigo = concat(node->children[0]->codigo, x, node->children[1]->codigo);
}

void gera_not(struct AST *node) {
    node->codigo = node->children[0]->codigo;
    node->tl = node->children[0]->fl;
    node->fl = node->children[0]->tl;
}


struct code *gera_IF(struct AST *cond, struct AST *bloco, struct AST *else_opt) {
    struct code *x = rot();
    struct code *y = rot();
    struct code *z = NULL;
    struct code *z_zump1 = NULL;
    struct code *z_zump2 = NULL;

    remenda(cond, REMENDO_T, x->label);
    remenda(cond, REMENDO_F, y->label);

    struct code* c = concat(cond->codigo, x, (bloco == NULL) ? NULL : bloco->codigo);
    if (else_opt == NULL) {
        c = concat(c, y, NULL);
    } else {
        z = rot();
        z_zump1 = gera_code(NULL_LABEL, op_jumpI, NULL_REGIS, NULL_REGIS, z->label, NULL_REGIS, NULL);
        z_zump2 = gera_code(NULL_LABEL, op_jumpI, NULL_REGIS, NULL_REGIS, z->label, NULL_REGIS, NULL);
        z_zump1 = concat(z_zump1, y, else_opt->codigo);
        z_zump1 = concat(z_zump1, z_zump2, z);
        c = concat(c, z_zump1, NULL);
    }
    return c;
}

struct code *gera_IF_ternario(struct AST *cond, struct AST *expr1, struct AST *expr2, int reg) {
    struct code *x = rot();
    struct code *y = rot();
    struct code *z = NULL;
    struct code *z_zump1 = NULL;
    struct code *z_zump2 = NULL;

    remenda(cond, REMENDO_T, x->label);
    remenda(cond, REMENDO_F, y->label);
    //(int label, OP op, int arg1, int arg2, int dest1, int dest2, struct code *prox)
    //                                                                      expr1->valor_lexico->valor.val_int
    struct code* c = concat(cond->codigo, x, expr1->codigo);
    c = concat(c, gera_code(NULL_LABEL, op_i2i, expr1->local, NULL_REGIS, reg, NULL_REGIS, NULL), NULL);

    z = rot();
    z_zump1 = gera_code(NULL_LABEL, op_jumpI, NULL_REGIS, NULL_REGIS, z->label, NULL_REGIS, NULL);
    z_zump2 = gera_code(NULL_LABEL, op_jumpI, NULL_REGIS, NULL_REGIS, z->label, NULL_REGIS, NULL);
    z_zump1 = concat(z_zump1, y, expr2->codigo);
    z_zump1 = concat(z_zump1, gera_code(NULL_LABEL, op_i2i, expr2->local, NULL_REGIS, reg, NULL_REGIS, NULL), NULL);
    z_zump1 = concat(z_zump1, z_zump2, z);
    c = concat(c, z_zump1, NULL);

    return c;
}

struct code *gera_WHILE(struct AST *cond, struct AST *bloco) {
    struct code *x = rot();
    struct code *y = rot();
    struct code *z = rot();
    struct code *z_zump = gera_code(NULL_LABEL, op_jumpI, NULL_REGIS, NULL_REGIS, z->label, NULL_REGIS, NULL);

    remenda(cond, REMENDO_T, x->label);
    remenda(cond, REMENDO_F, y->label);

    struct code *c = concat(z, cond->codigo, x);
    c = concat(c, (bloco == NULL) ? NULL : bloco->codigo, z_zump);
    c = concat(c, y, NULL);

    return c;
}

struct code *gera_FOR(struct AST *atrib1, struct AST *cond, struct AST *atrib2, struct AST *bloco) {
    struct code *x = rot();
    struct code *y = rot();
    struct code *z = rot();
    struct code *z_zump = gera_code(NULL_LABEL, op_jumpI, NULL_REGIS, NULL_REGIS, z->label, NULL_REGIS, NULL);

    remenda(cond, REMENDO_T, x->label);
    remenda(cond, REMENDO_F, y->label);

    struct code *c = concat(atrib1->codigo, z, cond->codigo);
    c = concat(c, x, (bloco == NULL) ? NULL : bloco->codigo);
    c = concat(c, atrib2->codigo, z_zump);
    c = concat(c, y, NULL);
    return c;
}

struct code *gera_halt() {
    return gera_code(0, op_halt, NULL_REGIS, NULL_REGIS, NULL_REGIS, NULL_REGIS, NULL); // L0 default halt
}

struct code *instrucoes_iniciais() { // Jump to 'main' or 'halt' if not present
    struct code *jmp = gera_code(NULL_LABEL, op_jumpI, NULL_REGIS, NULL_REGIS, label_funcao("main"), NULL_REGIS, gera_halt());
    struct code *rbss_default = gera_code(NULL_LABEL, op_loadI, 500, NULL_REGIS, RBSS, NULL_REGIS, jmp);
    struct code *rsp_default = gera_code(NULL_LABEL, op_loadI, 1024, NULL_REGIS, RSP, NULL_REGIS, rbss_default);
    struct code *rfp_default = gera_code(NULL_LABEL, op_loadI, 1024, NULL_REGIS, RFP, NULL_REGIS, rsp_default);
    return rfp_default;
}

struct code *rot() {
    return gera_code(gera_label(), nop, NULL_REGIS, NULL_REGIS, NULL_REGIS, NULL_REGIS, NULL);
}

struct l_remendo *lista_rem(int *pos_remendo) {
    struct l_remendo *rem = (struct l_remendo *) malloc(sizeof(struct l_remendo));
    rem->remendo = pos_remendo;
    rem->prox = NULL;
    return rem;
}

void remenda(struct AST *ast, int t_f, int label) {
    if (t_f == REMENDO_T) {
        remenda_lista(ast->tl, label);
        libera_remendo(ast->tl);
        ast->tl = NULL;
    } else {
        remenda_lista(ast->fl, label);
        libera_remendo(ast->fl);
        ast->fl = NULL;
    }
}

void remenda_lista(struct l_remendo *lista_remendo, int label) {
    if (lista_remendo == NULL) return;
    *lista_remendo->remendo = label;
    remenda_lista(lista_remendo->prox, label);
}

struct l_remendo *concat_remendo(struct l_remendo *r1, struct l_remendo *r2) {
    struct l_remendo *aux = r1;
    if(aux == NULL) {
        return r2;
    }
    while(aux->prox != NULL) {
        aux = aux->prox;
    }
    aux->prox = r2;
    return r1;
}



OP op_operacao(struct valor_lexico_t *operacao) {
    if (operacao->tipo == VAL_ESPECIAL) { // char
        return op_simples(operacao->valor.val_char);
    } else { // Op-composto
        return op_composta(operacao->valor.val_str);
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
    case '<':
        return op_cmp_LT;
        break;
    case '>':
        return op_cmp_GT;
        break;
    default:
        break;
    }
}

OP op_composta(char *op) {
    if (strcmp(op, "<=") == 0) {
        return op_cmp_LE;
    } else if (strcmp(op, ">=") == 0) {
        return op_cmp_GE;
    } else if (strcmp(op, "==") == 0) {
        return op_cmp_EQ;
    } else if (strcmp(op, "!=") == 0) {
        return op_cmp_NE;
    } 
    // else if (strcmp(op, "&&") == 0) {
    //     return op_and;
    // } else if (strcmp(op, "||") == 0) {
    //     return op_or;
    // }
}

void update_current_fun_name(struct valor_lexico_t *fun) {
    fun_atual = fun->valor.val_str;
}

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
        print_rr_d_cmp(c);
        break;
    case op_cmp_LE:
        printf("cmp_LE");
        print_rr_d_cmp(c);
        break;
    case op_cmp_EQ:
        printf("cmp_EQ");
        print_rr_d_cmp(c);
        break;
    case op_cmp_GE:
        printf("cmp_GE");
        print_rr_d_cmp(c);
        break;
    case op_cmp_GT:
        printf("cmp_GT");
        print_rr_d_cmp(c);
        break;
    case op_cmp_NE:
        printf("cmp_NE");
        print_rr_d_cmp(c);
        break;
    case op_cbr:
        printf("cbr");
        print_r_LL(c);
        break;
    case op_jump:
        printf("jump");
        print_r_jmp(c);
        break;
    case op_jumpI:
        printf("jumpI");
        print_L_jmp(c);
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

void print_rr_d_cmp(struct code *c) {
    printf(" ");
    traduz_regis(c->arg1);
    printf(", ");
    traduz_regis(c->arg2);
    printf(" -> ");
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
    printf(" -> ");
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

void print_r_jmp(struct code *c) {
    printf(" -> ");
    traduz_regis(c->dest1);
}

void print_L_jmp(struct code *c) {
    printf(" -> ");
    traduz_label(c->dest1);
}

void libera_codigo(struct code *c) {
    if (c == NULL) return;
    libera_codigo(c->prox);
    free(c);
}

void libera_remendo(struct l_remendo *r) {
    if (r == NULL) return;
    libera_remendo(r->prox);
    free(r);
}
