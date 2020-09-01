# Etapa1 - Projeto de Compiladores

A primeira etapa consiste em fazer um analisador léxico utilizando a ferramenta de geração de reconhecedores **flex**. 

## Alunos:
- Guilherme Torresan Bazzo  (00287687)
- Gustavo Acauan Lorentz    (00287681)

## Informações da Linguagem

### Palavras reservadas: 

``
int float bool char string if then else while do input output return const static foreach for switch case break continue class private public protected end default
``

### Caracteres Especiais: 
``
,   ;   :   (   )   [   ]   {   }   +   -   |   ?   @ * /   <   >   =   !   &   %   #   ˆ   .   $ 
``

### Operadores Compostos: 
``
<=  >=  ==  !=  &&  ||  >>  <
``

###  Identificadores
Formados  por  um caractere alfabético seguido de zero ou mais caracteres alfanuméricos, onde considera-se caractere alfabético como letras  maiúsculas  ou  minésculas  ou  o  caractere  sublinhado e onde dígitos são: 0,1,2, . . . ,9.

### Literais
Literais  do  tipo int: repetições  de  um  ou  mais  dígitos  precedidos  opcionalmente  pelo  sinal  de  negativo  ou  positivo.
```
1
-5
+10
```
Literais  em float são formados como um inteiro seguido de ponto decimal e uma sequência de dígitos. 
A notação científica ́e possível para números ponto flutuantes utilizando um 'E' ou 'e' seguindo de um número positivo ou negativo inteiro.
```
1.0
-2.5e-3
+1.0E+5
```
Literais do tipo bool podem ser false ou true.
```
false
true
```
Literais do tipo char são representados por um único caractere entre entre aspas simples.
```
'a'
' '
'+'
'\''
```
E Strings, representadas por uma sequencia de caracteres entre entre aspas duplas.
```
"a"
"exemplo de string"
"String com \"quotes\""
```

## Como executar
Dentro do diretório Etapa1:

```
make
./etapa1 < arquivo_teste.txt
```
