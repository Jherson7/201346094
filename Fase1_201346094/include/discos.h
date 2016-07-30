#ifndef DISCOS_H
#define DISCOS_H
#include <time.h>

typedef struct Particiones{
    char part_status[1];
    char *part_type;
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
}MasterBR;

typedef struct Vacio{
    char espacio[1024];
}basura;
#endif // DISCOS_H
