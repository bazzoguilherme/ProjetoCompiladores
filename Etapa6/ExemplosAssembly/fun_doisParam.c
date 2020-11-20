int simples1; // 7
int simples2; // 21

int inc(int x) {
    if (x > 10){
        return x;
    }
    return inc(x+1);
}

int main() {
    simples1 = 0;

    simples1 = inc(simples1);
    return simples1;
}