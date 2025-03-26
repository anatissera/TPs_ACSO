#include "structures.h"

/*
B 
b target

BR 
br X1

BEQ (B.Cond) 
cmp X1,X2 
beq target

BNE (B.Cond) 
cmp X1,X2 
bne target

BGT (B.Cond) 
cmp X1,X2 
bgt target

BLT (B.Cond) 
cmp X1,X2 
blt target

BGE (B.Cond) 
cmp X1,X2 
bge target

BLE (B.Cond) 
cmp X1,X2 
ble target

CBZ 
cbz X3, label 
 
CBNZ 
cbnz X3, label

*/

void B(uint32_t instruction);
void Br(uint32_t instruction);
void B_cond(uint32_t instruction);

void Cbz(uint32_t instruction); 
void Cbnz(uint32_t instruction);