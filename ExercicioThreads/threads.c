#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <syscall.h>

//iniciando a barreira, mutex e variáveis globais 
pthread_barrier_t bar;
pthread_mutex_t mut;
int encontrado = 0, k=0, randon, **matriz, *ganhador_rodada;

//criando o struct para as threads 
typedef struct argus {
    int id, lin, colu;
} argu;

void *busca_matriz_thr1(void *argumentos);
void *busca_matriz_thr2(void *argumentos);
void *busca_matriz_thr3(void *argumentos);
void *busca_matriz_thr4(void *argumentos);

int main (int argc, char *argv[]){
    int i,j;

    //ler dados 
    int m = strtol(argv[1], NULL, 10);
    int n = strtol(argv[2], NULL, 10);
    int r = strtol(argv[3], NULL, 10);
    
    //iniciando matriz 
    matriz = (int**)malloc(m*sizeof(int*));

    for(i=0; i<m; i++){
        matriz[i] = (int*)malloc(n*sizeof(int));
    } 
   
    //gerando números aleatórios 
    srand(time(NULL));
    int v, k=0, lim=m*n, teste=0; 
    int valores[lim];
  
    while( k != lim){
        v = rand() % lim+1;
        if(v != 0){
            for(i=0; i<k; i++){
                if(valores[i] == v){
                    teste++;
                }
            }
            if(teste == 0){
                valores[k] = v;
                k++;
            }
            teste = 0;
        }
    }

    //preenchendo matriz com números aleatórios
    k=0;
    for(i=0; i<m; i++){
        for( j=0; j<n; j++){
            matriz[i][j] = valores[k];
            k++;
        }
    } 

    //criando as threads 
    pthread_t thr[4];

    //criando os paramentros pras threads 
    argu thr_arg;

    //iniciando vetor de ganhadores
    ganhador_rodada = (int*)malloc(r*sizeof(int));
    for(i = 0; i < r; i++) {
        ganhador_rodada[i] = 0;
    }

    //inciando mutex e barreira
    pthread_mutex_init(&mut, NULL);
    pthread_barrier_init(&bar, NULL, 4);

    /* iniciando as threads de maneira aleatória para evitar "vicíos" de ganhadores na busca do número
    sorteado, pois se sempre inicar com a mesma thread mesmo usando a barreira ela tem "vantagem" sobre
    as outras e acaba ganhando em maior quantidade */
    int rc, t=0, aux, vet[4];
    teste=0, k=0;
    for(i=0; i<r; i++){
        randon = rand() % lim+1;
        encontrado = 0; 
        t=0;
        while(t<4){
            aux = rand() % 4+1;
            for(j=0;j<4;j++){
                if(vet[j] == aux)
                    teste++;
            }
            if(teste == 0){
                vet[t] = aux; 
                t++;
                if(aux == 1){
                    pthread_mutex_lock(&mut);
                    thr_arg.id = 1;
                    thr_arg.lin = m;
                    thr_arg.colu = n;
                    rc = pthread_create(&thr[1], NULL, busca_matriz_thr1, &thr_arg);
                }
                if(aux == 2){
                    pthread_mutex_lock(&mut);
                    thr_arg.id = 2;
                    thr_arg.lin = m;
                    thr_arg.colu = n-1;
                    rc = pthread_create(&thr[2], NULL, busca_matriz_thr2, &thr_arg);
                }
                if(aux == 3){
                    pthread_mutex_lock(&mut);
                    thr_arg.id = 3;
                    thr_arg.lin = m-1;
                    thr_arg.colu = n;
                    rc = pthread_create(&thr[3], NULL, busca_matriz_thr3, &thr_arg);
                }
                if(aux == 4){
                    pthread_mutex_lock(&mut);
                    thr_arg.id = 4;
                    thr_arg.lin = m-1;
                    thr_arg.colu = n-1;
                    rc = pthread_create(&thr[4], NULL, busca_matriz_thr4, &thr_arg);
                }
            }
            teste=0;
        }

        for( j=1; j<5; j++){
            pthread_join(thr[j], NULL);
            vet[j-1]=0;
        }
            
    }

    //destruindo mutex e barreira
    pthread_mutex_destroy(&mut);
    pthread_barrier_destroy(&bar);

    //calculando as vitórias de cada thread
    int v1=0, v2=0, v3=0, v4=0;
    for(i=0;i<r;i++){
        if(ganhador_rodada[i] == 1)
            v1++;
        if(ganhador_rodada[i] == 2)
            v2++;
        if(ganhador_rodada[i] == 3)
            v3++;
        if(ganhador_rodada[i] == 4)
            v4++;
    }
            
    printf("thread %d => %d vitorias\n", 1, v1);
    printf("thread %d => %d vitorias\n", 2, v2);
    printf("thread %d => %d vitorias\n", 3, v3);
    printf("thread %d => %d vitorias\n", 4, v4);
    printf("----------------------\n");

    //calculando as threads vencedoras 
    aux = v1;
    if(v4 > aux)
        aux = v4;
    if(v3 > aux)
        aux = v3;
    if(v2 > aux)
        aux = v2;

    printf("Thread(s) vencedora(s): ");
    if(v1 == aux)
        printf("1 ");
    if(v2 == aux)
        printf("2 ");
    if(v3 == aux)
        printf("3 ");
    if(v4 == aux)
        printf("4 ");  
    printf("\n");  
    
    return 0;
}

