#ifndef DISCOS_H
#define DISCOS_H
#include <time.h>



typedef struct Particiones{
    char part_status[1];
    char part_type[2];
    char part_fit[2];
    int part_start;
    int part_size;
    char part_name[16];
    int part_id;
}Particion;

typedef struct Disk{
    int estado;
    char llave[3];
    char *direccion;
}disco;

typedef struct MasterBootRecord{
    int mbr_tamanio;
    char mbr_fecha_creacion[19];
    int mbr_disk_signature;
    Particion mbr_particion_[4];
    int indiceParticion;
}MasterBR;

typedef struct ExtendedBootRecord{
    char part_status[1]; //Indica si la partición está activa o no
    char part_fit[2]; //Tipo de ajuste de la partición. Tendrá los valores BF (Best), FF   (First) o WF (worst)
    int part_start; //Indica en que byte del disco inicia la partición
    int part_size; //Contiene el tamaño total de la partición en bytes.
    int part_next; //Byte en el que está el próximo EBR. -1 si no hay siguiente
    char part_name[16] ; //Nombre de la partición
}EBR;


typedef struct disk{
    char nombre[20];
    int id;
    char *particiones[27];
    int llenas;
}MDD;

typedef struct Vacio{
    char espacio[1024];
}basura;
#endif // DISCOS_H
