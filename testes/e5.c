int simples1; // 7
int simples2; // 21
int if_test; // 0
int while_test; // 10
int for_test; // 20
int ternario_test; // 11
int fun1; // 2
int fun_add; // 5
int fun_mult; // 6
int fun3param1; // 7
int fib0, fib1, fib2, fib3, fib4, fib5, fib6, fib7, fib8;

int inc(int x) {
    return x+1;
}

int add(int a, int b) {
    return a+b;
}

int mult(int a, int b) {
    return a*b;
}

int multi_add(int a, int b, int c) {
    return a+b+c;
}

int fib(int a) {
    if (a == 0) {
        return 0;
    };
    if (a == 1) {
        return 1;
    };
    // int aux1, aux2;
    return fib(a-1)+fib(a-2);
    // aux1 = fib(a-1);
    // aux2 = fib(a-2);
    // return aux1+aux2;
}

int main() {

    simples1 = 2 + 5;
    simples2 = simples1 * 3;

    int i<=1;

    if (i == 2) {
        if_test = 1;
    } else {
        if_test = 0;
    };

    while_test = 0;
    int w <= 10;
    while(w > 0) do {
        w = w-1;
        while_test = while_test + 1;
    };


    int f <= 0;
    for (f = 0 : f < 20 : f = f+1) {
	int inutil <= 0;
    };

    for_test = f;

    ternario_test = (f == 20 && while_test == 10) ? 11 : -1;

    fun1 = inc(i);

    fun_add = add(2, 3);
    fun_mult = mult(2, 3);

    fun3param1 = multi_add(1+1, 2, 1+1+1);

    fib0 = fib(0);
    fib1 = fib(1);
    fib2 = fib(2);
    fib3 = fib(3);
    fib4 = fib(4);
    fib5 = fib(5);
    fib6 = fib(6);
    fib7 = fib(7);
    fib8 = fib(8);

    return fun3param1;
}
