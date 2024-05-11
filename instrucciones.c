#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "funciones.h"
//2 operandos
void MOV(maquinaVirtual*MV,int opA,int opB,char tipoA,char tipoB){
    int datoB = obtieneOP(MV,opB,tipoB);
    if(tipoA==3){
        escribememoria(MV,cantMemoria(opA),obtienePunteroMemoria(MV,opA),datoB);
    }
    else
        escriberegistro(MV,opA,datoB);
}
void ADD(maquinaVirtual *MV, int opA, int opB, char tipoA, char tipoB) {
    int datoB = obtieneOP(MV,opB,tipoB);
    int datoA = obtieneOP(MV,opA,tipoA);
    datoA=datoA+datoB;
    if(datoA==0)
        MV->registro[CC]=0x00000001;
    else if(datoA<0){
        MV->registro[CC]=0x80000000;
    }
    else
        MV->registro[CC]=0;
    if(tipoA==3)
        escribememoria(MV,cantMemoria(opA),obtienePunteroMemoria(MV,opA),datoA);
    else
        escriberegistro(MV,opA,datoA);

}

void SUB(maquinaVirtual *MV, int opA, int opB, char tipoA, char tipoB) {
    int datoB = obtieneOP(MV,opB,tipoB);
    int datoA = obtieneOP(MV,opA,tipoA);
    datoA=datoA-datoB;
    if(datoA==0)
        MV->registro[CC]=0x00000001;
    else if(datoA<0){
        MV->registro[CC]=0x80000000;
    }
    else
        MV->registro[CC]=0;
    if(tipoA==3)
        escribememoria(MV,cantMemoria(opA),obtienePunteroMemoria(MV,opA),datoA);
    else
        escriberegistro(MV,opA,datoA);
}

void SWAP(maquinaVirtual *MV, int opA, int opB, char tipoA, char tipoB) {
    int datoB = obtieneOP(MV,opB,tipoB);
    int datoA = obtieneOP(MV,opA,tipoA);
    if(tipoA==3){
        escribememoria(MV,cantMemoria(opA),obtienePunteroMemoria(MV,opA),datoB);
        escribememoria(MV,cantMemoria(opA),obtienePunteroMemoria(MV,opB),datoA);
    }
    else{
        escriberegistro(MV,opA,datoB);
        escriberegistro(MV,opB,datoA);
    }
}

void MUL(maquinaVirtual *MV, int opA, int opB, char tipoA, char tipoB) {
    int datoB = obtieneOP(MV,opB,tipoB);
    int datoA = obtieneOP(MV,opA,tipoA);
    datoA=datoA*datoB;
    if(datoA==0)
        MV->registro[CC]=0x00000001;
    else if(datoA<0){
        MV->registro[CC]=0x80000000;
    }
    else
        MV->registro[CC]=0;
    if(tipoA==3)
        escribememoria(MV,cantMemoria(opA),obtienePunteroMemoria(MV,opA),datoA);
    else
        escriberegistro(MV,opA,datoA);
}

void DIV(maquinaVirtual *MV, int opA, int opB, char tipoA, char tipoB) {
    int datoB = obtieneOP(MV,opB,tipoB);
    int datoA = obtieneOP(MV,opA,tipoA);
    if (datoB == 0) {
        printf("Error: División por cero.\n");
        exit(0);
    }

    MV->registro[AC] = datoA % datoB;
    datoA = datoA / datoB;

    if(datoA==0)
        MV->registro[CC]=0x00000001;
    else if(datoA<0){
        MV->registro[CC]=0x80000000;
    }
    else
        MV->registro[CC]=0;

    if(tipoA==3)
        escribememoria(MV,cantMemoria(opA),obtienePunteroMemoria(MV,opA),datoA);
    else
        escriberegistro(MV,opA,datoA);
}

void CMP(maquinaVirtual *MV, int opA, int opB, char tipoA, char tipoB) {
    int datoB = obtieneOP(MV,opB,tipoB);
    int datoA = obtieneOP(MV,opA,tipoA);
    datoA=datoA-datoB;
    if(datoA==0)
        MV->registro[CC]=0x00000001;
    else if(datoA<0){
        MV->registro[CC]=0x80000000;
    }
    else
        MV->registro[CC]=0;
}

void SHL(maquinaVirtual *MV, int opA, int opB, char tipoA, char tipoB) {
    int datoB = obtieneOP(MV,opB,tipoB),secReg;
    int datoA = obtieneOP(MV,opA,tipoA);
    datoA=datoA<<datoB;
    if(tipoA==3)
        escribememoria(MV,cantMemoria(opA),obtienePunteroMemoria(MV,opA),datoA);
    else{
        secReg=(opA & 0x000000F0)>>4;
        if((secReg == LOW) || (secReg = HIGH)){
            datoA=datoA & 0x000000FF;
        }
        else if(secReg==X){
            datoA=datoA & 0x0000FFFF;
        }
        escriberegistro(MV,opA,datoA);
    }
}

