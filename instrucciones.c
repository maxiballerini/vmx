#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "funciones.h"
#include "ejecucion.h"
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
        escribememoria(MV,cantMemoria(opB),obtienePunteroMemoria(MV,opA),datoB);
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
        printf("Error: Division por cero.\n");
        printf("en linea %04X",MV->registro[IP]&0x0000FFFF);
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
void readS(maquinaVirtual *MV){
    int pos;
    char aux[50];
    pos=obtienePunteroMemoria(MV,0x000D0000);
    scanf("%s",aux);
    for(int i =0;i<sizeof(aux);i++){
        MV->memoria[pos+i]=aux[i];
    }
}
void writeS(maquinaVirtual *MV){
    int pos=obtienePunteroMemoria(MV,0x000D0000);
    while(MV->memoria[pos]!='\0'){
        printf("%c",MV->memoria[pos++]);
    }
}
void clear(){
    system("cls");
}
void breakpoint(maquinaVirtual *MV,char *VMI){
    FILE *arch;
    if (VMI!=NULL){
        arch = fopen(VMI,"wb");
        if(arch){
            char caracter;
            printf("Presiona 'q' 'g' o enter \n");
            while (1) {
                fwrite("VMI24188",1,8,arch);
                fwrite(MV->registro,sizeof(MV->registro),1,arch);
                fwrite(MV->segmento,sizeof(MV->segmento),1,arch);
                fwrite(MV->memoria,sizeof(MV->memoria),1,arch);
                scanf("%c",&caracter);

                if (caracter == 'q') {
                    fclose(arch);
                    exit(0);
                } else if (caracter == 'g') {
                    fclose(arch);
                    return;
                } else if (caracter == '\n') {
                 //   STEP(MV,VMI);
                }
                else{
                    printf("caracter invalido");
                    exit(1);
                }
            }
        }
        else
            printf("el archivo no se pudo abrir");
    }
}
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
                if(aux>=33 &&aux1<=126)
                    printf("%c",aux1);
                else
                    printf(".");
            }
            printf(" ");
        }

        if((formato & 0x00000001 )!=0 )
            printf("# %d",aux);

        printf("\n");
    }
}
//1 opereando
void SYS (maquinaVirtual *MV,int opA,char tipoA,char *VMI){
    FuncPtr0 funcionesSYS[4] = {read,write,readS,writeS};
    int indice = obtieneOP(MV,opA,tipoA);
    if (indice>=7)
        if(indice==15)
            breakpoint(MV,VMI);
        else
            clear();
    else
        funcionesSYS[indice - 1](MV);
}
void JMP (maquinaVirtual *MV,int opA,char tipoA){
    int dato = obtieneOP(MV,opA,tipoA);
    MV->registro[IP]=(MV->registro[IP]&0xFFFF0000) | (dato&0x0000FFFF);
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
    dato=obtieneOP(MV,opA,tipoA);
    escribePila(MV,dato);
}
void POP (maquinaVirtual *MV,int opA,int tipoA){
    int dato;
    dato = leePila(MV);
     if(tipoA==3){
        escribememoria(MV,cantMemoria(opA),obtienePunteroMemoria(MV,opA),dato);
    }
    else
        escriberegistro(MV,opA,dato);
}
void CALL (maquinaVirtual *MV,int opA,int tipoA){
    PUSH(MV,IP,1);
    JMP(MV,opA,tipoA);

}
//0 operandos
void STOP (maquinaVirtual *MV){
    exit(0);
}
void RET (maquinaVirtual *MV){
    POP(MV,IP,1);
}