//implementação das funções de busca de cada thread a partir de sua posição inicial
void *busca_matriz_thr1(void *argumentos){
    int i, j, a, b, id;
    argu *aux = argumentos;
    argu thr_arg = *aux;
    id = thr_arg.id;
    a = thr_arg.lin;
    b = thr_arg.colu;
    pthread_mutex_unlock(&mut);
    pthread_barrier_wait(&bar); 
    for(i = 0; i < a; i++) {
        for(j = 0; j < b; j++) {
            if(matriz[i][j] == randon) {
                pthread_mutex_lock(&mut);
                if(encontrado == 0) {
                    ganhador_rodada[k] = id;
                    k++;
                    encontrado = 1;
                }
                pthread_mutex_unlock(&mut);
                pthread_exit(NULL);
            }
        }
    }
}

void *busca_matriz_thr2(void *argumentos){
    int i, j, a, b, id;
    argu *aux = argumentos;
    argu thr_arg = *aux;
    id = thr_arg.id;
    a = thr_arg.lin;
    b = thr_arg.colu;
    pthread_mutex_unlock(&mut);
    pthread_barrier_wait(&bar); 
    for(i = 0; i < a; i++) {
        for(j = b; j >= 0; j--) {
            if (matriz[i][j] == randon) {  
                pthread_mutex_lock(&mut);
                if(encontrado == 0) {
                    ganhador_rodada[k] = id;
                    k++;
                    encontrado = 1;
                }
                pthread_mutex_unlock(&mut);
                pthread_exit(NULL);
            }
        }
    }
}

void *busca_matriz_thr3(void *argumentos){
    int i, j, a, b, id;
    argu *aux = argumentos;
    argu thr_arg = *aux;
    id = thr_arg.id;
    a = thr_arg.lin;
    b = thr_arg.colu;
    pthread_mutex_unlock(&mut);
    pthread_barrier_wait(&bar); 
    for(i = a; i >= 0; i--) {
        for(j = 0; j < b; j++) {
            if(matriz[i][j] == randon) {
                pthread_mutex_lock(&mut);
                if(encontrado == 0) {
                    ganhador_rodada[k] = id;
                    k++;
                    encontrado = 1;
                }
                pthread_mutex_unlock(&mut);
                pthread_exit(NULL);
            }
        }
    }
}

void *busca_matriz_thr4(void *argumentos){
    int i, j, a, b, id;
    argu *aux = argumentos;
    argu thr_arg = *aux;
    id = thr_arg.id;
    a = thr_arg.lin;
    b = thr_arg.colu;
    pthread_mutex_unlock(&mut);
    pthread_barrier_wait(&bar); 
    for(i = a; i >= 0; i--) {
        for(j = b; j >= 0; j--) {
            if(matriz[i][j] == randon) {
                pthread_mutex_lock(&mut);
                if(encontrado == 0) {
                    ganhador_rodada[k] = id;
                    k++;
                    encontrado = 1;
                }
                pthread_mutex_unlock(&mut);
                pthread_exit(NULL);
            }
        }
    }
}

            
            