void SHR(maquinaVirtual *MV, int opA, int opB, char tipoA, char tipoB) {
    int datoB = obtieneOP(MV,opB,tipoB),secReg;
    int datoA = obtieneOP(MV,opA,tipoA);
    datoA=datoA>>datoB;
    if((datoA & 0x80000000) == 0x80000000){
        for(int i=1;i<datoB;i++){
            datoA |= (1 << (31 - i));
        }
    }
    if(tipoA==3)
        escribememoria(MV,cantMemoria(opA),obtienePunteroMemoria(MV,opA),datoA);
    else{
        secReg=(opA & 0x000000F0)>>4;
        if((secReg == LOW) || (secReg = HIGH)){
            datoA=datoA & 0x000000FF;
        }
        else if(secReg==X){
            datoA=datoA & 0x0000FFFF;
        }
        escriberegistro(MV,opA,datoA);
    }
}

void AND(maquinaVirtual *MV, int opA, int opB, char tipoA, char tipoB) {
    int datoB = obtieneOP(MV,opB,tipoB);
    int datoA = obtieneOP(MV,opA,tipoA);
    datoA=datoA&datoB;
    if(datoA==0)
        MV->registro[CC]=0x00000001;
    else if(datoA<0){
        MV->registro[CC]=0x80000000;
    }
    else
        MV->registro[CC]=0;
    if(tipoA==3)
        escribememoria(MV,cantMemoria(opA),obtienePunteroMemoria(MV,opA),datoA);
    else
        escriberegistro(MV,opA,datoA);
}

void OR(maquinaVirtual *MV, int opA, int opB, char tipoA, char tipoB) {
       int datoB = obtieneOP(MV,opB,tipoB);
    int datoA = obtieneOP(MV,opA,tipoA);
    datoA=datoA|datoB;
    if(datoA==0)
        MV->registro[CC]=0x00000001;
    else if(datoA<0){
        MV->registro[CC]=0x80000000;
    }
    else
        MV->registro[CC]=0;
    if(tipoA==3)
        escribememoria(MV,cantMemoria(opA),obtienePunteroMemoria(MV,opA),datoA);
    else
        escriberegistro(MV,opA,datoA);
}

void XOR(maquinaVirtual *MV, int opA, int opB, char tipoA, char tipoB) {
       int datoB = obtieneOP(MV,opB,tipoB);
    int datoA = obtieneOP(MV,opA,tipoA);
    datoA=datoA^datoB;
    if(datoA==0)
        MV->registro[CC]=0x00000001;
    else if(datoA<0){
        MV->registro[CC]=0x80000000;
    }
    else
        MV->registro[CC]=0;
    if(tipoA==3)
        escribememoria(MV,cantMemoria(opA),obtienePunteroMemoria(MV,opA),datoA);
    else
        escriberegistro(MV,opA,datoA);
}

void RND(maquinaVirtual *MV, int opA, int opB, char tipoA, char tipoB) {
    int datoB = obtieneOP(MV,opB,tipoB);
    srand(time(NULL));
    datoB = rand() % (datoB+1);

    if(tipoA==3)
        escribememoria(MV,cantMemoria(opA),obtienePunteroMemoria(MV,opA),datoB);
    else
        escriberegistro(MV,opA,datoB);
}


//CORREGIR READ Y WRITE
void read(maquinaVirtual *MV){
    int formato=MV->registro[10]&0x000000FF;
    int CH=(MV->registro[12]&0x0000FF00)>>8;
    int CL=MV->registro[12]&0x000000FF,i;
    int aux;
    char c;
    int posicion =obtienePunteroMemoria(MV,13<<16);
    switch (formato) {
        case 4:
            for(i=0;i<CL;i++){
                printf("Introduce un carácter en formato octal: \n");
                scanf("%o", &aux);
                escribememoria(MV,CH,posicion+CH*i,aux);
            }
            break;
        case 8:
            for(i=0;i<CL;i++){
                printf("Introduce un carácter en formato hexadecimla: \n");
                scanf("%x", &aux);
                escribememoria(MV,CH,posicion+CH*i,aux);
            }
            break;
        case 1:
            for(i=0;i<CL;i++){
                printf("Introduce un carácter en formato decimal: \n");
                scanf("%d", &aux);
                escribememoria(MV,CH,posicion+CH*i,aux);
            }
            break;
        case 2:
            for(i=0;i<CL;i++){
                printf("Introduce un carácter: ");
                scanf("%c", &c);
                escribememoria(MV,CH,posicion+CH*i,aux);
            }
            break;
    }
}

