#include "componentes.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "const.h"
void inicializaTablaSegmentos(maquinaVirtual *MV,uint16_t codeS,uint16_t dataS,uint16_t extraS,uint16_t stackS,uint16_t constS,int tamanoMemoria){
    int aux=0,pos=0;
    MV->registro[KS] = -1;
    MV->registro[CS] = -1;
    MV->registro[DS] = -1;
    MV->registro[ES] = -1;
    MV->registro[SS] = -1;
    if(constS>0){
        MV->registro[KS]=pos<<16;
        MV->segmento[pos] = aux << 16;
        MV->segmento[pos] += constS;
        aux+=constS;
        pos++;
    }
    if(codeS>0){
        MV->registro[CS]=pos<<16;
        MV->segmento[pos] =  aux  << 16;
        MV->segmento[pos] += codeS;
        aux+=codeS;
        MV->registro[IP]=pos<<16;
        pos++;
    }
    if(dataS>0){
        MV->registro[DS]=pos<<16;
        MV->segmento[pos] =  aux  << 16;
        MV->segmento[pos] += dataS;
        aux+=dataS;
        pos++;
    }
    if(extraS>0){
        MV->registro[ES]=pos<<16;
        MV->segmento[pos] =  aux  << 16;
        MV->segmento[pos] += extraS;
        aux+=extraS;
        pos++;
    }
    if(stackS>0){
        MV->registro[SS]=pos<<16;
        MV->segmento[pos] =  aux  << 16;
        MV->segmento[pos] += stackS;
        aux+=stackS;
        MV->registro[SP]=MV->registro[SS];
        MV->registro[SP]+=stackS;
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
    fread(&aux,1,1,arch);
    fread(&aux,1,1,arch);
    while(!feof(arch)){
        fread(&aux,1,1,arch);
        MV->memoria[i]=aux;
        i++;
    }
    i--;
    printf("%d",i);
    inicializaTablaSegmentos(MV,i,tamanoMemoria-i,0,0,0,tamanoMemoria);
    MV->registro[IP]=0;
}
void readInt2bytes(uint16_t *aaa,FILE *arch){
    uint8_t aux;
    uint16_t aux2;
    fread(&aux, 1, 1, arch);
    aux2=aux;
    aux2=aux2<<8;
    fread(&aux, 1, 1, arch);
    aux2+=aux;
    *aaa=aux2;
}
void readInt4bytes(int *aaa,FILE *arch){
    char aux;
    int aux2=0;
    fread(&aux, 1, 1, arch);
    aux2=aux2|(aux&0x000000FF);
    aux2=aux2<<8;
    fread(&aux, 1, 1, arch);
    aux2=aux2|(aux&0x000000FF);
    aux2=aux2<<8;
    fread(&aux, 1, 1, arch);
    aux2=aux2|(aux&0x000000FF);
    aux2=aux2<<8;
    fread(&aux, 1, 1, arch);
    aux2=aux2|(aux&0x000000FF);
    *aaa=aux2;
}
void leeVerision2VMX(maquinaVirtual *MV,FILE *arch,int tamanoMemoria){
    uint16_t codeS,dataS,extraS,stackS,constS,offsetIP;
    int i;
    readInt2bytes(&codeS,arch);
    readInt2bytes(&dataS,arch);
    readInt2bytes(&extraS,arch);
    readInt2bytes(&stackS,arch);
    readInt2bytes(&constS,arch);
    readInt2bytes(&offsetIP,arch);
    inicializaTablaSegmentos(MV,codeS,dataS,extraS,stackS,constS,tamanoMemoria);
    i=MV->segmento[(MV->registro[CS])>>16]>>16;
    for(int j=0;j<codeS;j++){
        fread(&(MV->memoria[i+j]),1,1,arch);
    }
    i=MV->segmento[(MV->registro[KS])>>16]>>16;
    for(int j=0;j<constS;j++){
        fread(&(MV->memoria[i+j]),1,1,arch);
    }
    MV->registro[IP]+=offsetIP;

}
void leeVerision2VMI(maquinaVirtual *MV,FILE *arch,int tamanoMemoria){
    uint16_t tamanoMemoriaP;
    char aux;
    int i,aux2;
    readInt2bytes(&tamanoMemoriaP,arch);
    for(i=0;i<16;i++){
        readInt4bytes(&aux2,arch);
        MV->registro[i]=aux2;
    }
    for(i=0;i<8;i++){
        readInt4bytes(&aux2,arch);
        MV->segmento[i]=aux2;
    }
    i=0;
    while(!feof(arch)){
        fread(&aux,1,1,arch);
        MV->memoria[i]=aux;
        i++;
    }

}
void leeARG(int argc,char *argv[],int *tamanoMemoria,int *mostrarAssembler,char **nombreArchivoVMX,char **nombreArchivoVMI){

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            *mostrarAssembler = 1;
        } else if (strncmp(argv[i], "m=", 2) == 0) {
            *tamanoMemoria = atoi(argv[i] + 2);
        }else if (strstr(argv[i], ".vmx") != NULL) {
            *nombreArchivoVMX = argv[i];
        }
        else {
            *nombreArchivoVMI = argv[i];
        }
    }
}

int leeArch(maquinaVirtual *MV,int argc,char *argv[],int *mostrarAssembler,char **VMI){
    FILE *arch;
    char identificador[5],*nombreArchivoVMX = NULL, *nombreArchivoVMI = NULL,buffer[6];
    int tamanoMemoria = 16,aux=0;
    uint8_t version;
    leeARG(argc,argv,&tamanoMemoria,mostrarAssembler,&nombreArchivoVMX,&nombreArchivoVMI);
    if(nombreArchivoVMX!=NULL){
        arch = fopen(nombreArchivoVMX,"rb");
        aux=1;
    }
    else{
        arch = fopen(nombreArchivoVMI,"rb");
        aux=1;
    }
    if(nombreArchivoVMI!=NULL){
        *VMI=nombreArchivoVMI;
    }
    if(arch){
        aux=1;
        fread(buffer, sizeof(char), sizeof(identificador), arch);
        buffer[5]='\0';
        fread(&version, 1, 1, arch);
        if(strcmp(buffer,"VMX24")==0){
            if(version==1){
                leeVersion1(MV,arch,tamanoMemoria*1024);
            }
            else
                leeVerision2VMX(MV,arch,tamanoMemoria);
            }
        else{
            leeVerision2VMI(MV,arch,tamanoMemoria);
        }
        fclose(arch);
    }
    return aux;
}

