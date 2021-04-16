# SOP

Enunciado dos exercícios:

1-Arquivos
Uma das decisões de projeto em sistemas de arquivos é a escolha do tamanho dos blocos lógicos. Como
discutido em aula, blocos maiores oferecem maior eficiência na transferência de dados de/para o 
dispositivo de armazenamento, mas geram maior desperdício de espaço devido a fragmentação interna.
Outra questão diz respeito à necessidade de escolher estruturas de metadados que comportem arquivos
de diferentes tamanhos. Neste exercício você irá coletar dados do seu sistema de arquivos para raciocinar
sobre essas questões.
Primeiro, gere uma lista de arquivos em /usr/bin e /usr/share e seus respectivos tamanhos, usando o
comando abaixo:
$ find /usr/bin /usr/share -type f -exec ls -l {} \; 2>/dev/null | \
awk ’{print "\""$9"\"", $5}’ | tee /tmp/lista-arqs
Ao final da execução, o arquivo /tmp/lista-arqs conterá duas colunas, a primeira com os nomes de
arquivos e a segunda com seus tamanhos (em bytes).
Com base nos dados coletados, responda às seguintes perguntas:
(a) Quantos arquivos foram encontrados?
(b) Qual o maior tamanho de arquivo observado? A que arquivo corresponde?
(c) Quantos arquivos com tamanho zero existem? A que porcentagem do total eles correspondem?
(d) Qual o tamanho médio de arquivo? Qual a porcentagem de arquivos com tamanho igual ou menor
do que a média?
(e) Qual a mediana do tamanho de arquivo?
(f) Qual o menor tamanho de bloco necessário para que pelo menos 50% dos arquivos ocupem apenas
um bloco? Qual a porcentagem de arquivos que ocuparia um bloco se esse tamanho fosse adotado?
Quantos blocos (com o tamanho encontrado) ocuparia o maior arquivo?
NOTA: o tamanho de bloco deve ser uma potência de 2 (512 bytes, 1 KB, 2 KB, 4 KB, 8 KB, . . . )
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

2-Escalonador de Disco
Escreva um programa C que simule parte de um escalonador de disco. O programa deve atender aos
seguintes requisitos:
R1. O programa deve ler requisições de disco da entrada padrão (uma requisição por linha). O formato
das requisições é mostrado abaixo, juntamente com alguns exemplos:
bloco inicial   no. de blocos   operação
101             5               r
75              4               w
106             3               r
99              2               w
109             60              r

O bloco inicial corresponde ao primeiro bloco a ser lido ou escrito, e o no. de blocos contém a quantidade de blocos
(limitada a 64 por requisição); ambos são valores inteiros sem sinal. A operação pode
ser r, para leitura, e w, para escrita. Nos exemplos, a primeira requisição solicita a leitura dos blocos
101 a 105, e a segunda a escrita dos blocos 75 a 78.
R2. A leitura encerra quando o bloco inicial for −1. Essa requisição deve ser ignorada.
R3. As requisições lidas devem ser inseridas em uma fila de despacho, que deve ser ordenada pelo
número do bloco.
R4. Requisições com a mesma operação que sejam adjacentes devem ser fundidas em uma única requisição, respeitando 
o limite para o número de 64 blocos por requisição. Também devem ser fundidas
requisições de leitura onde haja sobreposição de blocos.
R5. As requisições devem ser fundidas à medida em que forem sendo lidas (i.e., não apenas ao final da
leitura).
R6. Não há limite predefinido para o tamanho da lista de requisições.
R7. Quando a leitura for encerrada, o programa deve imprimir uma linha contendo “Fila:” e exibir o
conteúdo da lista, uma requisição por linha, com campos separados por um espaço (veja o exemplo
abaixo).
Exemplo de execução do programa:
$ ./req-disco
100 2 r
75 4 w
106 5 r
9 4 w
99 2 w
111 60 r
101 5 r
6 3 w
10 2 w
-1
Fila:
6 7 w
10 2 w
75 4 w
99 2 w
100 11 r
111 60 r

Nesse exemplo, as requisições com blocos iniciais 100, 106 e 101 foram fundidas em uma única requisição
de 11 blocos, começando no 100. A requisição de bloco inicial 99 não pôde ser fundida por ser de escrita.
Se a requisição de bloco inicial 111 fosse fundida com a de bloco inicial 106 (são adjacentes), a requisição
resultante extrapolaria o limite de 64 blocos.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

