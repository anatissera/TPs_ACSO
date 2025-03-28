#include "math_operations.h"

/* 
Aclaración de Especificaciones: 
1.  Repetimos, solo tienen que hacer la variante de la instrucción para 64 bits.  
2.  ADDS (Extended Register) tiene el siguiente formato en el manual
| 31 30 29 28 | 27 26 25 24 | 23 22 21  20 |       16 | 15   13 12 |   10 9   |    5 4  |    0 |
  sf| 0| 1 |0   1   0  1  1 | 0  0 | 1 |     Rm       | option |  imm3   |     rn   |    rd    |

Pero cuando compilamos la instrucción ADDS Xd, Xn, Xm el bit 21 de output es 0, no 1. 
Por favor asegúrense de que ande ADDS. Lo mismo ocurre con SUBS.

Cuando hacemos ADDS (Extended Register) vemos eso:
No tienen que implementar la parte de extended y amount (O sea, pueden asumir que 
es 0). Solo implementen ADDS Xd, Xn, Xm. Lo mismo con SUBS.

Para ADDS (Immediate), No hay que implementar Reserved Value, pero si los otros dos. Lo mismo con SUBS,  
(y extra credit, add)


ADD (Extended Register & Immediate) 
Immediate: add X0, X1, 3 (descripción: X0 = X1 + 3) 
El caso de shift == 01 se debe implementar, osea moviendo el imm12, 12 bits a la izquierda. 
También se debe implementar shift 00, pero no el caso de ReservedValue. 
 
Extended Register: add X0 = X1, X2 (descripción: X0 = X1 + X2) 
 
ADDS (Extended Register, Immediate): 
Immediate: adds X0, X1, 3 (descripción: X0 = X1 + 3, luego updatear flags) 
El caso de shift == 01 se debe implementar, osea moviendo el imm12, 12 bits a la izquierda. 
Tambien shift 00, pero no el ReservedValue 
 
Extended Register: adds X0 = X1, X2 (descripción: X0 = X1 + X2, luego updatear flags)


COMANDOS
cd TP1-ARM
cd src
make clean
make
cd ..
src/sim inputs/adds.x (por ejemplo)
rdump
run 1 (para correr la primera linea del adds.x)
rdump
run 1 (para correr la segunda linea del adds.x)
rdump


si quiero ver el simulador, correr ref_sim_x86
*/

int64_t add(uint32_t instruction, bool updateFlags, bool extended, bool immediate)
{
    /* 64-bit variant
    Applies when sf == 1 */
    int64_t result;

    if (extended)
    {
        // Extended Register: Xd = Xn + Xm
        uint8_t rn = (instruction >> 5) & 0x1F;  // Bits 5-9 
        uint8_t rm = (instruction >> 16) & 0x1F; // Bits 16-20
        
        // nota: cuando compilamos la instrucción ADDS Xd, Xn, Xm el bit 21 de output es 0, no 1.
        result = CURRENT_STATE.REGS[rn] + CURRENT_STATE.REGS[rm];
    } 
    else if (immediate)
    {
        // Immediate: Xd = Xn + imm12 (shifted if shift == 01)
        uint8_t rn = (instruction >> 5) & 0x1F;  // Bits 5-9
        int64_t imm12 = (instruction >> 10) & 0xFFF; // Bits 10-21
        
        uint8_t shift = (instruction >> 22) & 0x3; // shift en bits 22-23

        if (shift == 1) {
            imm12 <<= 12; // shift imm12 12 bits izq
        } else if (shift == 0) {
            // sin shift
        }
        result = CURRENT_STATE.REGS[rn] + imm12;
    }

    if (updateFlags) { // ADDS
        update_flags(result);
    }

    return result;
}

