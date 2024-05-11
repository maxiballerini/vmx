#include <stdio.h>
#include <stdlib.h>
#include "disassembler.h"
#include "componentes.h"
#include "ejecucion.h"
#include "fncArch.h"
int main(int argc,char *argv[]){
    maquinaVirtual MV;
    int  mostrarAssembler = 0;
    if(leeArch(&MV,argc,argv,&mostrarAssembler)){
        if(mostrarAssembler){
            printf("ejecutando dissasembler\n");
            ejecutadisasssembler(MV);
        }
        printf("\n---------------------------------------------------------------------\n");
        ejecutaInstrucciones(&MV);
    }
    else
        printf("el archivo no se pudo abrir");
    printf("%d \n%d \n%d \n",MV.memoria[40],MV.memoria[41],MV.memoria[43]);
    return 0;
}
