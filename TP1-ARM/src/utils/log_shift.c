#include "log_shift.h"

void lsl_lsr_imm(uint32_t instruction){   // lsl and lsr immediate
    uint32_t rd = instruction & 0x1F;
    uint32_t rn = (instruction >> 5) & 0x1F;
    uint32_t imm = (instruction >> 16) & 0x3F;
    uint32_t mask = 0xFC00;

    uint32_t mode = (instruction & mask) >> 10;

    if (mode == 0x3F) {  // 0b111111 → LSR
      NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] >> (64 - imm);
    } else {             // LSL
      NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rn] << (64 - imm);
    }
  }