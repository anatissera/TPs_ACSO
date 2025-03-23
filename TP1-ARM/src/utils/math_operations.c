#include "math_operations.h"

add(char * restOfInstruction, bool updateFlags, bool extended, bool immediate)
{
    int64_t result;
    if (extended)
    {
        result = CURRENT_STATE.REGS[restOfInstruction[0]] + CURRENT_STATE.REGS[restOfInstruction[1]];
    } else if (immediate) {
        result = CURRENT_STATE.REGS[restOfInstruction[0]] + restOfInstruction[1];
    }
    if (updateFlags) { //if true, instruction is ADDS
        NEXT_STATE.FLAG_N = (result < 0);
        NEXT_STATE.FLAG_Z = (result == 0);
    }
    return result;
}

sub(char * restOfInstruction, bool updateFlags, bool extended, bool immediate)
{
    int64_t result;
    if (extended)
    {
        result = CURRENT_STATE.REGS[restOfInstruction[0]] - CURRENT_STATE.REGS[restOfInstruction[1]];
    } else if (immediate) {
        result = CURRENT_STATE.REGS[restOfInstruction[0]] - restOfInstruction[1];
    }
    if (updateFlags) { //if true, instruction is SUBS
        NEXT_STATE.FLAG_N = (result < 0);
        NEXT_STATE.FLAG_Z = (result == 0);
    }
    return result;
}

mul(char * restOfInstruction)
{
    //ejemplo de uso: mul X0, X1, X2 (descripción X0 = X1 * X2) 
    int64_t result;
    result = CURRENT_STATE.REGS[restOfInstruction[0]] * CURRENT_STATE.REGS[restOfInstruction[1]];
    CURRENT_STATE.REGS[restOfInstruction[2]] = result;
    return result;
}

cmp(char * restOfInstruction, bool extended, bool immediate)
{
    int64_t result;
    
}


/*
ADDS (Extended Register, Immediate)

SUBS (Extended Register, Immediate)

CMP (Extended Register, Immediate)
 
ADD (Extended Register & Immediate)

MUL 
mul X0, X1, X2 (descripción X0 = X1 * X2) 
*/


// void updateFlags(int64_t result) {
//     NEXT_STATE.FLAG_N = (result < 0);
//     NEXT_STATE.FLAG_Z = (result == 0);
// }

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