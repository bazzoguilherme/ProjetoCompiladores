# E6

## Grupo J
- Guilherme T. Bazzo
- Gustavo A. Lorentz

## Relatório do trabalho de Compiladores - Etapa 6

Geração de código assembly sendo feita sobre o código intermediário gerado na etapa5.

Pequenas alterações foram feitas do codigo iloc da etapa anterior, como fazer a pilha crescer para baixo. Houve também a adição de um campo "tipo" no código com o intuito de realizar uma "anotação" do que o código está fazendo e o que as instruções a seguir realizam. Isso se torna útil na hora da chamada de função ou retorno, pois conseguimos remover do print instruções do iloc que aqui são inúteis.

### Registradores
Os registradores da etapa anterior estão sendo "ignorados" nessa etapa.
Como haviam "infinitos" registradores na etapa anterior, o grupo optou por não tentar nenhuma abordagem mais sofisticada para diminuir a utilização deles.

Nesse sentido, o grupo utiliza a própria pilha na memória para realizar as operações, empilhando e desempilhando os valores conforme necessidade.