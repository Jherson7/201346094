#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <discos.h>

//declaracion de prototipos de los metodos que voy a requerir
void crearDisco(char token[], int posIni);
void inicio();
void buildDisk(char *nombre,char *ruta,int tam,int unidad);
//variables globales para la creacion de disco
char unidad[2];
char path[200]="";
char name[20]="";
int tamanio=0;

int main()
{
    inicio();
    return 0;
}

void inicio(){
int n=0;

    char *token;

    while(n==0)
    {
        printf("Ingrese comandos a Ejecutar!\n");
        char comando[200]="";
        fgets(comando,200,stdin);
        strcpy(comando,"Mkdisk -size::2 -path::\"/home/jherson/Escritorio/fa/\" -name::\"j.dsk\"");
        int o;
        char *primerComando=(char*)malloc(sizeof(10));
        for(o=0;o<200;o++){
            if(comando[o]==45||comando[o]==32|| comando[o]==43){
                primerComando[o]='\0';
                if(strcmp(primerComando,"Mkdisk")==0){
                    crearDisco(comando,o);
                    break;
                }else if(strcmp(primerComando,"Rmdisk")==0){


//(strcmp(comando[0],'-')==0)||(strcmp(comando[0],' ')==0)||(strcmp(comando[0],'+')==0)
              //  else if(strcmp(primerComando,"fdisk")==0)

               } else{
                    printf("Error comando no reconocido!\n");
                    break;
                    }

            }else{
                primerComando[o]=comando[o];
            }

        }


    }


}
void crearDisco(char token[200],int posIni){
    char *ins=malloc(sizeof(20));
    bool pat=false,nom=false,unin=false;
    int copi;
    char nombre[20]={0};
    char ruta[300]={0};
    int tam=0;
    char tama[200]={0};
    int unit=0;
    char ext[4]={0};
    for(copi=0;posIni<200|| token[posIni]!='\0';posIni++){
        if(token[posIni]=='-'||token[posIni]=='+')
        {
            posIni++;//aumento la variable para comenzar a copiar el comando
            while(token[posIni]!=':'){//ciclo hasta que encuentro el primer : para sacar su valor
               ins[copi]= token[posIni];
               copi++;
               posIni++;
            }
            ins[copi]='\0';//limito el caracter para poder compararlo
            if((strcmp(ins,"size")==0)||(strcmp(ins,"Size")==0)){
                posIni+=2;
                copi=0;//para volver a copiar otro char desde cero


                for(;posIni<200;posIni++,copi++){
                    if((token[posIni]=='-')||(token[posIni]=='+')||(token[posIni]==32)){
                        printf("entro no se porque\n");
                        break;
                    }

                    tama[copi]=token[posIni];
                }
                tama[copi]='\0';
                tam =atoi(tama);//conversion a int del tamanio del disco
                posIni--;
                copi=0;
            }else if(strcmp(ins,"path")==0||strcmp(ins,"Path")==0){
                posIni+=2;
                copi=0;
                path[copi]='"';
               for(;posIni<200;posIni++,copi++){
                    if((token[posIni]=='-')||(token[posIni]=='+')||(token[posIni]==32)){
                        printf("entro no se porque\n");
                        break;
                    }

                      ruta[copi]=token[posIni];
                }

                path[copi]='\0';
                pat=true;
                copi=0;
            }
            else if(strcmp(ins,"name")==0||strcmp(ins,"Name")==0){
                posIni+=2;
                copi=0;
                if(token[posIni]!='"'){
                    printf("Error el nombre debe venir dentro de comillas");
                    break;
                }
                posIni++;
                while(token[posIni]!='.'){//falta condicion para que no se salga del tamanio de comando
                    nombre[copi]=token[posIni];
                    copi++;
                        posIni++;
                }
                posIni++;
                copi=0;

              for(;posIni<200;posIni++,copi++){
                     if((token[posIni]=='-')||(token[posIni]=='+')||(token[posIni]==32)||(token[posIni]=='\0')||(token[posIni]=='"')){
                         printf("entro no se porque\n");
                         break;
                     }

                       ext[copi]=token[posIni];
                 }

              ext[copi]='\0';
                if(strcmp(ext,"dsk")==0){
                    strcat(nombre,".");
                    strcat(nombre,ext);
                    nom=true;
                }
                else{
                    printf("Error en la extension del Disco: %s!\n",ext);
                    break;
                }
            posIni++;
            copi=0;
            }
            else if(strcpy(ins,"unit")==0||strcpy(ins,"Unit")==0){
                posIni+=3;
                if(strcpy(token[posIni],"M")==0){
                    unit=1;
                    unin=true;
                }else if(strcpy(token[posIni],"K")==0){
                     unit=2;
                     unin=true;
                }
                else{
                    printf("Error en la especificacion de unidad de tamanio %c",token[posIni]);
                    break;
                }
                copi=0;
            }
            else{
                printf("Error en comando !%s! no reconocido!\n",ins);
                break;
            }

        }
//token[posIni]!=' '||token[posIni]!='-'||token[posIni]!='+'

        /*

*/
    }
    if(pat&&nom&&(tam>0)){
        buildDisk(nombre,ruta,tam,unit);
    }else{
        printf("Error en la entrada de texto porfavor intente de nuevo!\n");
    }


}

