#include "log_shift.h"

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