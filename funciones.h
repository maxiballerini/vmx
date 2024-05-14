#include "componentes.h"
#include "const.h"
void AnalizaSegmento(maquinaVirtual *MV,int posicion,int cant);
int obtienePunteroMemoria(maquinaVirtual *MV,int op);
int leememoria(maquinaVirtual *MV,int cant,int posicion);
void escribememoria(maquinaVirtual *MV,int cant,int posicion,int dato);
int leeregistro(maquinaVirtual *MV,int quebyte,int Reg);
void escriberegistro(maquinaVirtual *MV,int Reg,int dato);
int obtieneOP(maquinaVirtual *MV,int OP,int tipoOP);
void escribePila(maquinaVirtual *,int);
int leePila(maquinaVirtual *);
