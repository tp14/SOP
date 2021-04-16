#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct linha linha;

struct linha{
    int blocoInicial;
    int numBlocos;
    char op;
};

linha teste(linha novo);

linha *l;
int cont=1;

int main(){

    l = malloc(sizeof(linha));
    while(1){
        int blocoInicial;
        int numBlocos;
        char op;
        linha novo;

        scanf("%d", &blocoInicial);
        if(blocoInicial == -1)
            break;
        else{
        scanf("%d %c", &numBlocos, &op);

        novo.blocoInicial = blocoInicial;
        novo.numBlocos = numBlocos;
        novo.op = op;
        novo = teste(novo);

        l = realloc(l, cont*sizeof(linha));
        l[cont-1] = novo;
        cont++;
        }
    }

    linha aux;
    for(int i=0; i<cont-1; i++){
        for(int j=0; j<cont-1; j++){
            if (l[i].blocoInicial < l[j].blocoInicial){     
                aux = l[i];
                l[i] = l[j];
                l[j] = aux;
            }
        }
    }    
    printf("Fila:\n");
    for(int i=0; i<cont-1; i++)
        printf("%d %d %c\n", l[i].blocoInicial, l[i].numBlocos, l[i].op);
     
    return 0;
}

linha teste(linha novo){

    if(novo.op == 'r'){
        for(int i=0; i<cont; i++){
            if(l[i].op == 'r'){
                int somaBlocos, dist, inicio, novoNum, alcance;
                somaBlocos = l[i].numBlocos + novo.numBlocos;

                if(l[i].blocoInicial>novo.blocoInicial){
                    dist = l[i].blocoInicial-novo.blocoInicial;
                    alcance = novo.numBlocos;
                    inicio = novo.blocoInicial;
                    if(novo.blocoInicial + alcance == l[i].blocoInicial)
                        novoNum = alcance + l[i].numBlocos;
                    else if (alcance > l[i].blocoInicial + l[i].numBlocos)
                        novoNum =  alcance;
                    else 
                        novoNum = alcance + l[i].numBlocos - (alcance - dist);
                }else{
                    dist = novo.blocoInicial - l[i].blocoInicial;
                    alcance = l[i].numBlocos;
                    inicio = l[i].blocoInicial;
                    if(l[i].blocoInicial + alcance == novo.blocoInicial)
                        novoNum = alcance + novo.numBlocos;
                    else if (alcance > novo.blocoInicial + novo.numBlocos)
                        novoNum =  alcance;
                    else 
                        novoNum = alcance + novo.numBlocos - (alcance - dist);
                }
                if(somaBlocos < 64){
                    if(dist <= alcance){
                        novo.blocoInicial = inicio;
                        novo.numBlocos = novoNum;

                        for(i; i<cont-1; i++)
                            l[i] = l[i+1];

                        cont--;
                        l = realloc(l, cont*sizeof(linha));
                        novo = teste(novo);
                    }
                }
            }
        }
    }else{
        for(int i=0; i<cont; i++){
            if(l[i].op == 'w'){
                int somaBlocos, dist, inicio, novoNum, alcance;

                somaBlocos = l[i].numBlocos + novo.numBlocos;

                if(l[i].blocoInicial>novo.blocoInicial){
                    dist = l[i].blocoInicial-novo.blocoInicial;
                    alcance = novo.numBlocos;
                    inicio = novo.blocoInicial;
                    novoNum = alcance + l[i].numBlocos;
                }else{
                    dist = novo.blocoInicial - l[i].blocoInicial;
                    alcance = l[i].numBlocos;
                    inicio = l[i].blocoInicial;
                    novoNum = alcance + novo.numBlocos;
                }
                if(somaBlocos < 64){
                    if(dist == alcance){
                        novo.blocoInicial = inicio;
                        novo.numBlocos = novoNum;

                        for(i; i<cont-1; i++)
                            l[i] = l[i+1];

                        cont--;
                        l = realloc(l, cont*sizeof(linha));
                        novo = teste(novo);
                    }
                }
            }
        }
    }
    
    return novo;
}