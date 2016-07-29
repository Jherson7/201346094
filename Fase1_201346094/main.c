#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//declaracion de prototipos de los metodos que voy a requerir
void crearDisco(char * token);


int main()
{
    int n=0;

    char *token;

    while(n==0)
    {
        printf("Ingrese comandos a Ejecutar!\n");
        char comando[200]="";
        fgets(comando,200,stdin);
        const char s[2] = " ";
        char *token;

        token = strtok(comando, s);

        while( token != NULL )
        {
            printf( " %s\n", token );
            if(strcmp(token,"Mkdisk")==0 || strcmp(token,"mkdisk")==0){
                    token = strtok(NULL, s);
                    crearDisco(token);
            }
            token = strtok(NULL, s);
        }

    }


    return 0;
}

void crearDisco(char * token){
printf("imprimo desde el metodo\n");
const char s[2] = " ";
    while( token != NULL )
    {

        printf( " %s\n", token );
        token = strtok(NULL, s);

    }
}
