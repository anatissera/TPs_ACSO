#include "logical_operations.h"

/*
ANDS (Shifted Register) 
ands X0, X1, X2 (descripción: X0 = X1 & X2, luego updatear flags) 
En el opcode se considerar que shift y N son siempre ceros, por lo que se chequean los bits 
<31:21> 
No se tiene que implementar el shift

EOR (Shifted Register) 
eor X0, X1, X2 (descripción: X0 = X1 ^ X2) 
En el opcode se considerar que shift y N son siempre ceros, por lo que se chequean los bits 
<31:21> 
No se tiene que implementar el shift 
 
ORR (Shifted Register) 
orr X0, X1, X2 (descripción: X0 = X1 | X2) 
En el opcode se considerar que shift y N son siempre ceros, por lo que se chequean los bits 
<31:21> 
No se tiene que implementar el shift 
*/

void Ands_shifted_reg(uint32_t instruction){
    uint32_t rd = instruction & 0x1F;         // destino (bits 0-4)
    uint32_t rn = (instruction >> 5) & 0x1F;  // fuente 1 (bits 5-9)
    uint32_t rm = (instruction >> 16) & 0x1F; // fuente 2 (bits 16-20)
    
    int64_t result = CURRENT_STATE.REGS[rn] & CURRENT_STATE.REGS[rm];
    
    NEXT_STATE.REGS[rd] = result;
    
    update_flags(result);
}
void Eor_shifted_reg(uint32_t instruction){
    uint32_t rd = instruction & 0x1F;         // destino (bits 0-4)
    uint32_t rn = (instruction >> 5) & 0x1F;  // fuente 1 (bits 5-9)
    uint32_t rm = (instruction >> 16) & 0x1F; // fuente 2 (bits 16-20)
    
    int64_t result = CURRENT_STATE.REGS[rn] ^ CURRENT_STATE.REGS[rm];
    
    NEXT_STATE.REGS[rd] = result;
    
}
void Orr_shifted_reg(uint32_t instruction){
    uint32_t rd = instruction & 0x1F;         // destino (bits 0-4)
    uint32_t rn = (instruction >> 5) & 0x1F;  // fuente 1 (bits 5-9)
    uint32_t rm = (instruction >> 16) & 0x1F; // fuente 2 (bits 16-20)
    
    int64_t result = CURRENT_STATE.REGS[rn] | CURRENT_STATE.REGS[rm];
    
    NEXT_STATE.REGS[rd] = result;
    
}

/*
logicalOp(char * restOfInstruction, int operation, bool updateFlags, bool shifted);
¿Qué hace?
Ejecuta operaciones lógicas entre registros (AND, ORR, EOR).
Puede actualizar los flags si updateFlags es true.
Parámetro operation:
0 → AND
1 → ORR
2 → EOR
Cuándo se usa:
Para AND(S), ORR(S) y EOR(S), con o sin actualización de flags.
Soporta operandos normales o registros desplazados.
Ejemplos de uso:
logicalOp(instr, 0, true, true); → ANDS X1, X2, X3, LSL #1
logicalOp(instr, 2, false, false); → EOR X1, X2, X3
*/