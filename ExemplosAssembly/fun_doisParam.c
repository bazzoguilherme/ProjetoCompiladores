int simples1; // 7
int simples2; // 21

int inc(int x) {
    if (x > 10){
        return x;
    }
    return inc(x+1);
}

int foo (int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9){
    return a1+a7+a8+a9;
}

int main() {
    foo(1, 2, 3, 4, 5,6,7,8,9);
}