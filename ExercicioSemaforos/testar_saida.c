#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (){
    int a=0, b=0, c=0, t = 1;

    FILE *f= NULL;
    
    if ((f = fopen("/home/pimenta/Documentos/saida", "r")) == NULL)
        printf ("Erro na abertura do arquivo.\n");
    else
        printf("Arquivo aberto com sucesso.\n");

    size_t len= 10; 
    char *linha= malloc(len);
    char *ta = "CAOS!",  *tb = "CASO!";

    while (getline(&linha, &len, f) > 0){
        t =  strcmp(linha, tb);
        printf("%d\n", t);
         if(t == -4)
            a++;
         if(t == 10)
            b++;

         c++;
    }

    printf("caos = %d caso = %d total=%d\n", a, b, c);
    if (linha)
        free(linha);

    fclose(f);

    return 0;
}