int global;

int inc(int x){
    return x+1;
}

int main(){
    global = 0;
    while(global<=10)
    global = inc(global);
    return global;
}