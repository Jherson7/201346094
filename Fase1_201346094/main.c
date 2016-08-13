#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "discos.h"
#include <unistd.h>

//declaracion de prototipos de los metodos que voy a requerir
void crearDisco(char token[], int posIni);
void inicio();
void buildDisk(char *nombre,char *ruta,int tam,int unidad);
void deleteDisk(char *token, int indice);
void comandoFseek(char *token,int posIni);
void crearParticion(char direcion[], char nombre[], int tam, int tipoAjuste, int acomodamiento , char tajuste[], char tacomodamiento[]);
void montarParticion(char token[], int posIni);
void eliminarParticion(char ruta[], char nombre[], int tipo);
void comandoExect(char token[], int posIni);
void Reportes(char token[], int posIni);
void ejecutarComandos(char comando[]);
void desmontar(char token[], int posIni);
//variables globales para la creacion de disco
char unidad[2];
char path[200]="";
char name[20]="";
int tamanio=0;
int indiceDiscos=0;
//MasterDisk DISCO;
char abecedario[27]={'\0','a','b','c','d','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
int main()
{
 FILE *ff;
 ff=fopen("/home/jherson/Escritorio/fa/manager.dsk","r+");
 if(ff){
   printf("Creado manejador de Discos!\n")  ;
 }
 else{
     int i;
     ff=fopen("/home/jherson/Escritorio/fa/manager.dsk","ab");
     MasterDisk tmp;
     for(i=0;i<27;i++){
         tmp.DISCOSS[i].llenas=0;
         strcpy(tmp.DISCOSS[i].nombre,"\0");
     }
     tmp.indicador=1;
     fseek(ff,0,SEEK_SET);
     fwrite(&tmp,sizeof(MasterDisk),1,ff);
     fclose(ff);
     usleep(20);

 }

    inicio();
    return 0;
}

void inicio(){
int n=0;

    char *token;
    char comandoAux[200]={0};
    while(n==0)
    {
        char comando[800]={0};
        bool ciclo=true;
        printf("Ingrese comandos a Ejecutar!\n");
        while(ciclo){

            fgets(comandoAux,200,stdin);

            int t;
            for(t=0;t<200;t++){
                if(comandoAux[t+1]=='\n'){
                    if(comandoAux[t]==92){
                        comandoAux[t]='\0';
                        strcat(comando,comandoAux);
                        strcpy(comandoAux,"\0");
                        break;
                    }else{
                        strcat(comando,comandoAux);
                        ciclo=false;
                        break;
                    }
                }
            }
        }

      //strcpy(comando,"Mkdisk -size::10 -path::\"/home/jherson/Escritorio/fa/\" -name::\"jh.dsk\" +unit::M");
      // strcpy(comando,"fdisk -size::2 +type::L +unit::M +fit::BF -path::\"/home/jherson/Escritorio/fa/uno.dsk\" -name::\"l13\"");
        //strcpy(comando,"mount -path::\"/home/jherson/Escritorio/fa/disco1.dsk\" -name::\"particion1\"");
      //strcpy(comando,"exec \"/home/jherson/Escritorio/fa/calificaciones.sh\"");
        int o;
        char *primerComando=malloc(sizeof(10));
        for(o=0;o<800;o++){
            if(comando[o]==45||comando[o]==32||comando[o]==43|| comando[o]=='\n'){
                primerComando[o]='\0';

                if(strcasecmp(primerComando,"Mkdisk")==0||strcasecmp(primerComando,"mkdisk")==0){
                    crearDisco(comando,o);
                    break;
                }else if(strcasecmp(primerComando,"rmdisk")==0||strcasecmp(primerComando,"rmDisk")==0){
                        deleteDisk(comando,o);
                        break;
               }
                else if(strcasecmp(primerComando,"fdisk")==0||strcasecmp(primerComando,"fDisk")==0){
                           comandoFseek(comando,o);;
                            break;
                }
                else if(strcasecmp(primerComando,"mount")==0||strcasecmp(primerComando,"mount")==0){
                    montarParticion(comando,o);;
                       break;
                } else if(strcasecmp(primerComando,"unmount")==0||strcasecmp(primerComando,"unMount")==0){
                    desmontar(comando,o);;

                    break;
                }
                else if(strcasecmp(primerComando,"exec")==0||strcasecmp(primerComando,"mount")==0){
                    comandoExect(comando,o);
                       break;
                }  else if(strcasecmp(primerComando,"rep")==0||strcasecmp(primerComando,"Rep")==0){
                    Reportes(comando,o);
                       break;
                }
                else{
                    printf("Error comando no reconocido!\n");
                    break;
               }
            }else{
              primerComando[o]=comando[o];
            }
        }
    }
}

void desmontar(char token[], int posIni){
    char aja[50];
    strcpy(aja,token);
    int copi=0;
    char ins[10]={0};
    FILE *disca;
    disca=fopen("/home/jherson/Escritorio/fa/manager.dsk","rb+");
    MasterDisk temporal;
    fread(&temporal,sizeof(MasterDisk),1,disca);
    usleep(200);
    fclose(disca);
    for(;(posIni<200)&&(token[posIni]!='\0')&& (token[posIni]!='\n');posIni++){
        copi=0;
        if(token[posIni]=='-')
        {
            posIni++;
            if(token[posIni]=='i'&& token[posIni+1]=='d'){
                if(token[posIni+3]==':'&&token[posIni+4]==':')
                {
                    posIni+=5;
                    for(;posIni<200;posIni++,copi++){
                       if(token[posIni]=='-'||token[posIni]==32||token[posIni]=='\0'||token[posIni]=='\n'){
                           break;
                       }
                       ins[copi]=token[posIni];
                  }
                    //--*-*-*aqui vamos a buscar el sd
                    ins[copi]='\0';

                    //----
//                    disca=fopen("/home/jherson/Escritorio/fa/manager.dsk","rb+");
//                    fseek(disca,0,SEEK_SET);
//                    fwrite(&DISCO,sizeof(MasterDisk),1,disca);
//                    fclose(disca);
                    //------

                    if(ins[0]=='v' && ins[1]=='d'){
                            int v;
                           for(v=0;v<27;v++){
                                if(ins[2]==abecedario[v])
                                    break;
                           }

                           char tam[2];
                           int h;
                           for(h=3;h<5;h++)
                               tam[h-3]=ins[h];
                           tam[h-3]='\0';
                           int noParticion=atoi(tam);
                           if(noParticion>0){
                               if(temporal.DISCOSS[v].llenas>0){
                                   if(strcasecmp(temporal.DISCOSS[v].particion[noParticion-1].nombre,"")!=0){//ver si le tengo que restar el uno
                                       strcpy(temporal.DISCOSS[v].particion[noParticion-1].nombre,"\0");
                                       temporal.DISCOSS[v].llenas--;
                                       disca=fopen("/home/jherson/Escritorio/fa/manager.dsk","rb+");
                                       fseek(disca,0,SEEK_SET);
                                       fwrite(&temporal,sizeof(MasterDisk),1,disca);
                                       usleep(200);
                                       fclose(disca);
                                       printf("Se desmonto correctamnte la particion vd%c%d\n",abecedario[v],noParticion);
                                   }
                               }else{
                                   printf("Error al desmontar particion no se encuentra!\n");
                                   break;
                               }
                           }else{
                               printf("El valor de sd%d es no es valido!ERROR!\n",ins[3]);
                           }
                    }
                    //-*-*-*-*-*-*-**-*-*-*-*-**-*-*-*
                }else
                    printf("Error en el comando unmount!!:: %s\n",token);

            }

        }
    }
}

//umount -id1::vda1 -id2::vdb2 -id3::vdc1
void comandoExect(char token[], int posIni){
    int copi=0;
    char ruta[100];
    for(;posIni<200&& token[posIni]!='\0'&& token[posIni]!='\n';posIni++){
        copi=0;
        if(token[posIni]=='\"')
        {
            for(;posIni<200;posIni++,copi++){
                if((token[posIni]=='-')||(token[posIni]==32)||(token[posIni]=='\n')||(token[posIni]=='\0'))
                    break;
                else
                    ruta[copi]=token[posIni];
            }
            ruta[copi]='\0';
            char jj[200];
            int va=1;
            for(;ruta[va]!='\"';va++)
                jj[va-1]=ruta[va];


            //-------------- aqui viene lo feo,
            FILE *archivo;
            char caracteres[200];
            archivo = fopen(jj,"r");
            if (archivo == NULL)
                printf("Error archivo no encontrado!\n");
            else{
                while (feof(archivo) == 0)
                {
                    fgets(caracteres,200,archivo);
                    char comando[800]={0};
                    bool aja=true;
                    if(caracteres[0]=='#'){
                        printf("%s",caracteres);
                    }else{
                        int v;
                        while(aja){
                            for(v=0;v<200;v++){
                                if(caracteres[v+1]=='\n'||caracteres[v+1]=='\r'){
                                    if(caracteres[v]==92){
                                        caracteres[v]='\0';
                                        strcpy(comando,caracteres);
                                        fgets(caracteres,200,archivo);
                                        break;
                                    }else{
                                        caracteres[v+1]='\0';
                                        strcat(comando,caracteres);
                                        printf("Comando::%s\n",comando);
                                        aja=false;
                                        break;
                                    }
                                }
                            }
                        }
                        ejecutarComandos(comando);

                    }
                }

            }
            //--------------
        }
    }
}

void crearDisco(char token[200],int posIni){
    char bueno[100];
    strcpy(bueno,token);
    char *ins=malloc(sizeof(20));
    bool pat=false,nom=false,unin=false;
    int copi;
    char nombre[20]={0};
    char ruta[300]={0};
    int tam=0;
    char tama[200]={0};
    int unit=1;
    char ext[4]={0};
    for(copi=0;posIni<800 && token[posIni]!='\0' && token[posIni]!='\n';posIni++){
        if(token[posIni]=='-'||token[posIni]=='+')
        {
            posIni++;//aumento la variable para comenzar a copiar el comando
            while(token[posIni]!=':'){//ciclo hasta que encuentro el primer : para sacar su valor
                ins[copi]= token[posIni];
                copi++;
                posIni++;
            }
            ins[copi]='\0';//limito el caracter para poder compararlo
            if((strcasecmp(ins,"size")==0)||(strcasecmp(ins,"Size")==0)){
                posIni+=2;
                copi=0;//para volver a copiar otro char desde cero
                if(token[posIni]=='-')
                    printf("Error en tamanio size! valor negativo\n");
                else{
                    for(;posIni<800;posIni++,copi++){
                        if((token[posIni]=='-')||(token[posIni]=='+')||(token[posIni]==32)){
                            break;
                        }

                        tama[copi]=token[posIni];
                    }
                    tama[copi]='\0';
                    tam =atoi(tama);//conversion a int del tamanio del disco
                    posIni--;
                    copi=0;
                }

            }else if(strcasecmp(ins,"path")==0||strcasecmp(ins,"Path")==0){
                posIni+=2;
                copi=0;
                if(token[posIni]!='"'){
                    printf("Error el nombre debe venir dentro de comillas");
                    break;
                }
                ruta[0]='\'';
                copi++;
                posIni++;
                for(;posIni<800;posIni++,copi++){
                    if((token[posIni]=='-')||(token[posIni]=='+')||(token[posIni]=='\"')||(token[posIni]=='\n')){
                        break;
                    }
                    ruta[copi]=token[posIni];
                }
                ruta[copi]='\"';
                pat=true;
                copi=0;
            }
            else if(strcasecmp(ins,"name")==0||strcasecmp(ins,"Name")==0){
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

                for(;posIni<800;posIni++,copi++){
                    if((token[posIni]=='-')||(token[posIni]=='+')||(token[posIni]=='\0')||(token[posIni]=='"')){
                        break;
                    }
                    ext[copi]=token[posIni];
                }

                ext[copi]='\0';
                if(strcasecmp(ext,"dsk")==0){
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
            else if(strcasecmp(ins,"unit")==0||strcasecmp(ins,"Unit")==0){
                char otro[90];
                int h;
                for(h=0;h<90;h++)
                    otro[h]=token[h];
                posIni+=2;
                if(token[posIni]=='M'||token[posIni]=='m'){
                    unit=1;
                    unin=true;
                }else if(token[posIni]=='K'||token[posIni]=='k'){
                    unit=2;
                    unin=true;
                }
                else{
                    printf("Error en la especificacion de unidad de tamanio %c",token[posIni]);
                    nom=false;tam=0;pat=false;
                    break;
                }
                copi=0;
            }
            else{
                printf("Error en comando !%s! no reconocido!\n",ins);
                break;
            }
        }
    }
    if(unit==1)
        tam=tam*1024*1024;
    else if(unit==2)
        tam=tam*1024;
    if(pat&&nom&&(tam>=10000000)){
        buildDisk(nombre,ruta,tam,unit);
    }else if(tam<10000000){
        printf("error el tamanio del disco debe ser mayor a 10M\n");
    }else{
        printf("Error en la entrada de texto porfavor intente de nuevo!\n");
    }
}

void buildDisk(char *nombre, char *ruta, int tam, int unidad){

    char directory[200]={0};
    int i;
    for(i=0;ruta[i];i++)
    {
        if(ruta[i]=='\"')
            ruta[i]='\'';
    }

    strcpy(directory,"mkdir -p ");
    strcat(directory,ruta);

    const int dir_err = system(directory);
    if (-1 == dir_err)
    {
        printf("Error creating directory!n");

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
//        int k;
//        int v=0;
//        for(k=1;ruta[k]!='"';k++){
//            v++;
//        }
//        v++;
//        ruta[v++]='\0';
        int k;
        char nueva[200]={0};
        ruta[0]='\"';
        for(k=1;ruta[k]!='\'';k++){
            nueva[k-1]=ruta[k];
        }
        strcat(nueva,nombre);
        MasterBR temp;

        //        if(unidad==2){//por si el tamanio es en Kilobytes
        //          tam=tam*1024;
        //        }
        //        else{
        //          tam=tam*1024*1024;
        //        }

        //-------------creacion del disco
        fichero=fopen(nueva,"r+");
        if(fichero==NULL){
          //fclose(fichero);
            fichero = fopen(nueva, "ab");
            if(fichero){
                //vamos a copiar los parametros a MBR
                strcpy(temp.mbr_fecha_creacion,output);//copio la fecha de creacion al MBR
                temp.mbr_tamanio=(tam-sizeof(temp));//le asigno el tamanio del disco restando el tamanio del mbr
                temp.mbr_disk_signature=1;
                //---------------
                basura tempBasura;
                int j;
                for(j=0; j<4; j++){
                    strcpy(temp.mbr_particion_[j].part_status,"0");
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
            else{
                printf("no se puede Escribir el disco %s\n",ruta);
            }
        }else{
            printf("Error el disco ya ha sido creado!ERROR!::%s\n",nombre);
        }


    }

}


void deleteDisk(char *token, int indice){
    char ruta[100]={0};
    int copi;
    char ins[10]={0};
    bool flag=false;
    for(copi=0;indice<200|| token[indice]!='\0';indice++){
        if(token[indice]=='-'||token[indice]=='+')
        {
            indice++;//aumento la variable para comenzar a copiar el comando
            while(token[indice]!=':'){//ciclo hasta que encuentro el primer : para sacar su valor
                ins[copi]= token[indice];
                copi++;
                indice++;
            }
            if(strcasecmp(ins,"path")==0||strcasecmp(ins,"path")==0){
                indice+=3;
                copi=0;
                while(token[indice]!='"'){//ciclo hasta que encuentro '"' para sacar su ruta
                    ruta[copi]= token[indice];
                    copi++;
                    indice++;
                }
                flag=true;
                break;
            }else{
                printf("Error en la ruta de disco,ERROR! %s\n",ins);
                break;
            }
        }
    }

    if(flag){
        printf("\nEsta seguro de Eliminar el Disco?:(S:N)\n");
        char res[2];
        fgets(&res,2,stdin);
        if(strcasecmp(res,"s")==0){
            int r=remove(ruta);
            if(r==0){
                printf("Eliminacion correcta de disco\n");
            }
            else{
                printf("Error al Eliminar el Disco: %s probablemente no exista el disco!\n",ruta);
            }
        }else
            printf("Se cancelo eliminacion del disco");

    }
}

void comandoFseek(char *token, int posIni){
    char commmm[400];
    strcpy(commmm,token);
    char *ins=malloc(sizeof(20));
    int tam=0;//variable para el tamanio de la particion con atoi
    char ruta[200]={0};//para el path del disco donde se va a crear la particion
    char tama[20]={0}; //para capturar el tamanio de la particion en char
    char nombre[20]={0};//para el nombre de la particion
    char unidad[1]="k";// para la dimensional del tamanio K,B,G
    char tipo[2]="P";//para el tipo de particion P,E,L
    char ajuste[5]="WF"; // para el tipo de ajuste BF,MF,WF
    char delet[6]={0}; // para el tipo de eliminacion puede ser Fast ||Full
    int add=0;// para indicar cuanto de espacio se le va aniadir a la particion
    char add1[20]={0};//para copiar lo que se va aniadir en char
    int copi;//variable puntero
    int tipoAjuste=1;//variable que me sirve para ver que tipo de ajuste va a ser, BF=1,MF=2,WF=3
    int tipoUnidad=0;//variable que me sirve para ver el tipo de unidad B=1,K=2,M=3
    int kb=1024;//para multiplicar el tamanio
    int tipoEliminacion=0;
    bool way,nome,size,supr;
    way=nome=size=supr=false;

    for(;posIni<800 && token[posIni]!='\0'&& token[posIni]!='\n';posIni++){
        copi=0;
        if(token[posIni]=='-'||token[posIni]=='+')
        {
            posIni++;//aumento la variable para comenzar a copiar el comando
            while(token[posIni]!=':'){//ciclo hasta que encuentro el primer : para sacar su valor
                ins[copi]= token[posIni];
                copi++;
                posIni++;
            }
            ins[copi]='\0';//limito el caracter para poder compararlo

            //***--------------comienza la obtencion de size*******-------------
            if((strcasecmp(ins,"size")==0)||(strcasecmp(ins,"Size")==0)){
                if(token[posIni]==':'&&token[posIni+1]==':'){
                    posIni+=2;
                    copi=0;
                    for(;posIni<800;posIni++,copi++){
                        if((token[posIni]=='-')||(token[posIni]=='+')||(token[posIni]==32)||(token[posIni]=='\0')){
                            break;
                        }
                        tama[copi]=token[posIni];
                    }
                    tama[copi]='\0';
                    tam =atoi(tama);//conversion a int del tamanio del disco
                    if(tam>0){
                        size=true;
                    }
                    else{
                        printf("Error en el tamanio de la particion : %s\n",tama);break;
                    }
                    posIni--;
                    copi=0;
                }else{
                    printf("Error en parametro size\n");
                    break;
                }
            }//---**********-termina la obtencion de size--------------------
            //------------comienza la obtencion del path****-------------------
            else if((strcasecmp(ins,"path")==0)||(strcasecmp(ins,"Path")==0)){
                bool ff=false;
                if(token[posIni]==':'&&token[posIni+1]==':'&&token[posIni+2]=='"'){
                    posIni+=3;
                    copi=0;
                    for(;posIni<800;posIni++,copi++){
                        if((token[posIni]=='-')||(token[posIni]=='+')||(token[posIni]=='"')||(token[posIni]=='\0')){
                            if(token[posIni]=='"')
                                ff=true;
                            else
                                printf("Error falta \" en el path,Error\n");
                            break;
                        }
                        ruta[copi]=token[posIni];
                    }
                    if(ff){
                        ruta[copi]='\0'; way=true;
                    }
                    else
                    {
                        printf("error en la obtencion de -path!\n");
                        break;
                    }
                }
            }
            //-**********/--termina la obtencion del path**--------------

            //----******comienza la obtencion del name***------------
            else if((strcasecmp(ins,"name")==0)||(strcasecmp(ins,"Name")==0)){
                bool ff=false;
                if(token[posIni]==':'&&token[posIni+1]==':'&&token[posIni+2]=='"'){
                    posIni+=3;
                    copi=0;
                    for(;posIni<800;posIni++,copi++){
                        if((token[posIni]=='-')||(token[posIni]=='+')||(token[posIni]=='"')||(token[posIni]=='\0')){
                            if(token[posIni]=='"')
                                ff=true;
                            else
                                printf("Error falta \" en el nombre,Error\n");
                            break;
                        }
                        nombre[copi]=token[posIni];
                    }
                    if(ff){
                        nombre[copi]='\0'; nome=true;
                    }
                    else
                    {
                        printf("error en la obtencion de -name!\n");
                        break;
                    }
                }
            }
            //---------**termina la obtencion del name***------------
            //********----------------comienza la obtencion del unit**-----------------
            else if((strcasecmp(ins,"unit")==0)||(strcasecmp(ins,"Unit")==0)){
                if(token[posIni]==':'&&token[posIni+1]==':'){
                    posIni+=2;
                    copi=0;
                    for(;posIni<800;posIni++,copi++){
                        if((token[posIni]=='-')||(token[posIni]=='+')||(token[posIni]==32)||(token[posIni]=='\0')){
                            break;
                        }
                        unidad[copi]=token[posIni];
                    }
                    unidad[copi]='\0';
                    if(unidad[0]=='m'||unidad[0]=='M'||unidad[0]=='B'||unidad[0]=='b'||unidad[0]=='K'||unidad[0]=='k'){
                        if(unidad[0]=='B'||unidad[0]=='b')
                            kb=1;
                        else if(unidad[0]=='M'||unidad[0]=='m')
                            kb=1024*1024;
                        else
                            kb=1024;
                    }
                    else{
                        printf("Error en la dimensional de tamanio: %s,ERROR!'n",unidad);
                        break;
                    }
                }else{
                    printf("Error en el parameto UNIT %s,ERROR!\n",ins);
                }
            }
            //*********-------termina la obtencion del unit**-------------------
            //----------inicia la obtencion del type**----------------------
            else if((strcasecmp(ins,"type")==0)||(strcasecmp(ins,"Type")==0)){
                if(token[posIni]==58 && token[posIni+1]==':'){
                    posIni+=2;
                    copi=0;
                    for(;posIni<800;posIni++,copi++){
                        if((token[posIni]=='-')||(token[posIni]=='+')||(token[posIni]==32)||(token[posIni]=='\0')){
                            break;
                        }
                        tipo[copi]=token[posIni];
                    }
                    tipo[copi]='\0';
                    if(tipo[0]=='e'||tipo[0]=='E'||tipo[0]=='P'||tipo[0]=='p'||tipo[0]=='L'||tipo[0]=='l'){
                        if(tipo[0]=='E'||tipo[0]=='e')
                            tipoAjuste=2;
                        else if(tipo[0]=='P'||tipo[0]=='p')
                            tipoAjuste=1;
                        else
                            tipoAjuste=3;

                    }//69=='E' tipo=='P'||tipo==69||tipo=='L'
                    else{
                        printf("Error en la dimensional de type: %s,ERROR!'n",tipo);
                        posIni=801;
                        break;
                    }
                }else{
                    printf("Error en el parameto TYPE %s,ERROR!\n",ins);
                }
            }
            //*****-*-*-*-* termina la obtencion del type*-*-*-*-*-
            //--------INICIA OBTENCION DEL FIT**------------
            else if((strcasecmp(ins,"fit")==0)||(strcasecmp(ins,"Fit")==0)){
                if(token[posIni]==':'&&token[posIni+1]==':'){
                    posIni+=2;
                    copi=0;
                    for(;posIni<800;posIni++,copi++){
                        if((token[posIni]=='-')||(token[posIni]=='+')||(token[posIni]==32)||(token[posIni]=='\0')){
                            break;
                        }
                        ajuste[copi]=token[posIni];
                    }
                    ajuste[copi]='\0';
                    if(strcasecmp(ajuste,"bf")==0||strcasecmp(ajuste,"mf")==0||strcasecmp(ajuste,"wf")==0){
                        if(strcasecmp(ajuste,"BF")==0)
                            tipoUnidad=1;
                        else if(strcasecmp(ajuste,"MF")==0)
                            tipoUnidad=2;
                        else
                            tipoUnidad=3;

                    }else{
                        printf("Error en la dimensional de AJUSTE: %s,ERROR!'n",ajuste);
                        posIni=801;
                        break;
                    }

                }else{
                    printf("Error en el parameto AJUSTE %s,ERROR!\n",ins);
                }
            }
            //---------- TERMINA OBTENCION DEL FIT...=-=-=-=-=
            //--------- INICIA la obtencion del DELETE*-*-*-*-*-*-*
            else if((strcasecmp(ins,"delete")==0)||(strcasecmp(ins,"Delete")==0)){
                if(token[posIni]==':'&&token[posIni+1]==':'){
                    posIni+=2;
                    copi=0;
                    for(;posIni<800;posIni++,copi++){
                        if((token[posIni]=='-')||(token[posIni]=='+')||(token[posIni]==32)||(token[posIni]=='\0')||(token[posIni]=='\n')||(token[posIni]=='\r')){
                            break;
                        }
                        delet[copi]=token[posIni];
                    }
                    delet[copi]='\0';
                    if(strcasecmp(delet,"Fast")==0||strcasecmp(delet,"full")==0){
                        if(strcasecmp(delet,"Fast")==0||strcasecmp(delet,"fast")==0)
                            tipoEliminacion=1;
                        else
                                 tipoEliminacion=2;
                        supr=true;
                    }

                    else
                        printf("Error en la dimensional de DELETE: %s,ERROR!'n",delet);
                }else{
                    printf("Error en el parameto delet %s,ERROR!\n",ins);
                }
            }
            //*-*-*-*-*-* TERMINA LA OBTENCION DEL DELETE/-*-*-/-/--*-
            //***-*-*-*INICIA LA OBTENCION DEL ADD/-/-/-/-/-/-/-/-/
            else if((strcasecmp(ins,"add")==0)||(strcasecmp(ins,"Add")==0)){
                if(token[posIni]==':'&&token[posIni+1]==':'){
                    posIni+=2;
                    copi=0;
                    for(;posIni<800;posIni++,copi++){
                        if((token[posIni]=='-')||(token[posIni]=='+')||(token[posIni]==32)||(token[posIni]=='\0')){
                            break;
                        }
                        add1[copi]=token[posIni];
                    }
                    add1[copi]='\0';
                    if(atoi(add1)>0)
                        printf("desesperado\n");//activar bandera de dimensional
                    else
                        printf("Error en la dimensional de ADD: %s,ERROR!'n",add1);
                    break;
                }else{
                    printf("Error en el parameto delet %s,ADD!\n",ins);
                }
            }
            //-/-/-/-/-/-termina la obtencion del add/-/-/-/-/-/-/
        }
    }//*************termina ciclo for de la linea de comandos*----------------
    if(size && way && nome &&((tam*kb)>2000000)){
        tam=tam*kb;
        crearParticion(ruta,nombre,tam,tipoAjuste,tipoUnidad,tipo,ajuste);
    }else if(size &&way &&nome&&tam*kb<2000000){
        printf(("Error el tamanio minnimo para un particion debe ser mayor o igual a 2MG\n"));
    }
    else if(supr && way && nome){
        eliminarParticion(ruta,nombre,tipoEliminacion);
    }
}


//inicia metodo de crear particion
void crearParticion(char direcion[200], char nombre[20], int tam, int tipoAjuste, int acomodamiento,char tajuste[2],char tacomodamiento[2]){
    bool ff=false;
    bool f1=false;
    bool fe=false;
    bool fl=false;
    FILE *archivo = fopen(direcion,"rb+");//compruebo si el disco existe
    if(archivo){
        MasterBR temporal;
        EBR tempEBR;
        fread(&temporal,sizeof(MasterBR),1,archivo);
        int k;
        int indice=sizeof(MasterBR);

        //condicion si es particion primaria
        if(tipoAjuste==1){
            bool noHay=true;
            for(k=0;k<4;k++){
                if (temporal.mbr_particion_[k].part_start!=0) {
                    if(strcasecmp(nombre,temporal.mbr_particion_[k].part_name)==0){
                    noHay=false;
                    printf("Error particion en Ruta:%s ya existe! Error::%s\n",direcion,nombre);
                    }

                }
            }

           if(noHay){
                for(k=0;k<4;k++){
                    indice+=temporal.mbr_particion_[k].part_size;
                    if(temporal.mbr_particion_[k].part_start==0){
                        if((k+1)<5){
                            if((indice+tam)<temporal.mbr_particion_[k+1].part_start && temporal.mbr_tamanio>(indice+tam))
                                f1=true;
                            if((temporal.mbr_particion_[k+1].part_start==0 )&&( temporal.mbr_tamanio>(indice+tam)))//aqui verifico
                                f1=true;//
                        }
                        if(f1){
                            strcpy(temporal.mbr_particion_[k].part_fit,tacomodamiento);
                            strcpy(temporal.mbr_particion_[k].part_name,nombre);
                            temporal.mbr_particion_[k].part_size=tam;
                            temporal.mbr_particion_[k].part_start=indice+1;
                            strcpy(temporal.mbr_particion_[k].part_status,"1");
                            strcpy(temporal.mbr_particion_[k].part_type,tajuste);
                            ff=true;
                            printf("Se creo correctamente la particion Primaria:%s\n",nombre);
                        }
                        else
                            printf("Error longitud mayor a tamanio de disco || sobre pasa los limites de otra particion ya creada! \n");
                        break;
                    }
                }
             }//termina condicion para particion primaria
            }

        //condicion si es para particion extendida
        else if(tipoAjuste==2){
            int p=0;
            for(k=0;k<4;k++){
                if(strcasecmp(temporal.mbr_particion_[k].part_name,"")!=0){
                    if(strcasecmp(temporal.mbr_particion_[k].part_type,"E")==0){
                        p=1;break;
                    }
                }
            }
            if(p==0) {
                for(k=0;k<4;k++){
                    indice+=temporal.mbr_particion_[k].part_size;
                    if(temporal.mbr_particion_[k].part_start==0){
                        if((k+1)<5){
                            if((indice+tam)<temporal.mbr_particion_[k+1].part_start && temporal.mbr_tamanio>(indice+tam))
                                f1=true;
                            if((temporal.mbr_particion_[k+1].part_start==0 )&&( temporal.mbr_tamanio>(indice+tam)))//aqui verifico
                                f1=true;//
                        }
                        if(f1){
                            strcpy(temporal.mbr_particion_[k].part_fit,tacomodamiento);

                            strcpy(temporal.mbr_particion_[k].part_name,nombre);
                            temporal.mbr_particion_[k].part_size=tam;
                            temporal.mbr_particion_[k].part_start=indice+1;
                            strcpy(temporal.mbr_particion_[k].part_status,"1");
                            strcpy(temporal.mbr_particion_[k].part_type,tajuste);
                            //aqui escribo el primer EBR

                            strcpy(tempEBR.part_fit,"");
                            strcpy(tempEBR.part_name,"");
                            tempEBR.part_next=-1;
                            tempEBR.part_size=0;
                            tempEBR.part_start=indice+1;
                            strcpy(tempEBR.part_status,"0");
                            //termina escritura de EBR
                            fe=true;
                            printf("Se creo correctamente la particion Extendida:%s\n",nombre);
                        }
                        else
                            printf("Error longitud mayor a tamanio de disco || sobre pasa los limites de otra particion ya creada! \n");
                        break;
                    }
                }
            }else{
                printf("Ya existe una particion extendida:%s  ! ERROR!\n",nombre);
            }
        }
        //condicion si es para particion logica
        else{//dejare en estan by
            int p=0;
            for(k=0;k<4;k++){
                if(strcasecmp(temporal.mbr_particion_[k].part_name,"")!=0){
                    if(strcasecmp(temporal.mbr_particion_[k].part_type,"E")==0){
                        bool aja=true;//variable para recorrer mis EBR
                        bool nom=true;//variable para ver si no existe ya el nombre
                        //*-*-*
                        EBR tmp;
                        fseek(archivo,temporal.mbr_particion_[k].part_start,SEEK_SET);
                        fread(&tmp,sizeof(EBR),1,archivo);
                        //*-*-*
                        //aqui debo hacer un algoritmo para verificar que no exista el nombre de la particion

                        while(nom){
                            if(strcasecmp(tmp.part_name,nombre)==0){
                                aja=false;
                                nom=false;
                                printf("error ya existe ese nombre de particion!:%s\n",nombre);
                            }else{
                                if(tmp.part_next!=-1){
                                    fseek(archivo,tmp.part_next,SEEK_SET);
                                    fread(&tmp,sizeof(EBR),1,archivo);
                                }else{
                                    nom=false;
                                }
                            }
                        }
                        //---termina el metdo para verificar si existe el nombre de la particion
                        //maniana termino las particiones logicas

                        fseek(archivo,temporal.mbr_particion_[k].part_start,SEEK_SET);
                        fread(&tmp,sizeof(EBR),1,archivo);
                        int counter=temporal.mbr_particion_[k].part_start+sizeof(EBR);//OJO AQUI

                        tam=tam-(2*sizeof(EBR));
                        while(aja){
                            if(tmp.part_size==0&&tmp.part_next==-1){
                                //---------
                                if((counter+tam+1)<(temporal.mbr_particion_[k].part_start+temporal.mbr_particion_[k].part_size))//verificar esto
                                    p=true;
                                else{
                                    k=4;
                                    break;
                                }

                                if(p){
                                    strcpy(tmp.part_fit,tajuste);//no es tipo sino ajuste
                                    strcpy(tmp.part_name,nombre);
                                    tmp.part_size=tam;
                                    tmp.part_next=(tmp.part_start+sizeof(EBR)+tmp.part_size+1);//aqui voy a crear otro EBR y se lo pongo verificar esto
                                    strcpy(tmp.part_status,"1");
                                    aja=false;
                                    fseek(archivo,tmp.part_start,SEEK_SET);
                                    fwrite(&tmp,sizeof(EBR),1,archivo);//aqui actualizo la creacion de de la particion logica
                                    //ahora creo una nueva EBR vacia

                                    EBR temp2;
                                    temp2.part_start=tmp.part_start+sizeof(EBR)+tmp.part_size+1;

                                    strcpy(temp2.part_fit,"");
                                    strcpy(temp2.part_name,"");
                                    temp2.part_size=0;
                                    temp2.part_next=-1;
                                    strcpy(temp2.part_status,"1");

                                    fseek(archivo,temp2.part_start,SEEK_SET);
                                    fwrite(&temp2,sizeof(EBR),1,archivo);//actualizo la siguiente particion logica
                                    fl=true;
                                    printf("Se creo correctamente la particion logica:%s\n",nombre);
                                    break;
                                }

                            }else if(tmp.part_size==0&&tmp.part_next!=-1){//este lo ocupo cuando elimine una particion logica y se quedo el ebr porque hay mas
                                EBR temp2;//*-*-*-*-*-*-*-*-*
                                fseek(archivo,tmp.part_next,SEEK_SET);//aqui leo la siguiente EBR
                                fread(&temp2,sizeof(EBR),1,archivo);
                                if((tmp.part_start+sizeof(EBR)+tam)<temp2.part_start)
                                {
                                    strcpy(tmp.part_fit,tajuste);
                                    strcpy(tmp.part_name,nombre);
                                    tmp.part_size=tam;
                                    tmp.part_next=temp2.part_start;//aqui voy a crear otro EBR y se lo pongo verificar esto
                                    strcpy(tmp.part_status,"1");
                                    aja=false;
                                    fseek(archivo,tmp.part_start,SEEK_SET);
                                    fwrite(&tmp,sizeof(EBR),1,archivo);//aqui actualizo la creacion de de la particion logica
                                    //verificar si debo grabar la particion siguiente
                                    //-*-*-*-*-*-*-*
                                    printf("Se creo correctamente la particion logica:%s\n",nombre);
                                    fl=true;
                                    break;
                                }

                            }else{
                                fseek(archivo,tmp.part_next,SEEK_SET);//aqui leo la siguiente EBR
                                fread(&tmp,sizeof(EBR),1,archivo);
                                counter=(tmp.part_start+sizeof(EBR));//LEER ESTE CONTADOR HAHA PARA VER PARA QUE ME SIRVE
                                //if()
                            }
                        }
                    }
                }
                if(fl)
                    break;
            }

        }//termina creacion de particion logica
        if(fe){
            fseek(archivo,0,SEEK_SET);
            fwrite(&temporal,sizeof(MasterBR),1,archivo);
            fseek(archivo,indice+1,SEEK_SET);//debo verificar si esto  me funciona
            fwrite(&tempEBR,sizeof(EBR),1,archivo);

        }else if(ff){
            fseek(archivo,0,SEEK_SET);
            fwrite(&temporal,sizeof(MasterBR),1,archivo);
        }else if(fl){
            printf("Se creo la particion logica %s\n",nombre);
        }else
            printf("No se pudo crear la particion::%s en ruta::%s\n",nombre,direcion);
        fclose(archivo);
    }else{
        printf("Error al abrir disco, no existe,ERROR!\n");
        return;
    }
}

// finaliza metodo para crear particion


//inicializa metodo para eliminar particion

void eliminarParticion(char ruta[], char nombre[], int tipo){
    bool ff=false;
    bool f1=false;
    bool fe=false;
    bool fl=false;
    FILE *archivo = fopen(ruta,"rb+");//compruebo si el disco existe
    if(archivo){
        MasterBR temporal;
        EBR tempEBR;
        fread(&temporal,sizeof(MasterBR),1,archivo);
        int k;
        int indice=sizeof(MasterBR);
        for(k=0;k<4;k++){
            if(strcasecmp(temporal.mbr_particion_[k].part_name,nombre)==0){

                if(tipo==1){
                    strcpy(temporal.mbr_particion_[k].part_status,"0");
                    strcpy(temporal.mbr_particion_[k].part_type,"");
                    strcpy(temporal.mbr_particion_[k].part_fit,"");
                    temporal.mbr_particion_[k].part_start=0;
                    temporal.mbr_particion_[k].part_size=0;
                    strcpy(temporal.mbr_particion_[k].part_name,"");
                }else{
                    fseek(archivo,temporal.mbr_particion_[k].part_start,SEEK_SET);
                    int r;
                    int total=temporal.mbr_particion_[k].part_size+temporal.mbr_particion_[k].part_start;
                    for(r=temporal.mbr_particion_[k].part_start;r<total;r++){
                        fseek(archivo,r,SEEK_SET);
                        fputc(0,archivo);
                    }
                    strcpy(temporal.mbr_particion_[k].part_status,"0");
                    strcpy(temporal.mbr_particion_[k].part_type,"");
                    strcpy(temporal.mbr_particion_[k].part_fit,"");
                    temporal.mbr_particion_[k].part_start=0;
                    temporal.mbr_particion_[k].part_size=0;
                    strcpy(temporal.mbr_particion_[k].part_name,"");
                }

            }
        }
        fclose(archivo);
        //-*-*-*-*-
        for(k=0;k<4;k++){
            if(strcasecmp(temporal.mbr_particion_[k].part_name,"")!=0){
                if(strcasecmp(temporal.mbr_particion_[k].part_type,"E")==0){
                    bool nom=true;//variable para ver si no existe ya el nombre
                    //*-*-*
                    FILE *archivo = fopen(ruta,"rb+");
                    EBR tmp;
                    EBR tmp2;
                    usleep(200);
                    fseek(archivo,temporal.mbr_particion_[k].part_start,SEEK_SET);
                    fread(&tmp,sizeof(EBR),1,archivo);
                    //*-*-*
                    //aqui debo hacer un algoritmo para verificar que exista el nombre de la particion
                    bool modi = false;
                    while(nom){
                        if(strcasecmp(tmp.part_name,nombre)==0){
                            nom=false;
                            strcpy(tmp.part_fit,"");//este caso solo se puede dar una vez
                            strcpy(tmp.part_name,"");
                            //tmp.part_next=-1;
                            tmp.part_size=0;
                            strcpy(tmp.part_status,"0");
                            fseek(archivo,tmp.part_next,SEEK_SET);//necesito acordarme para que hice esto
                            fread(&tmp2,sizeof(EBR),1,archivo);
                            if(tmp2.part_size==0&&tmp2.part_next>0)
                                tmp.part_next=tmp2.part_next;
                            modi=true;
                            //aqui algoritmo final para particion
                            //pregunto si el tamanio de la siguiente es cero
                            //temporal primero es igual a temporal2_next
                        }else{
                            if(tmp.part_next!=-1){
                                fseek(archivo,tmp.part_next,SEEK_SET);
                                fread(&tmp,sizeof(EBR),1,archivo);
                            }else{
                                nom=false;
                            }
                        }
                    }
                    //---termina el metdo para verificar si existe el nombre de la particion
                    fseek(archivo,temporal.mbr_particion_[k].part_start,SEEK_SET);//necesito acordarme para que hice esto
                    fread(&tmp,sizeof(EBR),1,archivo);
                    nom=true;
                    EBR tmp3;
                    while(modi){
                        if(tmp.part_next>0){
                            fseek(archivo,tmp.part_next,SEEK_SET);//necesito acordarme para que hice esto
                            fread(&tmp2,sizeof(EBR),1,archivo);

                            if(tmp2.part_next>0){
                                //tmp.part_next=tmp2.part_next;
                                fseek(archivo,tmp2.part_next,SEEK_SET);//me paso al siguiente a analizar
                                fread(&tmp3,sizeof(EBR),1,archivo);
                                if(tmp2.part_size==0&&tmp3.part_size==0&&tmp3.part_next>0){
                                    tmp2.part_next=tmp3.part_next;
                                }
                            }else{
                                fseek(archivo,tmp.part_next,SEEK_SET);//necesito acordarme para que hice esto
                                fread(&tmp2,sizeof(EBR),1,archivo);
                            }
                        }else{
                            modi=false;
                        }


                    }

                }
            }
            if(fl)
                break;
        }
        fclose(archivo);

        //-*-*-*-*-

    }else{
        printf("Error al abrir disco, no existe o ruta mal establecida,ERROR!\n");
    }
}

//*-*-*-*termina metodo para eliminar particion

//inicializa metodo para montar particion
void montarParticion(char token[200], int posIni){
    int copi;
    char ruta[50];
    char nombre[20];
    char ins[10];
    bool way=false;
    bool nome=false;
    bool mont=false;
    char mounts[50];
    for(copi=0;copi<50&&token[copi]!='\n';copi++)
        mounts[copi]=token[copi];
    if(strcasecmp(mounts,"mount")==0){
        FILE *archivo;
            archivo=fopen("/home/jherson/Escritorio/fa/manager.dsk","r+");
            MasterDisk temp;
            fseek(archivo,0,SEEK_SET);
            fread(&temp,sizeof(MasterDisk),1,archivo);
            usleep(200);
            fclose(archivo);
            for(copi=0;copi<26;copi++){
                if(temp.DISCOSS[copi].llenas>0){
                    int r=0;
                    for(;r<26;r++){
                        if(strcasecmp(temp.DISCOSS[copi].particion[r].nombre,"")!=0)
                            printf("#id::vd%c%d -path::%s -name::%s\n",abecedario[copi]
                                    ,r+1,temp.DISCOSS[copi].nombre,temp.DISCOSS[copi].particion[r].nombre);
                    }
                    mont=true;
                }
            }
    }else{
        for(;posIni<800|| token[posIni]!='\0';posIni++){
            copi=0;
            if(token[posIni]=='-')
            {
                posIni++;//aumento la variable para comenzar a copiar el comando
                while(token[posIni]!=':'){//ciclo hasta que encuentro el primer : para sacar su valor
                    ins[copi]= token[posIni];
                    copi++;
                    posIni++;
                }
                ins[copi]='\0';//limito el caracter para poder compararlo

                //------------comienza la obtencion del path****-------------------
                if((strcasecmp(ins,"path")==0)||(strcasecmp(ins,"Path")==0)){
                    bool ff=false;
                    if(token[posIni]==':'&&token[posIni+1]==':'&&token[posIni+2]=='"'){
                        char *abecedario[27]= {"\0","a","b","c","d","e","f","g","h","i","j","k","l","m",
                                               "n","o","p","q","r","s","t","u","v","w","x","y","z"};        posIni+=3;
                        copi=0;
                        for(;posIni<800;posIni++,copi++){
                            if((token[posIni]=='-')||(token[posIni]=='+')||(token[posIni]=='"')||(token[posIni]=='\0')){
                                if(token[posIni]=='"')
                                    ff=true;
                                else
                                    printf("Error falta \" en el path,Error\n");
                                break;
                            }
                            ruta[copi]=token[posIni];
                        }
                        if(ff){
                            ruta[copi]='\0'; way=true;
                        }
                        else
                        {
                            printf("error en la obtencion de -path!\n");
                            break;
                        }
                    }
                }
                //-**********/--termina la obtencion del path**--------------

                //----******comienza la obtencion del name***------------
                else if((strcasecmp(ins,"name")==0)||(strcasecmp(ins,"Name")==0)){
                    bool ff=false;
                    if(token[posIni]==':'&&token[posIni+1]==':'&&token[posIni+2]=='"'){
                        posIni+=3;
                        copi=0;
                        for(;posIni<800;posIni++,copi++){
                            if((token[posIni]=='-')||(token[posIni]=='+')||(token[posIni]=='"')||(token[posIni]=='\0')){
                                if(token[posIni]=='"')
                                    ff=true;
                                else
                                    printf("Error falta \" en el nombre,Error\n");
                                break;
                            }
                            nombre[copi]=token[posIni];
                        }
                        if(ff){
                            nombre[copi]='\0'; nome=true;

                        }
                        else
                        {
                            printf("error en la obtencion de -name!\n");
                            break;
                        }
                    }
                }
                //---------**termina la obtencion del name***------------
            }//termina if (-)
        }//termina for de comandos
    }
    if(way&&nome)
        buildPartition(ruta,nombre);
    else if(!mont)
        printf("No mas particiones por mostrar!\n");
    else
      printf("Error en el parametro MOUNT! %s\n",token);
}
void buildPartition(char ruta[],char nombre[]){
    FILE *archivo;
    FILE *disca;
    disca=fopen("/home/jherson/Escritorio/fa/manager.dsk","rb+");
    MasterDisk DISCO;
    fseek(disca,0,SEEK_SET);
    fread(&DISCO,sizeof(MasterDisk),1,disca);
    usleep(300);
    fclose(disca);//cierro el archivo
    int k;
    archivo=fopen(ruta,"r+");
    bool recursiva=false;
    if(archivo!=NULL){//el archivo si existe
        MasterBR tmp;
        fseek(archivo,0,SEEK_SET);
        fread(&tmp,sizeof(MasterBR),1,archivo);//cargo el master boot recorder para ver si existe la particion
        //-*-*-*-*-aqui voy a buscar que exista la partiion
        int q;
        int tipo=0;
        bool ex=true;
        for(q=0;q<4;q++){
            if(strcasecmp(tmp.mbr_particion_[q].part_name,nombre)==0){
                if(strcasecmp(tmp.mbr_particion_[q].part_type,"E")!=0){//verifico que no sea una particion extendida
                    tipo=1;
                    ex=false;
                    break;
                }
            }
        }
        if(ex){
            for(k=0;k<4;k++){
                if(strcasecmp(tmp.mbr_particion_[k].part_type,"E")==0){
                    EBR tmpE;
                    fseek(archivo,tmp.mbr_particion_[k].part_start,SEEK_SET);
                    fread(&tmpE,sizeof(EBR),1,archivo);
                    bool aja=true;
                    while(aja){
                        if(strcasecmp(tmpE.part_name,nombre)==0){
                            aja=false;
                            ex=false;
                        }else{
                            if(tmpE.part_next!=-1){
                                fseek(archivo,tmpE.part_next,SEEK_SET);
                                fread(&tmpE,sizeof(EBR),1,archivo);
                            }
                            else
                                aja=false;
                        }
                    }
                }
            }
        }
        //----termino de verificar que exista la particion
        bool flg=false;
        bool hay=true;
        if(!ex ){//si existe la particion es primaria || logica
            for(k=1;k<27;k++){
                if(DISCO.DISCOSS[k].id!=0){//pregunta si el id es distinto de cero pero al principio es cero???
                    if(strcasecmp(DISCO.DISCOSS[k].nombre,ruta)==0){
                        flg=true;
                        if(DISCO.DISCOSS[k].llenas==0){//esto es para la primera particion del Disco sda1
                            //   DISCO.DISCOSS[k].particiones[DISCO.DISCOSS[k].llenas]=malloc(sizeof(nombre+1));
                            strcpy(DISCO.DISCOSS[k].particion[DISCO.DISCOSS[k].llenas].nombre,nombre);
                            DISCO.DISCOSS[k].llenas++;
                            break;
                        }else{//ya tiene particiones el disco
                            int r;
                            hay=true;
                            for(r=0;r<DISCO.DISCOSS[k].llenas;r++){//aqui voy a verificar que no haya montado esa particion
                                if(strcasecmp(DISCO.DISCOSS[k].particion[r].nombre,nombre)==0){
                                    hay=false;
                                    printf("Ya se monto esa particion! ERROR!:: %s\n",nombre);
                                    break;
                                }
                            }
                            if(hay){//si no he montado esa particion la voy a montar
                                strcpy(DISCO.DISCOSS[k].particion[DISCO.DISCOSS[k].llenas].nombre,nombre);
                                DISCO.DISCOSS[k].llenas++;
                                break;
                            }
                        }
                    }
                    if(!hay)
                        break;
                }
            }
        }

        if(!flg && !ex){//verificar si ocupo && !ex

            strcpy(DISCO.DISCOSS[DISCO.indicador].nombre,ruta);//copio la ruta del archivo
            DISCO.DISCOSS[DISCO.indicador].llenas=0;//le pongo llenas cero porque no tengo al momento ninguna particion
            DISCO.DISCOSS[DISCO.indicador].id=(DISCO.indicador);//creacion de un nuevo disco el indice me va a servir despues
            DISCO.indicador++;
            disca=fopen("/home/jherson/Escritorio/fa/manager.dsk","rb+");
            fseek(disca,0,SEEK_SET);
            fwrite(&DISCO,sizeof(MasterDisk),1,disca);
            //fclose(archivo);
            usleep(2000);
            fclose(disca);
            recursiva=true;
            buildPartition(ruta,nombre);//recurro otra vez al metodo pero ya con
        }
        if(!recursiva && !ex && hay ){//aqui no tiene que estrar si hay error en hay
            disca=fopen("/home/jherson/Escritorio/fa/manager.dsk","rb+");
            fseek(disca,0,SEEK_SET);
            fwrite(&DISCO,sizeof(MasterDisk),1,disca);
            fclose(disca);
        }
        fclose(archivo);//ciero el archivo para evitar errores
        if(ex)
            printf("Error al montar la particion: %s no ha sido creada!\n",nombre);
    }
    else
        printf("Error en la ruta del archivo o Disco no existe! %s",ruta);
}

//finaliza metodo para montar particion

void ejecutarComandos(char comando[]){
    //*-*-*-*-*-*-*-*-*
    int o;
    char primerComando[10];
    for(o=0;o<800&&comando[o]!='\0'&&comando[o]!='\n'&&comando[o]!='\r';o++){
        if(comando[o]==45||comando[o]==32||comando[o]==43|| comando[o]=='\n'){
            primerComando[o]='\0';

            if(strcasecmp(primerComando,"Mkdisk")==0||strcasecmp(primerComando,"mkdisk")==0){
                crearDisco(comando,o);
                break;
            }else if(strcasecmp(primerComando,"rmdisk")==0||strcasecmp(primerComando,"rmDisk")==0){
                    deleteDisk(comando,o);
                    break;
           }
            else if(strcasecmp(primerComando,"fdisk")==0||strcasecmp(primerComando,"fDisk")==0){
                       comandoFseek(comando,o);;
                        break;
            }
            else if(strcasecmp(primerComando,"mount")==0||strcasecmp(primerComando,"mount")==0){
                montarParticion(comando,o);;
                   break;
            } else if(strcasecmp(primerComando,"unmount")==0||strcasecmp(primerComando,"unMount")==0){
                desmontar(comando,o);;

                break;
            }
            else if(strcasecmp(primerComando,"exec")==0||strcasecmp(primerComando,"mount")==0){
                comandoExect(comando,o);
                   break;
            }  else if(strcasecmp(primerComando,"rep")==0||strcasecmp(primerComando,"Rep")==0){
                Reportes(comando,o);
                   break;
            }
            else{
                printf("Error comando no reconocido!\n");
                break;
           }
        }else{
          primerComando[o]=comando[o];
        }
    }

}

void Reportes(char token[], int posIni){
    int copi;
    char cooo[400]={0};
    strcpy(cooo,token);
    char ruta[200]={0};
    char nombre[20];
    char iden[20];
    int id;
    bool nom=false;
    bool way=false;
    bool ser=false;

    char ins[10];
    FILE *disca;
    disca=fopen("/home/jherson/Escritorio/fa/manager.dsk","rb+");
    MasterDisk temporal;
    fread(&temporal,sizeof(MasterDisk),1,disca);
    usleep(200);
    fclose(disca);
    for(;posIni<800 && token[posIni]!='\0'&&token[posIni]!='\n';posIni++){
        copi=0;
        if(token[posIni]=='-')
        {
            posIni++;//aumento la variable para comenzar a copiar el comando
            while(token[posIni]!=':'){//ciclo hasta que encuentro el primer : para sacar su valor
                ins[copi]= token[posIni];
                copi++;
                posIni++;
            }
            ins[copi]='\0';//limito el caracter para poder compararlo
            //------------comienza la obtencion del path****-------------------
            if((strcasecmp(ins,"path")==0)||(strcasecmp(ins,"Path")==0)){
                bool ff=false;
                if(token[posIni]==':'&&token[posIni+1]==':'&&token[posIni+2]=='"'){
                    posIni+=3;
                    copi=0;
                    for(;posIni<800;posIni++,copi++){
                        if((token[posIni]=='-')||(token[posIni]=='"')||(token[posIni]=='\0')){
                            if(token[posIni]=='"')
                                ff=true;
                            else
                                printf("Error falta \" en el path,Error\n");
                            break;
                        }
                        ruta[copi]=token[posIni];
                    }
                    if(ff){
                        ruta[copi]='\0'; way=true;
                    }
                    else
                    {
                        printf("error en la obtencion de -path!\n");
                        break;
                    }
                }
            }
            //-**********/--termina la obtencion del path**--------------

            //----******comienza la obtencion del name***------------
            else if((strcasecmp(ins,"name")==0)||(strcasecmp(ins,"Name")==0)){
                if(token[posIni]==':'&&token[posIni+1]==':'){
                    posIni+=3;
                    copi=0;
                    for(;posIni<800 && token[posIni]!='\0'&&token[posIni]!='\n';posIni++,copi++){
                        if((token[posIni]=='-')||(token[posIni]=='\0')||(token[posIni]=='\"')||(token[posIni]=='\n')||(token[posIni]==32)){
                          break;
                        }
                        nombre[copi]=token[posIni];
                    }
                 nombre[copi]='\0';
                 nom=true;
                }
            }
            //---------**termina la obtencion del id***------------
            else if((strcasecmp(ins,"id")==0)||(strcasecmp(ins,"Id")==0)){
                if(token[posIni]==':'&&token[posIni+1]==':'){
                    posIni+=2;
                    copi=0;
                    for(;posIni<800 && token[posIni]!='\0'&&token[posIni]!='\n';posIni++,copi++){
                        if((token[posIni]=='-')||(token[posIni]=='\0')||(token[posIni]=='n')||(token[posIni]==32)){
                            break;
                        }
                        iden[copi]=token[posIni];
                    }
                    iden[copi]='\0'; nom=true;
                    if(iden[0]=='v' && iden[1]=='d'){
                        int v;
                        for(v=0;v<27;v++){
                            if(iden[2]==abecedario[v])
                                break;
                        }

                        char tam[2];
                        int h;
                        for(h=3;h<5&&iden[h]!='\0';h++)
                            tam[h-3]=iden[h];
                        tam[h-3]='\0';
                        int noParticion=atoi(tam);
                        if(noParticion>0){
                            if(temporal.DISCOSS[v].llenas>0){
                                id=v;
                                ser=true;
                           }else{
                                printf("Error no se encontro vd %c particion no se encuentra!\n",abecedario[v]);
                                break;
                            }
                        }else{
                            printf("El valor de vd%s es no es valido!ERROR!\n",ins[3]);
                        }
                    }
                    //-*-*-*-*-*-*-**-*-*-*-*-**-*-*-*
                }else
                    printf("Error en el comando rep!!:: %s\n",token);

            }
         }//----termina comparacion de id
     }
    if(nom &&ser&&way){

        char directory[200]={0};
        strcpy(directory,"mkdir -p ");
        char carpeta[250];
        int e; int ultimo=0;
        for(e=0;e<200&&ruta[e]!='\0';e++){
            if(ruta[e]=='/')
                ultimo=e;
        }
        carpeta[0]='\'';
        for(e=1;e<=ultimo+1;e++)
            carpeta[e]=ruta[e-1];
        carpeta[e]='\'';
        strcat(directory,carpeta);
        const int dir_err = system(directory);
        bool pat=true;
        if (0 != dir_err)
        {
            printf("Error al crear Directorio::%s\n",ruta);
            pat=false;

        }

        FILE *rep=fopen(temporal.DISCOSS[id].nombre,"r+");

        if(rep&&pat){
            fseek(rep,0,SEEK_SET);
            MasterBR tmp;
            EBR tmpE;
            fread(&tmp,sizeof(MasterBR),1,rep);
          // fclose(rep);

        FILE *grafo =fopen("/home/jherson/Escritorio/disco.dot","w+");

        if(strcasecmp(nombre,"mbr")==0){
            FILE *grafo =fopen("/home/jherson/Escritorio/disco.dot","w+");

            fprintf(grafo,"digraph structura{\n");
            fprintf(grafo,"node [shape=record,height=.1];\n");
            fprintf(grafo,"struct3 [shape=record,label=\"{{Nombre|Valor}");
            fprintf(grafo,"|{mbr_tamanio|%d}",tmp.mbr_tamanio);
            fprintf(grafo,"|{mbr_fecha_creacion|%s}",tmp.mbr_fecha_creacion);
            fprintf(grafo,"|{mbr_disk_signature|%d}",tmp.mbr_disk_signature);

            int t;
            for(t=0;t<4;t++){
              if(strcasecmp(tmp.mbr_particion_[t].part_type,"E")==0) {
                fseek(rep,tmp.mbr_particion_[t].part_start,SEEK_SET);
                  fread(&tmpE,sizeof(EBR),1,rep);
                  EBR temp2;
                  bool aja=true;
                  int d=1;
                  while(aja){
                      if(tmpE.part_next>0){
                          if(tmpE.part_size>20){
                            fprintf(grafo,"|{EBR_%d}",d);
                            fprintf(grafo,"|{part_status_%d|%s}",d,tmpE.part_status);
                            //fprintf(grafo,"|{part_fit_%s|%d}",d,tmpE.part_fit);
                            fprintf(grafo,"|{part_start_%d|%d}",d,tmpE.part_start);
                            fprintf(grafo,"|{part_size_%d|%d}",d,tmpE.part_size);
                            fprintf(grafo,"|{part_next_%d|%d}",d,tmpE.part_next);
                          //  fprintf(grafo,"|{part_name_%s|%d}",d,tmpE.part_name);
                            d++;
                            fseek(rep,tmpE.part_next,SEEK_SET);
                            fread(&tmpE,sizeof(EBR),1,rep);
                              }else{
                              fseek(rep,tmpE.part_next,SEEK_SET);
                              fread(&tmpE,sizeof(EBR),1,rep);
                         }
                      }else{
                          //iprimir lo que contiene actual
                          aja=false;
                      }
                  }
              }else{
                  if(tmp.mbr_particion_[t].part_size>0){
                      fprintf(grafo,"|{part_status_%d|%s}",(t+1),tmp.mbr_particion_[t].part_status);
                      fprintf(grafo,"|{part_type_%d|%s}",(t+1),tmp.mbr_particion_[t].part_type);
                      fprintf(grafo,"|{part_fit_%d|%s}",(t+1),tmp.mbr_particion_[t].part_fit);
                      fprintf(grafo,"|{part_start_%d|%d}",(t+1),tmp.mbr_particion_[t].part_start);
                      fprintf(grafo,"|{part_size_%d|%d}",(t+1),tmp.mbr_particion_[t].part_size);
                      fprintf(grafo,"|{part_name_%d|%s}",(t+1),tmp.mbr_particion_[t].part_name);
                  }
              }
            }
            fprintf(grafo,"}");

            fprintf(grafo,"\"];");
            fprintf(grafo,"}");
            fclose(grafo);
            fclose(rep);
            char coma[200];
            strcpy(coma,"dot -Tpng /home/jherson/Escritorio/disco.dot -o");
            strcat(coma,ruta);
            //system("dot -Tpng /home/jherson/Escritorio/disco.dot -o /home/jherson/Escritorio/discos.png");
            system(coma);
            strcat(ruta," &");
            char abrir[200];
            strcpy(abrir,"gnome-open ");
            strcat(abrir,ruta);
            system(abrir);

        }else if(strcasecmp(nombre,"disk")==0){
            fprintf(grafo,"digraph structura{\n");
            fprintf(grafo,"node [shape=record,height=.1];\n");
            fprintf(grafo,"struct3 [shape=record,label=\"MBR");
                 int t;
                for(t=0;t<4;t++){
                 if(tmp.mbr_particion_[t].part_size!=0){
                    if(strcasecmp(tmp.mbr_particion_[t].part_type,"E")==0){
                        fprintf(grafo,"|{Extendida|{");
                        fseek(rep,tmp.mbr_particion_[t].part_start,SEEK_SET);
                        fread(&tmpE,sizeof(EBR),1,rep);
                        EBR temp2;
                        bool aja=true;
                        while(aja){
                            if(tmpE.part_next>0){
                                if(tmpE.part_size>20){
                                    fseek(rep,tmpE.part_next,SEEK_SET);
                                    fread(&temp2,sizeof(EBR),1,rep);
                                   if(temp2.part_next>0 ){
                                      fprintf(grafo,"EBR|Logica|");
                                   }else{
                                     fprintf(grafo,"EBR|Logica");
                                   }
                                      fseek(rep,tmpE.part_next,SEEK_SET);
                                     fread(&tmpE,sizeof(EBR),1,rep);
                                }else{
                                    fprintf(grafo,"|Libre");
                                    fseek(rep,tmpE.part_next,SEEK_SET);
                                    fread(&tmpE,sizeof(EBR),1,rep);
                               }
//                                FILE *disco;
//                                disco=fopen("/home/jherson/Escritorio/fa/manejador.dsk","ab");
//                                MasterDisk temp;
//                                fseek(disco,0,SEEK_SET);
//                                fread(&temp,sizeof(MasterDisk),1,disco);
//                                temp.indicador=1;
//                                fseek(disco,0,SEEK_SET);
//                                fwrite(&temp,sizeof(MasterDisk),1,disco);
//                                fclose(disco);
                            }else{
                                //iprimir lo que contiene actual
                                aja=false;
                            }
                        }
                        fprintf(grafo,"}}");

                    }else{
                        fprintf(grafo,"|Primaria");
                    }
                }else{
                    fprintf(grafo,"|Libre");
                }
             }
                //fprintf(grafo,"}");
                fprintf(grafo,"\"];");
                fprintf(grafo,"}");
                fclose(grafo);
                fclose(rep);
                char coma[200];
                strcpy(coma,"dot -Tpng /home/jherson/Escritorio/disco.dot -o");
                strcat(coma,ruta);
                //system("dot -Tpng /home/jherson/Escritorio/disco.dot -o /home/jherson/Escritorio/discos.png");
                system(coma);
                strcat(ruta," &");
                char abrir[200];
                strcpy(abrir,"gnome-open ");
                strcat(abrir,ruta);
                system(abrir);
            }

        }else{
            printf("Error tipo de reporte incorrecto!::%s\n",nombre);
        }
    }else{
        printf("Errores en la entrada de Reportes!::%s \n",token);
    }
}

