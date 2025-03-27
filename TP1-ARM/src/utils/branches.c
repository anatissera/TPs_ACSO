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

   }
   

    
}

void Br(uint32_t instruction){
    /*
    31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
    1  1  0  1  0  1  1 |0  0 |0  0 |1  1  1  1  1 |0  0  0  0  0  0 |Rn       |0 0 0 0 0
    */
   uint32_t Rn_num = (instruction >> 5) & 0x1F;




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

}

void Cbz(uint32_t instruction){
    /*
    31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
    sf  0  1  1  0  1  0 |0| imm19                                             |Rt
    */

    uint32_t Rt_num = instruction & 0x1F;

    int64_t imm19 = (instruction >> 5) & 0x7FFFF;

    if (imm19 & (1<< 18)){ // bit extend
        imm19 |= ~((1LL << 19)-1);
    }

    uint64_t Rt_val = CURRENT_STATE.REGS[Rt_num];

     

}

void Cbnz(uint32_t instruction){
    /*
    31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
    sf  0  1  1  0  1  0 |1| imm19                                             |Rt
    */

    uint32_t Rt_num = instruction & 0x1F;

    int64_t imm19 = (instruction >> 5) & 0x7FFFF;

    if (imm19 & (1<< 18)){ // bit extend
        imm19 |= ~((1LL << 19)-1);
    }

    uint64_t Rt_val = CURRENT_STATE.REGS[Rt_num];
}

/*
1. B() – Salto incondicional
¿Qué hace?
Realiza un salto incondicional a una dirección calculada a partir de un desplazamiento de 26 bits.
Proceso:
Extrae el desplazamiento de 26 bits.
Lo extiende con signo a 64 bits.
Lo multiplica por 4 (alineación).
Suma el desplazamiento al CURRENT_STATE.PC para actualizar el NEXT_STATE.PC.
Ejemplo en ARM64:
B label    ; Salta incondicionalmente a 'label'


2. Br() – Salto indirecto por registro
¿Qué hace?
Realiza un salto incondicional a la dirección almacenada en un registro.
Proceso:
Extrae el número del registro Rn (5 bits).
Actualiza el NEXT_STATE.PC con el valor de CURRENT_STATE.REGS[Rn].
Ejemplo en ARM64:
BR X1      ; Salta a la dirección contenida en el registro X1


3. Bcond() – Salto condicional basado en flags
¿Qué hace?
Realiza un salto condicional basado en los valores de los flags (Z, N, etc.).
Proceso:
Extrae el desplazamiento de 19 bits y lo extiende con signo.
Extrae el código de condición (4 bits).
Evalúa la condición según los flags:
0 → BEQ (Z == 1)
1 → BNE (Z == 0)
10 → BGT (Z == 0 y N == V)
11 → BLT (N != V)
12 → BGE (N == V)
13 → BLE (Z == 1 o N != V)
Si la condición se cumple, actualiza el NEXT_STATE.PC con el desplazamiento.
Ejemplo en ARM64:
B.EQ label ; Salta si el flag Z está en 1 (igual)


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