void write(maquinaVirtual *MV){
    int formato=MV->registro[10]&0x000000FF;
    int CH=(MV->registro[12]&0x0000FF00)>>8;
    int CL=MV->registro[12]&0x000000FF,i;
    int aux,aux1;
    int posicion =obtienePunteroMemoria(MV,MV->registro[13]);
    for(i=0;i<CL;i++){
        aux = leememoria(MV,CH,posicion+(CH*i));
        printf("[%04X] ",posicion+(CH*i));

        if((formato & 0x00000008 )!=0 )
            printf("%% %X ",aux);

        if((formato & 0x00000004 )!=0 )
            printf("@ %o ",aux);

        if((formato & 0x00000002)!=0 ){
            printf("' ");
            aux=aux<<(24-(CH-1)*8);
            for(int j=0;j<CH;j++){
                aux1 =aux >>(24-8*j);
                if(aux1==127 ||(aux>=0 &&aux1<=31))
                    printf(".");
                else
                    printf("%c",aux1);
            }
            printf(" ");
        }

        if((formato & 0x00000001 )!=0 )
            printf("# %d",aux);

        printf("\n");
    }
}
//1 opereando
void SYS (maquinaVirtual *MV,int opA,char tipoA){
    FuncPtr0 funcionesSYS[2] = {read,write};
    int indice = obtieneOP(MV,opA,tipoA);
    funcionesSYS[indice - 1](MV);
}
void JMP (maquinaVirtual *MV,int opA,char tipoA){
    int dato = obtieneOP(MV,opA,tipoA);
    MV->registro[IP]=(MV->registro[IP]&0x0000FFFF) | dato;
}
void JZ (maquinaVirtual *MV,int opA,int tipoA) {
    if(MV->registro[CC]  == 1 ){
        JMP(MV,opA,tipoA);
    }
}
void JP (maquinaVirtual *MV,int opA,int tipoA) {
    if(MV->registro[CC]  == 0 ){
        JMP(MV,opA,tipoA);
    }
}
void JN (maquinaVirtual *MV,int opA,int tipoA) {
    if(MV->registro[CC]  == -1 ){
        JMP(MV,opA,tipoA);
    }
}
void JNZ (maquinaVirtual *MV,int opA,int tipoA){
    if(MV->registro[CC]  == -1 || MV->registro[CC]  == 0){
        JMP(MV,opA,tipoA);
    }
}
void JNP (maquinaVirtual *MV,int opA,int tipoA){
    if(MV->registro[CC]  == 1 || MV->registro[CC]  == -1){
        JMP(MV,opA,tipoA);
    }
}
void JNN (maquinaVirtual *MV,int opA,int tipoA){
    if(MV->registro[CC]  == 1 || MV->registro[CC]  == 0){
        JMP(MV,opA,tipoA);
    }
}
void LDL (maquinaVirtual *MV,int opA,int tipoA){
    int dato=obtieneOP(MV,opA,tipoA);
    MV->registro[AC] = (MV->registro[AC] & 0xFFFF0000) | (dato & 0x0000FFFF);
}
void LDH (maquinaVirtual *MV,int opA,int tipoA){
    int dato=obtieneOP(MV,opA,tipoA);
    MV->registro[AC]=(MV->registro[AC] & 0x0000FFFF) | (dato & 0x0000FFFF)<<16;
}
void NOT (maquinaVirtual *MV,int opA,int tipoA){
    int datoA = obtieneOP(MV,opA,tipoA),secReg;
    datoA=~datoA;
    if(datoA==0)
        MV->registro[CC]=0x00000001;
    else if(datoA<0){
        MV->registro[CC]=0x80000000;
    }
    else
        MV->registro[CC]=0;

    if(tipoA==3)
        escribememoria(MV,cantMemoria(opA),obtienePunteroMemoria(MV,opA),datoA);
    else{
        secReg=(opA & 0x000000F0)>>4;
        if((secReg == LOW) || (secReg = HIGH)){
            datoA=datoA & 0x000000FF;
        }
        else if(secReg==X){
            datoA=datoA & 0x0000FFFF;
        }
        escriberegistro(MV,opA,datoA);
    }
}
void PUSH (maquinaVirtual *MV,int opA,int tipoA){
    int dato;
    MV->registro[SP]-=4;
    if( MV->registro[SP]< MV->registro[SS]){
        printf("STACK OVERFLOW\n");
        exit(0);
    }
    dato=obtieneOP(MV,opA,tipoA);
    escribePila(MV,dato);
}
void POP (maquinaVirtual *MV,int opA,int tipoA){

}
void CALL (maquinaVirtual *MV,int opA,int tipoA){

}
//0 operandos
void STOP (maquinaVirtual *MV){
    exit(0);
}
void RET (maquinaVirtual *MV){
    exit(0);
}
