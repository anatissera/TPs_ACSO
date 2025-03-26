#include "additionals.h"


/*
HLT

MOVZ 
movz X1, 10
*/

void Movz(uint32_t instruction){   
    // Precise bit extraction for MOVZ
    uint32_t t_register = instruction & 0b11111; // Destination register (bits [4:0])
    uint32_t immediate = (instruction >> 5) & 0xFFFF; // 16-bit immediate (bits [20:5])
    uint32_t hw = (instruction >> 21) & 0b11; // Shift amount (bits [22:21])

    // Calculate shift based on hw value
    uint32_t shift = hw * 16;

    // Set register value
    NEXT_STATE.REGS[t_register] = (uint64_t)immediate << shift;
    
    printf("MOVZ: X%d = 0x%X (immediate 0x%X, shift %d)\n", 
           t_register, (uint32_t)NEXT_STATE.REGS[t_register], immediate, shift);
}