3-Semaforos
Escreva um programa em C no Linux usando a biblioteca Pthreads e que atenda aos seguintes requisitos:
R1. O programa recebe três parâmetros m, n e r na linha de comando; ou seja, o programa deve ser
invocado como:
$ ./prog m n r

R2. O programa deve alocar dinamicamente uma matriz A de m × n números inteiros, onde m é o número de linhas 
e n é o número de colunas, e preenchê-la com números entre 1 e m × n. Os números
devem estar em posições aleatórias (isto é, dispostos de forma não sequencial), e não podem ser
repetidos (sugestão: preencha a matriz sequencialmente e depois faça um rearranjo aleatório dos
elementos). Por exemplo, para m = 2 e n = 3, a matriz abaixo seria válida:
5 2 1
6 3 4

R3. O programa deve criar quatro threads, todas elas executando a mesma função (descrita mais à frente).
R4. Cada thread deve receber como parâmetros, pelo menos, o seu número de identificação (de 1 a 4) e
um outro parâmetro que permita determinar o ponto inicial de busca. Podem ser passados quaisquer
parâmetros adicionais, conforme a necessidade.
R5. O programa deve realizar r rodadas do seguinte procedimento:
i. main() escolhe um número aleatório entre 1 e m × n;
ii. as threads buscam (em paralelo) o número sorteado na matriz;
iii. a primeira thread a encontrar o número é considerada a vencedora da rodada.
R6. Cada thread deve buscar o número sorteado por main() partindo de um canto diferente da matriz
(A11, A1n, Am1, Amn) e percorrendo todos os elementos até encontrar o número procurado. Como a
matriz contém todos os números entre 1 e m × n, o número sorteado sempre será encontrado.
R7. Todas as threads devem iniciar a busca ao mesmo tempo, depois que o número for sorteado por
main().
R8. A primeira thread que encontrar o número deve inserir o seu número de identificação em uma lista
com a vencedora de cada rodada (i.e., o k-ésimo elemento da lista será a thread mais rápida da k-ésima
rodada). Essa lista pode ser estática ou dinâmica.
R9. Depois de encontrar o número sorteado, uma thread deve bloquear até que outro número seja sorteado, ou, se for a última rodada, até que a última thread termine o seu processamento. Em outras
palavras, uma thread só pode passar para a próxima rodada (ou finalizar) quando todas as threads tiverem concluído a busca. O programa principal (main()) deve esperar que todas as threads terminem
a rodada k antes de avançar para a rodada k + 1.
R10. Ao final das r rodadas, o programa principal deve mostrar o número de rodadas em que cada thread
foi a mais rápida, e declarar a(s) vencedora(s).
R11. O programa deve tratar condições de disputa no código.
O exemplo a seguir ilustra o formato esperado para a saída do programa (invocado com m = 30, n = 40
e r = 10) quando as threads 1 e 3 foram as mais rápidas em duas rodadas cada uma, e as threads 2 e 4
foram mais rápidas em três rodadas cada.
$ ./prog 30 40 10
thread 1 => 2 vitorias
thread 2 => 3 vitorias
thread 3 => 2 vitorias
thread 4 => 3 vitorias
-------------------------------
Thread(s) vencedora(s): 2 4
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

4-Threads
Considere um programa concorrente com três threads, A, B e C, mostradas no pseudocódigo abaixo.
Deseja-se que a sequência de execução seja tal que o programa imprima para toda a eternidade (ou
enquanto o programa executar) as mensagens “CASO!\n” ou “CAOS!\n”, dependendo da sequência de escalonamento 
(ou seja, em cada linha da saída as duas palavras devem ser igualmente possíveis). Mostre
como garantir isso usando semáforos. A solução não deve alterar a estrutura do pseudocódigo existente,
apenas incluir instruções de manipulação de semáforos para garantir a sequência de execução desejada.
Você pode inicializar os semáforos como variáveis regulares (semaphore x=1) e manipulá-los com os
pares de primitivas down()/up() ou sem_wait()/sem_post().
A1 void A() {
A2  while (1) {
A3    printf("O");
A4  }
A5 }

B1 void B() {
B2  while (1) {
B3    printf("CA");
B4    printf("!\n");
B5  }
B6 }

C1 void C() {
C2  while (1) {
C3    printf("S");
C4  }
C5 }
