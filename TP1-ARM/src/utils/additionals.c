#include "additionals.h"

void Movz(uint32_t instruction){   
    uint32_t t_register = instruction & 0b11111; // Registro destino (bits [4:0])
    uint32_t imm16 = (instruction >> 5) & 0xFFFF; // (bits [20:5])
    uint32_t hw = (instruction >> 21) & 0b11; // Shift amount (bits [22:21])

    uint32_t shift = hw * 16;

    NEXT_STATE.REGS[t_register] = (uint64_t)imm16 << shift;
}

void HLT(uint32_t instruction){ 
    RUN_BIT = 0; 
}

