# E7

## Grupo J
- Guilherme T. Bazzo
- Gustavo A. Lorentz

## Otimização - Etapa 7



### Otimização 1 - Push-Pop

No código assembly, estamos utilizando a própria pilha pra auxiliar nas operações.
Algo muito frequente é fazer um Push de um valor e logo em seguida um Pop do mesmo.

Esse passo de otimização permite que o valor que esteja sendo feito o push vá direto ao registrador de destino do pop que há de vir a seguir.

Exemplo de otimização (código em assembly):
O que fazemos sem otimização:
``` 
    subq	$4, %rsp
    movl	%eax, (%rsp)
    movl    (%rsp), %edx
    addq    $4, %rsp
```
Código Otimizado:
```
    movl    $4, %edx
```

### Otimização 2 - loadImed & opBin

Otimização do código iloc
Quando há, por exemplo, uma operação binária (+, -, *) entre dois inteiros (imediatos), essa operação é feita em código durante compilação.

Exemplo (código Iloc):
```
    loadI   2 => r0
    loadI   7 => r1
    add     r0, r1 => r0
```

Código Otimizado:
```
    loadI   9 => r0
```

## Redução de Tamanho
Para a **etapa*6*** do projeto, o nosso arquivo de teste apresentava um tamanho de 640 linhas.
Com as otimizações da **etapa*7*** o tamanho foi reduzido para ~400 linhas.