int64_t subs_cmp(uint32_t instruction, bool extended, bool immediate)
{
    /* 
    SUBS/CMP (Extended Register, Immediate)
    
    For CMP:
    Extended register: cmp X13, X14 (descripción: XZR = X13 - X14, luego updatear flags).
    XZR es el registro cero, así que se calcula el resultado y se updatean las flags, pero se tira el resultado.
    
    Immediate: cmp X13, 4 (descripción: XZR = X13 - X14, luego updatear flags)
    
    X31 debe considerarse como el registro XZR (registro que siempre tiene ceros).
    */
    
    int64_t result;
    uint8_t rd = instruction & 0x1F;  // Bits 0-4
    
    if (extended)
    {
        // Extended Register: Xd = Xn - Xm
        uint8_t rn = (instruction >> 5) & 0x1F;  // Bits 5-9
        uint8_t rm = (instruction >> 16) & 0x1F; // Bits 16-20
        
        // cuando compilamos la instrucción ADDS Xd, Xn, Xm el bit 21 de output es 0, no 1. 
        // Por favor asegúrense de que ande ADDS. Lo mismo ocurre con SUBS.
        result = CURRENT_STATE.REGS[rn] - CURRENT_STATE.REGS[rm];
    } 
    else if (immediate)
    {
        // Immediate: Xd = Xn - imm12 (shift opcional)
        uint8_t rn = (instruction >> 5) & 0x1F;  // Bits 5-9
        int64_t imm12 = (instruction >> 10) & 0xFFF; // Bits 10-21
        
        uint8_t shift = (instruction >> 22) & 0x3; // shift en bits 22-23

        if (shift == 1) {
            imm12 <<= 12; // shift imm12 12 bits izq
        } else if (shift == 0) {
            // sin shift
        }
        result = CURRENT_STATE.REGS[rn] - imm12;
    }
    
    // siempre updatear flags para SUBS/CMP
    update_flags(result);
    
    // resultado (se guarda en rn por el caller si no es XZR)
    return result;
}

void mul(uint32_t instruction)
{
    //ejemplo de uso: mul X0, X1, X2 (descripción X0 = X1 * X2) 
    int64_t result;
    // enmascaro los bits para obtener los números de registro correctos
    uint8_t rd = instruction & 0x1F;         // Bits 0-4
    uint8_t rn = (instruction >> 5) & 0x1F;  // Bits 5-9
    uint8_t rm = (instruction >> 16) & 0x1F; // Bits 16-20
    
    result = CURRENT_STATE.REGS[rn] * CURRENT_STATE.REGS[rm];
    NEXT_STATE.REGS[rd] = result;
}

// extra utils
void Adds_extended_reg(uint32_t instruction){
    int64_t result = add(instruction, true, true, false);
    NEXT_STATE.REGS[(instruction >> 0) & 0x1F] = result;
}

void Adds_imm(uint32_t instruction){
    int64_t result = add(instruction, true, false, true);
    NEXT_STATE.REGS[(instruction >> 0) & 0x1F] = result;
}

void Add_extended_reg(uint32_t instruction){
    int64_t result = add(instruction, false, true, false);
    NEXT_STATE.REGS[(instruction >> 0) & 0x1F] = result;
}

void Add_imm(uint32_t instruction){
    int64_t result = add(instruction, false, false, true);
    NEXT_STATE.REGS[(instruction >> 0) & 0x1F] = result;
}

void Subs_cmp_extended_reg(uint32_t instruction){
    int64_t result = subs_cmp(instruction, true, false);
    uint8_t rd = (instruction >> 0) & 0x1F;
    if (rd != 31) { // no escribir en XZR (X31)
        NEXT_STATE.REGS[rd] = result;
    }
}

void Subs_cmp_imm(uint32_t instruction){
    int64_t result = subs_cmp(instruction, false, true);
    uint8_t rd = (instruction >> 0) & 0x1F;
    if (rd != 31) { // no escribir en XZR (X31)
        NEXT_STATE.REGS[rd] = result;
    }
}

/*
ADDS (Extended Register, Immediate)

SUBS (Extended Register, Immediate)

CMP (Extended Register, Immediate)
 
ADD (Extended Register & Immediate)

MUL 
mul X0, X1, X2 (descripción X0 = X1 * X2) 
*/

/*
add(char * restOfInstruction, bool updateFlags, bool extended, bool immediate);
¿Qué hace?
Realiza una suma entre registros o entre un registro y un valor inmediato.
Puede actualizar los flags (N, Z, C, V) si updateFlags es true.
Cuándo se usa:
Para las instrucciones ADD (suma normal) y ADDS (suma con actualización de flags).
Admite operandos en forma de registros extendidos o inmediatos.
Ejemplos de uso:
add(instr, true, true, false); → ADDS X1, X2, X3, LSL #2
add(instr, false, false, true); → ADD X1, X2, #15


sub(char * restOfInstruction, bool updateFlags, bool extended, bool immediate);
¿Qué hace?
Realiza una resta o comparación (cuando no guarda el resultado).
Puede actualizar los flags si updateFlags es true.
Cuándo se usa:
Para las instrucciones SUB, SUBS (resta con/sin flags) y CMP (comparación).
Soporta operandos extendidos o inmediatos.
Ejemplos de uso:
sub(instr, true, true, false); → SUBS X1, X2, X3, LSR #4
sub(instr, true, false, true); → CMP X1, #42



mul(char * restOfInstruction);
¿Qué hace?
Realiza una multiplicación entre dos registros y almacena el resultado.
Cuándo se usa:
Para la instrucción MUL.
Ejemplo de uso:
mul(instr); → MUL X1, X2, X3

*/