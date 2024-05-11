#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "componentes.h"
#include "instrucciones.h"
#include "const.h"
void ejecutaInstrucciones(maquinaVirtual MV){

    char aux;
    int i, opA,opB,top,bottom,auxIP;
    char operandoA,operandoB;
    char mascara0Operando= 0xFF,mascara2Operando= 0x10;
    FuncPtr2 funciones2OP[13] = {MOV,ADD,SUB,SWAP,MUL,DIV,CMP,SHL,SHR,AND,OR,XOR,RND};
    FuncPtr1 funciones1OP[14] = {SYS,JMP,JZ,JP,JN,JNZ,JNP,JNN,LDL,LDH,NOT,PUSH,POP,CALL};
    FuncPtr0 funciones0OP[2] = {STOP,RET};
    bottom=(MV.segmento[MV.registro[CS]>>16]>>16)&0x0000FFFF;
    top=bottom+(MV.segmento[MV.registro[CS]>>16]&0x0000FFFF)-1;
    do{
       // printf("se ejecuto la linea %04X\n",MV.registro[IP]);
        auxIP = ((MV.registro[IP]>>16)& 0x0000FFFF ) + (MV.registro[IP]&0x0000FFFF);
        aux=MV.memoria[auxIP];
        MV.registro[IP]++;
        opB=0;
        opA=0;
        operandoB= (int)((( aux ^ 0b11000000 ) & 0b11000000) >> 6);
        for(i=0;i<operandoB;i++){
            auxIP = ((MV.registro[IP]>>16)& 0x0000FFFF ) + (MV.registro[IP]&0x0000FFFF);
            opB = opB << 8 | (MV.memoria[auxIP] & 0x000000FF);
            MV.registro[IP]++;
        }
        //caso 2 operandos
        if((aux & mascara2Operando) == 0){
            operandoA= (int)((( aux ^ 0b00110000 ) & 0b00110000) >> 4);
            for(i=0;i<operandoA;i++){
                auxIP = ((MV.registro[IP]>>16)& 0x0000FFFF ) + (MV.registro[IP]&0x0000FFFF);
                opA = opA << 8 | (MV.memoria[auxIP] & 0x000000FF) ;
                MV.registro[IP]++;
            }
            aux=aux&0x0F;
            if(aux>=0 && aux<CANT_FUNCIONES2){
                funciones2OP[(int)aux](&MV,opA,opB,operandoA,operandoB);
            }
            else{
                printf("instruccion invalida");
                exit(0);
            }
        }
        //caso 0 operanods
        else{
            if((aux & mascara0Operando) == mascara0Operando){
                funciones0OP[~aux](&MV);
            }
        //caso 1 operandos
            else{
                aux=aux&0x0F;
                if((int)aux>=0 && (int)aux<CANT_FUNCIONES1){
                    funciones1OP[(int)aux](&MV,opB,operandoB);
                }
                else{
                    printf("Intruccion invalida");
                    exit(0);
                }
            }
        }
    }
    while(auxIP>=bottom && auxIP<top);
}
