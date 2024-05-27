#ifndef COMPONENTES_H
#define COMPONENTES_H

typedef struct {
    char memoria[65535];
    int registro[18];
    int segmento[8];
} maquinaVirtual;

typedef void (*FuncPtr2)(maquinaVirtual *, int, int, char, char);
typedef void (*FuncPtr1)(maquinaVirtual *, int, char);
typedef void (*FuncPtr0)(maquinaVirtual *);

#endif // COMPONENTES_H
