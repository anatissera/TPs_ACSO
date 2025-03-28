#include "additionals.h"

/*
HLT

MOVZ 
movz X1, 10
*/

void Movz(uint32_t instruction){   
    uint32_t t_register = instruction & 0b11111; // Destination register (bits [4:0])
    uint32_t imm = (instruction >> 5) & 0xFFFF; // 16-bit immediate (bits [20:5])
    uint32_t hw = (instruction >> 21) & 0b11; // Shift amount (bits [22:21])

    uint32_t shift = hw * 16;

    NEXT_STATE.REGS[t_register] = (uint64_t)imm << shift;
}

void HLT(uint32_t instruction){ 
    RUN_BIT = 0; 
}