void buildDisk(char *nombre, char *ruta, int tam, int unidad){

    char directory[200]={0};
    strcpy(directory,"mkdir -p ");
    strcat(directory,ruta);

    const int dir_err = system(directory);
    if (-1 == dir_err)
    {
        printf("Error creating directory!n");
        exit(1);
    }
    else{
        FILE* fichero;//archivo que simulara el disco duro

        //creacion del tiempo en forma de char
        time_t tiempo = time(0);
        struct tm *tlocal = localtime(&tiempo);
        char output[19];
        strftime(output,19,"%d/%m/%y %H:%M:%S",tlocal);
        //termina creacion de tiempo
        //inicio a quitarles las comillas a la ruta para poder crear el archivo-------------------
        int k;
        int v=0;
        for(k=1;ruta[k]!='"';k++){
            v++;
        }
        v++;
        ruta[v++]='\0';
        strcat(ruta,nombre);
        char *nueva=(char*)malloc(200);
        for(k=1;ruta[k]!='\0';k++){
            nueva[k-1]=ruta[k];
        }
        strcat(nueva,"\0");
        //termino de copiar la ruta sin comillas-------------------------

        MasterBR temp;

        if(unidad==2){//por si el tamanio es en Kilobytes
          tam=tam*1024;
        }
        else{
          tam=tam*1024*1024;
        }

        //-------------creacion del disco

        fichero = fopen(nueva, "ab");
        //vamos a copiar los parametros a MBR
        strcpy(temp.mbr_fecha_creacion,output);//copio la fecha de creacion al MBR
        temp.mbr_tamanio=(tam-sizeof(temp));//le asigno el tamanio del disco restando el tamanio del mbr
        temp.mbr_disk_signature=1;
        //---------------
         basura tempBasura;
        int j;
        for(j=0; j<4; j++){
            strcpy(temp.mbr_particion_[j].part_status,"0");
            temp.mbr_particion_[j].part_type=malloc(1);
            strcpy(temp.mbr_particion_[j].part_type,"");
            strcpy(temp.mbr_particion_[j].part_fit,"");
            temp.mbr_particion_[j].part_start=0;
            temp.mbr_particion_[j].part_size=0;
            strcpy(temp.mbr_particion_[j].part_name,"");
            temp.mbr_particion_[j].part_id=j+1;
        }
        fseek(fichero,0,SEEK_SET);
        fwrite(&temp,sizeof(MasterBR),1,fichero);
        //---------------
        int i;
        tam=(tam-sizeof(MasterBR))/sizeof(basura);
        for(i=0; i< tam; i++){
            fseek(fichero,0,SEEK_END);
            strcpy(tempBasura.espacio,"");
            fwrite(&tempBasura,sizeof(basura),1,fichero);
            fflush(fichero);
        }

       fclose(fichero);
       fichero = fopen(nueva, "r");
       fseek(fichero,0,SEEK_SET);
       MasterBR tempMBR;
       fread(&tempMBR,sizeof(MasterBR),1,fichero);
       printf("Informacion del disco:\nTamanio: %d\nFecha Creacion: %s\n",tempMBR.mbr_tamanio,tempMBR.mbr_fecha_creacion);
       fclose(fichero);
    }
    /*
    int p= mkdir(ruta, 0777);
    if(p<0){
        printf("Error en la ruta de directorio: %s porfavor intente de nuevo\n",ruta);
        return;
    }
        char cero='0';
        for(v=0;v<tam*1024*1024;v++){
           fseek(fichero,0,SEEK_END);
           fputc('0',fichero);
           fflush(fichero);
        }
        fclose(fichero)

*/

}
