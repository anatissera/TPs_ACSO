#include "additionals.h"

/*
HLT

MOVZ 
movz X1, 10
*/

void movz(uint32_t instruction){   
    uint32_t t_register = instruction & 0b11111;
    uint32_t immediate = (instruction & (0b1111111111111111 << 5)) >> 5;
    NEXT_STATE.REGS[t_register] = immediate;
  }