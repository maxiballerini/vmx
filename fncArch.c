#include "componentes.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "const.h"
void inicializaTablaSegmentos(maquinaVirtual *MV,uint16_t codeS,uint16_t dataS,uint16_t extraS,uint16_t stackS,uint16_t constS,int tamanoMemoria){
    int aux=0,pos=0;
    MV->segmento[KS] = -1;
    MV->segmento[CS] = -1;
    MV->segmento[DS] = -1;
    MV->segmento[ES] = -1;
    MV->segmento[SS] = -1;
    if(KS>0){
        MV->registro[KS]=pos<<16;
        MV->segmento[pos] = aux < 16;
        MV->segmento[pos] += dataS;
        aux+=constS;
        pos++;
    }
    if(CS>0){
        MV->registro[CS]=pos<<16;
        MV->segmento[pos] =  aux  < 16;
        MV->segmento[pos] += codeS;
        aux+=codeS;
        pos++;
    }
    if(DS>0){
        MV->registro[DS]=pos<<16;
        MV->segmento[pos] =  aux  < 16;
        MV->segmento[pos] += dataS;
        aux+=dataS;
        pos++;
    }
    if(ES>0){
        MV->registro[ES]=pos<<16;
        MV->segmento[pos] =  aux  < 16;
        MV->segmento[pos] += dataS;
        aux+=extraS;
        pos++;
    }
    if(SS>0){
        MV->registro[SS]=pos<<16;
        MV->segmento[pos] =  aux  < 16;
        MV->segmento[pos] += dataS;
        aux+=stackS;
        pos++;
    }

    if (aux>tamanoMemoria*1024){
        printf("espacio insuficiente en la memoria\n");
        exit(0);
    }
}

void leeVersion1(maquinaVirtual *MV,FILE *arch,int tamanoMemoria){
    char aux;
    int i=0;
    while(!feof(arch)){
        fread(&aux,1,1,arch);
        MV->memoria[i]=aux;
        i++;
    }
    i--;
    inicializaTablaSegmentos(MV,i,tamanoMemoria-i,0,0,0,tamanoMemoria);
    MV->registro[IP]=0;
}
void leeVerision2VMX(maquinaVirtual *MV,FILE *arch,int tamanoMemoria){
    uint16_t codeS,dataS,extraS,stackS,constS;
    int flag,offsetIP;
    char aux;
    fread(&codeS, 2, 1, arch);
    fread(&dataS, 2, 1, arch);
    fread(&extraS, 2, 1, arch);
    fread(&stackS, 2, 1, arch);
    fread(&constS, 2, 1, arch);
    fread(&offsetIP, 2, 1, arch);
    inicializaTablaSegmentos(MV,codeS,dataS,extraS,stackS,constS,tamanoMemoria);
    flag=constS + codeS;
    for(int i=0;i<flag;i++){
        fread(&(MV->memoria[i]),1,1,arch);
    }
    MV->registro[IP]=constS<<16;
    MV->registro[IP]+=offsetIP;
}
void leeVerision2VMI(maquinaVirtual *MV,FILE *arch,int tamanoMemoria){
    uint16_t tamanoMemoriaP;
    int flag ;
    int i;
    fread(&tamanoMemoriaP,1,2,arch);
    for(i=0;i<16;i++){
        fread(&(MV->registro[i]),4,1,arch);
    }
    for(i=0;i<8;i++){
        fread(&(MV->segmento[i]),4,1,arch);
    }
    flag =( MV->segmento[((MV->registro[CS] >>16)&0x0000FFFF)] & 0x0000FFFF ) + ( MV->segmento[((MV->registro[KS] >>16)&0x0000FFFF)] & 0x0000FFFF );

    for(i=0;i<flag;i++){
        fread(&(MV->memoria[i]),1,1,arch);
    }

}
void leeARG(int argc,char *argv[],int *tamanoMemoria,int *mostrarAssembler,char *nombreArchivoVMX,char *nombreArchivoVMI){

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            *mostrarAssembler = 1;
        } else if (strncmp(argv[i], "m=", 2) == 0) {
            *tamanoMemoria = atoi(argv[i] + 2);
        }else if (strstr(argv[i], ".vmx") != NULL) {
            nombreArchivoVMX = argv[i];
        }
        else {
            nombreArchivoVMI = argv[i];
        }
    }
}
void leeARGforDebugger(int argc,char *argv[],int *tamanoMemoria,int *mostrarAssembler,char **nombreArchivoVMX,char **nombreArchivoVMI){
        *mostrarAssembler = 1;
        *nombreArchivoVMX = malloc(strlen("prueba1") + 1);
        strcpy(*nombreArchivoVMX,"prueba1");
}
int leeArch(maquinaVirtual *MV,int argc,char *argv[],int *mostrarAssembler){
    FILE *arch;
    char identificador[5],*nombreArchivoVMX = NULL, *nombreArchivoVMI = NULL;
    int tamanoMemoria = 16,aux=0;
    uint8_t version;
    //leeARG(argc,argv,&tamanoMemoria,&mostrarAssembler,nombreArchivoVMX,nombreArchivoVMI);
    leeARGforDebugger(argc,argv,&tamanoMemoria,mostrarAssembler,&nombreArchivoVMX,&nombreArchivoVMI);
    if(nombreArchivoVMX!=NULL){
        arch = fopen(nombreArchivoVMX,"rb");
        aux=1;
    }
    else{
        arch = fopen(nombreArchivoVMI,"rb");
        aux=1;
    }
    if(arch){
        aux=1;
        fread(identificador, 1, 5, arch);
        fread(&version, 1, 1, arch);
        if(version==1){
            leeVersion1(MV,arch,tamanoMemoria*1024);
        }
        else{
            if(!nombreArchivoVMX)
                leeVerision2VMI(MV,arch,tamanoMemoria);
            else
                leeVerision2VMX(MV,arch,tamanoMemoria);
        }
        fclose(arch);
    }
    return aux;
}

