#include "componentes.h"
#include "const.h"
#include "stdlib.h"
#include "stdio.h"
int extiendenegativo(int i,int cant){
    if(cant==1){
        return (i | 0xFFFFFF00);
    }
    else{
        return (i | 0xFFFF0000);
    }
}
int cantMemoria(int opA){
    return (~((opA)>>22) & 0x00000003)+1;
}
void AnalizaSegmento(maquinaVirtual *MV,int posicion,int cant,int seg){
    int top,bottom,indice;
     indice =(MV->registro[seg]>>16) & 0x0000FFFF;
    bottom=(MV->segmento[indice]>>16) & 0x0000FFFF;
    top = bottom + (MV->segmento[indice] & 0x0000FFFF);


    if(posicion < bottom || (posicion + cant) >= top){
        printf("segmentation fall en %08X\n",MV->registro[IP] );
        exit(0);
    }
}
int obtienePunteroMemoria(maquinaVirtual *MV,int op){

    int reg,offset1,offset2,sec,indice;

    reg = (op & 0x000F0000)>>16;
    if(reg==0){
        reg=DS;
    }

    sec = MV->registro[reg]>>16;
    indice = (MV->segmento[sec]>>16)&0x0000FFFF;
    offset1=op & 0x0000FFFF;
    offset2=MV->registro[reg]&0x0000FFFF;

    return indice + offset1 + offset2;
}

int leememoria(maquinaVirtual *MV,int cant,int posicion){
    int aux=0;
    AnalizaSegmento(MV,posicion,cant,DS);
    for(int i=0;i<cant;i++){
        aux = aux<<8;
        aux |= MV->memoria[posicion+i] & 0x000000FF;
    }
    return aux;
}
void escribememoria(maquinaVirtual *MV,int cant,int posicion,int dato){
    int aux;
    AnalizaSegmento(MV,posicion,cant,DS);
    for(int i=0;i<cant;i++){
        aux = (dato >>(8*(cant-1) - i*8)) & 0x000000FF;
        MV->memoria[posicion++]=aux;
    }
}

int leeregistro(maquinaVirtual *MV,int quebyte,int Reg){
    int aux = MV->registro[Reg];
    if(quebyte==EX){
        return aux;
    }
    else if(quebyte == LOW){
        aux=aux & 0x000000FF;

        if((aux & 0x00000080)!=0){
            aux = extiendenegativo(aux,1);
        }
        return aux;
    } else if (quebyte == HIGH){
        aux=(aux & 0x0000FF00)>>8;
        if((aux & 0x00000080)!=0){
            aux = extiendenegativo(aux,1);
        }
        return aux;
    } else  {
        aux=aux&0x0000FFFF;
        if((aux & 0x00008000)!=0){
            aux = extiendenegativo(aux,2);
        }
        return aux;
    }
}
void escriberegistro(maquinaVirtual *MV,int Reg,int dato){
    int secReg,codReg;
    codReg=Reg & 0x0000000F;
    secReg= (Reg & 0x000000F0)>> 4;
    if(secReg==EX){
        MV->registro[codReg] = dato;
    }
    else if(secReg== LOW){
        MV->registro[codReg] =  (MV->registro[codReg] & 0xFFFFFF00) | (dato & 0x000000FF);
    } else if (secReg == HIGH){
        MV->registro[codReg] =  (MV->registro[codReg] & 0xFFFF00FF) | ((dato & 0x000000FF)<<8);
    } else{
        MV->registro[codReg] =  (MV->registro[codReg] & 0xFFFF0000) | (dato & 0x0000FFFF);
    }
}
int obtieneOP(maquinaVirtual *MV,int OP,int tipoOP){
    int secReg,codReg;
    if(tipoOP==3){
        return leememoria(MV,cantMemoria(OP),obtienePunteroMemoria(MV,OP));
    }
    else if(tipoOP==1){
        secReg=(OP&0x000000F0)>>4;
        codReg=(OP&0x0000000F);
        return leeregistro(MV,secReg,codReg);
    }
    else{
        if ( (OP&0x00008000) != 0 ){
            OP = extiendenegativo(OP,2);
        }
        return OP;
    }
}
void escribePila(maquinaVirtual *MV,int dato){
    int bottom,indice;
    MV->registro[SP]-=4;
    indice = (MV->registro[SS]>>16) & 0x0000FFFF;
    bottom = (MV->segmento[indice]>>16) & 0x0000FFFF;
    if( MV->registro[SP]- 4 < bottom){
        printf("STACK OVERFLOW\n");
        exit(0);
    }
    escribememoria(MV,4,,dato);
}
int leePila(maquinaVirtual *MV){
    int tope,indice,dato;
    indice = (MV->registro[SS]>>16) & 0x0000FFFF;
    tope = ((MV->segmento[indice]>>16) & 0x0000FFFF)+(MV->segmento[indice] & 0x0000FFFF);
    if( MV->registro[SP]+ 4 >= tope){
        printf("STACK UNDEROVERFLOW\n");
        exit(0);
    }
    for(int i=0;i<4;i++){
        dato=MV->memoria[MV->registro[SP]++];
        dato=dato<<8;
    }
    return dato;
}
