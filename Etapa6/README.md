# E6

## Relatório do trabalho de Compiladores - Etapa 6

Geração de código assembly sendo feita sobre o código intermediário gerado na etapa5.

### Registradores
Os registradores da etapa anterior estão sendo "ignorados" nessa etapa.
Como haviam "infinitos" registradores na etapa anterior, o grupo optou por não tentar nenhuma abordagem mais sofisticada para diminuir a utilização deles.

Nesse sentido, o grupo utiliza a própria pilha na memória para realizar as operações, empilhando e desempilhando os valores conforme necessidade.