#include "log_shift.h"

/*
LSL (Immediate) 
lsl X4, X3, 4

LSR (Immediate) 
lsr X4, X3, 4
*/

/*
1. Descomponer una instrucción ARM en binario para extraer:
- El tipo de desplazamiento (N).
- La cantidad de bits a desplazar (immr).
- Los registros fuente (Rn) y destino (Rd).
2. Aplicar un desplazamiento lógico:
- Izquierda (LSL) si imms != 31.
- Derecha (LSR) si imms == 31.
3. Guarda el resultado en el registro de destino (Rd).
4. Avanza el PC al siguiente ciclo de instrucción.     NEXT_STATE.PC  += 4;
*/

void lsl_lsr_imm(uint32_t instruction){
    uint8_t rd = instruction & 0x1F;         // destino (bits 0-4)
    uint8_t rn = (instruction >> 5) & 0x1F;  // fuente (bits 5-9)
    
    uint8_t N = (instruction >> 22) & 0x1;   // N bit
    uint8_t immr = (instruction >> 16) & 0x3F; // immr (bits 16-21)
    uint8_t imms = (instruction >> 10) & 0x3F; // imms (bits 10-15)
    
    uint64_t value = CURRENT_STATE.REGS[rn];
    uint64_t result;
    
    // LSL o LSR
    if (imms != 31) {
        // LSL - desplazamiento lógico izquierdo
        uint8_t shift_amount = immr;
        result = value << shift_amount;
    } else {
        // LSR - desplazamiento lógico derecho
        uint8_t shift_amount = immr;
        result = value >> shift_amount;
    }
    
    // guardo resultado en el registro destino
    NEXT_STATE.REGS[rd] = result;
}