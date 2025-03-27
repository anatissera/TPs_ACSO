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
        uint8_t rn = (instruction >> 5) & 0x1F;  // Bits 5-9 contain Rn
        uint8_t rm = (instruction >> 16) & 0x1F; // Bits 16-20 contain Rm
        result = CURRENT_STATE.REGS[rn] + CURRENT_STATE.REGS[rm];
    } 
    else if (immediate)
    {
        // Immediate: Xd = Xn + imm12 (shifted if shift == 01)
        uint8_t rn = (instruction >> 5) & 0x1F;  // Bits 5-9 contain Rn
        int64_t imm12 = (instruction >> 10) & 0xFFF; // Bits 10-21 contain imm12
        
        // Extract shift from bits 22 and 23 of the instruction
        uint8_t shift = (instruction >> 22) & 0x3; // Extract bits 22-23

        if (shift == 1) {
            imm12 <<= 12; // Shift imm12 12 bits to the left
        } else if (shift == 0) {
            // No shift
        }
        result = CURRENT_STATE.REGS[rn] + imm12;
    }

    if (updateFlags) { // if true, instruction is ADDS
        update_flags(result);
    }

    return result;
}

int64_t sub(char * restOfInstruction, bool updateFlags, bool extended, bool immediate)
{
    int64_t result;
    if (extended)
    {
        result = CURRENT_STATE.REGS[restOfInstruction[0]] - CURRENT_STATE.REGS[restOfInstruction[1]];
    } else if (immediate) {
        result = CURRENT_STATE.REGS[restOfInstruction[0]] - restOfInstruction[1];
    }
    if (updateFlags) { //if true, instruction is SUBS
        update_flags(result);
    }
    return result;
}

int64_t mul(char * restOfInstruction)
{
    //ejemplo de uso: mul X0, X1, X2 (descripción X0 = X1 * X2) 
    int64_t result;
    result = CURRENT_STATE.REGS[restOfInstruction[0]] * CURRENT_STATE.REGS[restOfInstruction[1]];
    CURRENT_STATE.REGS[restOfInstruction[2]] = result;
    return result;
}

int64_t cmp(char * restOfInstruction, bool extended, bool immediate)
{
    int64_t result;
    
}

// The functions are declared to return int64_t and take char* but implemented to return void and take uint32_t.

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