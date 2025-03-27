#include "branches.h"

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

void B(uint32_t instruction){
    /*
    31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
    0 |0  0  1  0  1 |imm26 (26 bits)
    */
    int64_t imm26 = instruction & 0x3FFFFFF;

    if (imm26 & (1<<25)){
        imm26 |= ~((1LL << 26)-1);
    }

    int64_t offset = imm26 << 2;//Lo multiplica por 4 (alineación).
    NEXT_STATE.PC = CURRENT_STATE.PC + offset;

    
}

void Br(uint32_t instruction){
    /*
    31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
    1  1  0  1  0  1  1 |0  0 |0  0 |1  1  1  1  1 |0  0  0  0  0  0 |Rn       |0 0 0 0 0
    */

    uint32_t Rn_num = (instruction >> 5) & 0x1F;
    NEXT_STATE.PC = CURRENT_STATE.REGS[Rn_num];
}

void B_cond(uint32_t instruction){
    /*
    31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
    0  1  0  1  0  1  0 |0 |imm19 (19 bits)                                    |0 |cond
    */

    uint32_t cond = instruction & 0xF;
    int64_t imm19 = (instruction >> 5) & 0x7FFFF;

    if (imm19 & (1<< 18)){ // bit extend a 64
        imm19 |= ~((1LL << 19)-1);
    }

    int64_t target_address = CURRENT_STATE.PC + (imm19 << 2);

    // cond	          Condición          	   Evaluación con V = 0, C = 0
    // 0000 (0)	      BEQ (Z == 1)	           Z == 1
    // 0001 (1)	      BNE (Z == 0)	           Z == 0
    // 1010 (10)	  BGT (Z == 0 y N == V)	   Z == 0 y N == 0
    // 1011 (11)	  BLT (N != V)	           N != 0
    // 1100 (12)	  BGE (N == V)	           N == 0
    // 1101 (13)	  BLE (Z == 1 o N != V)	   Z == 1 o N != 0

    switch (cond) {
        case 0b0000:  // BEQ (Z == 1)
            if (FLAG_Z) { NEXT_STATE.PC = target_address; }
            break;
        case 0b0001:  // BNE (Z == 0)
            if (!FLAG_Z) { NEXT_STATE.PC = target_address; }
            break;
        case 0b1010:  // BGT (Z == 0 y N == 0)
            if (!FLAG_Z && !FLAG_N) { NEXT_STATE.PC = target_address; }
            break;
        case 0b1011:  // BLT (N != 0)
            if (FLAG_N) { NEXT_STATE.PC = target_address; }
            break;
        case 0b1100:  // BGE (N == 0) 
            if (!FLAG_N) { NEXT_STATE.PC = target_address; }
            break;
        case 0b1101:  // BLE (Z == 1 o N != 0)
            if (FLAG_Z || FLAG_N) { NEXT_STATE.PC = target_address; }
            break;
        default:
            break;
    }
}

void Cbz_Cbnz(uint32_t instruction){
    /*
    31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
    sf  0  1  1  0  1  0 |op| imm19                                             |Rt
    op = 0 -> Cbz
    op = 1 -> Cbnz
    */

    uint32_t Rt_num = instruction & 0x1F;

    int64_t imm19 = (instruction >> 5) & 0x7FFFF;

    if (imm19 & (1<< 18)){ // bit extend
        imm19 |= ~((1LL << 19)-1);
    }

    uint64_t Rt_val = CURRENT_STATE.REGS[Rt_num];

    uint32_t op = (instruction >> 24) & 0b1;

    int64_t branch_target = (int64_t)CURRENT_STATE.PC + ((int64_t)imm19 << 2);

    if (!op) { // CBZ
        if (Rt_val == 0) {
            CURRENT_STATE.PC = branch_target;
        }
    } else { // CBNZ
        if (Rt_val != 0) {
            CURRENT_STATE.PC = branch_target;
        }
    }

}

/*

4. cbz_cbnz() – Salto condicional por comparación a cero
¿Qué hace?
Realiza un salto condicional dependiendo de si el contenido de un registro es cero o no es cero.
Proceso:
Extrae el desplazamiento de 19 bits y lo extiende con signo.
Obtiene el número del registro Rt.
Si el valor del registro cumple con la condición (cero o no cero), actualiza el NEXT_STATE.PC.
Uso de la función unificada:
cbz_cbnz(restOfInstruction, true);  // Para CBZ (si el registro es cero)
cbz_cbnz(restOfInstruction, false); // Para CBNZ (si el registro NO es cero)

Ejemplos en ARM64:
CBZ X1, label  ; Salta si X1 == 0
CBNZ X1, label ; Salta si X1 != 0

*/