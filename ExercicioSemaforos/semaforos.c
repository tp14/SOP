#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sched.h>

sem_t sa;
sem_t sb;
sem_t sc;
sem_t sd;
sem_t se;
sem_t sf;

void* A(void *args);
void* B(void *args);
void* C(void *args);
int main() {

sem_init(&sa, 0, 0);
sem_init(&sb, 0, 0);
sem_init(&sc, 0, 0);
sem_init(&sd, 0, 0);
sem_init(&se, 0, 0);
sem_init(&sf, 0, 0);

pthread_t threads[3];
int i;

pthread_create(&threads[0],NULL,A,NULL);
pthread_create(&threads[1],NULL,B,NULL);
pthread_create(&threads[2],NULL,C,NULL);

for(i=0;i<3;i++){
    pthread_join(threads[i], NULL);
}

return 0;
}

void* A(void *args) {
    while(1) {
        sem_wait(&sf);
        sem_wait(&sa);
        printf("O");
        sem_post(&sb);
        sem_wait(&sc);
        sem_post(&sd);
        sem_wait(&se);
    }
}

void* B(void *args) {
    while(1) {
        sem_post(&sf);
        sem_post(&sf);
        printf("CA");
        sem_post(&sa);
        sem_post(&sa);
        sem_wait(&sb);
        sem_wait(&sb);
        printf("!\n");
        sem_post(&sc);
        sem_post(&sc);
    }
}

void* C(void *args) {
    while(1) {
        sem_wait(&sf);
        sem_wait(&sa);
        printf("S");
        sem_post(&sb);
        sem_wait(&sc);
        sem_post(&se);
        sem_wait(&sd);
    }
}