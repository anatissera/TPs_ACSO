#include "log_shift.h"

void lsl_lsr_imm(uint32_t instruction){   // lsl and lsr immediate
    uint32_t rd = instruction & 0b11111;
    uint32_t rn = (instruction & (0b11111 << 5)) >> 5;
    uint32_t imm = (instruction & (0b111111 << 16)) >> 16;
  
    uint32_t mask = 0b111111 << 10;
  
    if (((instruction & mask) >> 10) == 0b111111){ // LSR
      NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] >> (64 - imm);
    } else {                                       // LSL
      NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] << (64 - imm);
    }
  }