#include <stdio.h>
#include <stdlib.h>
#include "disassembler.h"
#include "componentes.h"
#include "ejecucion.h"
#include "fncArch.h"
int main(int argc,char *argv[]){
    maquinaVirtual MV;
    int  mostrarAssembler = 0;
    char *VMI=NULL;
    if(leeArch(&MV,argc,argv,&mostrarAssembler,&VMI)){
        if(mostrarAssembler){
            printf("ejecutando dissasembler\n");
            ejecutadisasssembler(MV);
        }
        printf("\n---------------------------------------------------------------------\n");
        ejecutaInstrucciones(&MV,VMI);
    }
    else
        printf("el archivo no se pudo abrir");
    printf("%d",MV.registro[EBX]);
    return 0;
}
