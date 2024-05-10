
//2 operandos
void MOV(maquinaVirtual *MV,int,int,char,char);
void ADD(maquinaVirtual *MV,int,int,char,char);
void SUB (maquinaVirtual *MV,int,int,char,char);
void SWAP(maquinaVirtual *MV,int,int,char,char);
void MUL (maquinaVirtual *MV,int,int,char,char);
void DIV (maquinaVirtual *MV,int,int,char,char);
void CMP (maquinaVirtual *MV,int,int,char,char);
void SHL (maquinaVirtual *MV,int,int,char,char);
void SHR (maquinaVirtual *MV,int,int,char,char);
void AND (maquinaVirtual *MV,int,int,char,char);
void OR (maquinaVirtual *MV,int,int,char,char);
void XOR (maquinaVirtual *MV,int,int,char,char);
void RND (maquinaVirtual *MV,int,int,char,char);

//1 opereando
void SYS (maquinaVirtual *MV,int,char);
void JMP (maquinaVirtual *MV,int,char);
void JZ (maquinaVirtual *MV,int,char);
void JP (maquinaVirtual *MV,int,char);
void JN (maquinaVirtual *MV,int,char);
void JNZ (maquinaVirtual *MV,int,char);
void JNP (maquinaVirtual *MV,int,char);
void JNN (maquinaVirtual *MV,int,char);
void LDL (maquinaVirtual *MV,int,char);
void LDH (maquinaVirtual *MV,int,char);
void NOT (maquinaVirtual *MV,int,char);
void PUSH (maquinaVirtual *MV,int,char);
void POP (maquinaVirtual *MV,int,char);
void CALL (maquinaVirtual *MV,int,char);

//0 operandos
void STOP (maquinaVirtual *MV);
void RET (maquinaVirtual *MV);
