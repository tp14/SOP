#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int main(void){
    FILE *arq;
    char caracter;
    arq = fopen("lista-arqs", "r");
    float quantArqs=0;

    // calcualr a quantidades de arquivos
    while ((caracter=fgetc(arq))!=EOF) {
        if (caracter=='\n') 
            quantArqs++;      
    }
    rewind(arq);

    // calcular a quantidade de arquivos com tamanho zero, o tamanho de todos os arquivos e o tamanho do maior arquivo
    char linha[400], *pLinha;
    char *pArq, *pTam;
    float tam=0, maior=0, tamZero=0, somaTotal=0;
    for (int i=0; i<quantArqs; i++ ){
        pLinha=fgets(linha,400,arq);

        pLinha = strtok(pLinha, " ");
        pArq = pLinha;
        pLinha = strtok(NULL, " ");
        pTam = pLinha;
        tam = atof(pTam);
        if(tam>maior)
            maior=tam;
        if(tam==0)
            tamZero++;  
        somaTotal+=tam;    
    }
    rewind(arq);

    // calcular mediana e preencher vetores que serão utilizados
    float media = somaTotal/quantArqs, quantArqMenorIgualMedia=0, mediana=0;
    float v[(int) quantArqs], v2[(int) quantArqs], v3[(int) quantArqs];
    int meio=quantArqs/2-1, meio2=-1;

    if((int)quantArqs%2==0)  
        meio2 = meio+1;

    for (int i=0; i<quantArqs; i++ ){
        pLinha=fgets(linha,400,arq);
        pLinha = strtok(pLinha, " ");
        pArq = pLinha;
        pLinha = strtok(NULL, " ");
        pTam = pLinha;
        tam = atoi(pTam);
        v[i] = tam;
        v2[i] = tam;
        v3[i] = tam;
        if(tam <= media)
            quantArqMenorIgualMedia++;

        if(i==meio && meio2<0)
            mediana = tam;
                
        if(i==meio && meio2>0)
            mediana+=tam/2;
            
        if(i==meio2)
            mediana+=tam/2;
    }
    rewind(arq);

    // calculando a soma dos 50% arquivos menores 
    float metadeArq[(int) quantArqs/2], menor=0, somaMetadeMenores=0;
    int j=0, quantArqOcuparia=0; 
    while(j<quantArqs/2){
        for (int i=0; i<quantArqs; i++ ){
            if(v[i]==menor){
                 somaMetadeMenores+=v[i];
                j++;
                v[i]=-1;
            }     
        }
       menor++;
    }

    // calculando quantos arquivos cabem em um bloco de 32MB
    float somaAqrsBloco = somaMetadeMenores;
    while(somaAqrsBloco<33554432){
       for (int i=0; i<quantArqs; i++ ){
            if(v[i]==menor){
                 somaAqrsBloco+=v[i];
                j++;
                v[i]=-1;
            }     
        }
       menor++;
    }

    // calculando a soma de todos os arquivos e o tamanho da fragmentação
    float tamTodosArqs=0, tamFragArqs=0; 
    for(int i=0; i<quantArqs; i++){
        tamTodosArqs+=v2[i];
        while(v2[i]>0){
            if(v2[i]>4096)
                v2[i]-=4096;
            else{
                tamFragArqs+=4096-v2[i];
                v2[i]-=v2[i];
            }
        }
    }

    // calculando quantos arquivos utilizam cada tipo de bloco
    int blocos=0, blocosSimples=0, blocosDuplos=0, blocosTriplos=0;
    for(int i=0; i< quantArqs; i++){
        if(v3[i]< 12*4096)
            blocos++;
        else if(v3[i]< (4096/4)*4096)
            blocosSimples++;
        else if(v3[i]< pow((4096/4),2)*4096)
            blocosDuplos++;
        else
            blocosTriplos++;    
    }
    
    // pegando o nome do maior arquivo a partir do seu tamanho
    tam=-1;
    while (tam!=maior){
        pLinha=fgets(linha,400,arq);
        pLinha = strtok(pLinha, " ");
        pArq = pLinha;
        pLinha = strtok(NULL, " ");
        pTam = pLinha;
        tam = atoi(pTam);
    }
 
    printf("a) Total de arquivos encontrados:%.0f\n", quantArqs);
    printf("b) Maior:%.0f Arquivo:%s\n", maior, pArq);
    printf("c) Arquivos com tamanho zero:%.0f Porcentagem do total:%.2f%%\n", tamZero, (tamZero/quantArqs)*100);
    printf("d) Tamanho Médio de arquivo:%.2f Porcentagem:%.2f%%\n", media, (quantArqMenorIgualMedia/quantArqs)*100);
    printf("e) Mediana:%.2f\n", mediana);
    printf("f) Total da soma dos 50%% arquivos menores:%.0f\n Quantidade de arquivos:%d Porcentagem:%.2f%%\n", somaMetadeMenores, j-1, (j/quantArqs)*100);
    printf("g) blocos:%.2f%%   blocosSimples:%.2f%%   blocosDuplos:%.3f%%   blocosTriplos:%.2f%%\n", (blocos/quantArqs)*100, (blocosSimples/quantArqs)*100, (blocosDuplos/quantArqs)*100, (blocosTriplos/quantArqs)*100);
    printf("h) Tamnaho total de todos arquivos:%.0f Tamanho da fragmentação:%.0f Porcentagem: %.2f%%\n", tamTodosArqs, tamFragArqs, (tamFragArqs/tamTodosArqs)*100);
    fclose(arq);
    return(0);
}