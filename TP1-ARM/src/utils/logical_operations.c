#include "logical_operations.h"

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