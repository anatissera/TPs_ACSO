#include "math_operations.h"

int64_t add(uint32_t instruction, bool updateFlags, bool extended, bool immediate)
{
    int64_t result;

    if (extended)
    {
        // Extended Register: Xd = Xn + Xm
        uint8_t rn = (instruction >> 5) & 0x1F;  // Bits 5-9 
        uint8_t rm = (instruction >> 16) & 0x1F; // Bits 16-20
        
        // nota: cuando compilamos la instrucción ADDS Xd, Xn, Xm el bit 21 de output es 0, no 1.
        result = CURRENT_STATE.REGS[rn] + CURRENT_STATE.REGS[rm];
    } 
    else if (immediate)
    {
        // Immediate: Xd = Xn + imm12 (shifted if shift == 01)
        uint8_t rn = (instruction >> 5) & 0x1F;  // Bits 5-9
        int64_t imm12 = (instruction >> 10) & 0xFFF; // Bits 10-21
        
        uint8_t shift = (instruction >> 22) & 0x3; // shift en bits 22-23

        if (shift == 1) {
            imm12 <<= 12; // shift imm12 12 bits izq
        } else if (shift == 0) {
            // sin shift
        }
        result = CURRENT_STATE.REGS[rn] + imm12;
    }

    if (updateFlags) { // ADDS
        update_flags(result);
    }

    return result;
}

void mul(uint32_t instruction){
    int64_t result;
    // enmascaro los bits para obtener los números de registro correctos
    uint8_t rd = instruction & 0x1F;         // Bits 0-4
    uint8_t rn = (instruction >> 5) & 0x1F;  // Bits 5-9
    uint8_t rm = (instruction >> 16) & 0x1F; // Bits 16-20
    
    result = CURRENT_STATE.REGS[rn] * CURRENT_STATE.REGS[rm];
    NEXT_STATE.REGS[rd] = result;
}

// extra utils
void Adds_extended_reg(uint32_t instruction){
    int64_t result = add(instruction, true, true, false);
    NEXT_STATE.REGS[(instruction >> 0) & 0x1F] = result;
}

void Adds_imm(uint32_t instruction){
    int64_t result = add(instruction, true, false, true);
    NEXT_STATE.REGS[(instruction >> 0) & 0x1F] = result;
}

void Add_extended_reg(uint32_t instruction){
    int64_t result = add(instruction, false, true, false);
    NEXT_STATE.REGS[(instruction >> 0) & 0x1F] = result;
}

void Add_imm(uint32_t instruction){
    int64_t result = add(instruction, false, false, true);
    NEXT_STATE.REGS[(instruction >> 0) & 0x1F] = result;
}

void Subs_cmp_imm(uint32_t instruction) {
    uint32_t imm12 = (instruction >> 10) & 0xFFF;
    uint8_t shift = (instruction >> 22) & 0x3;
    uint8_t rd = instruction & 0x1F;
    uint8_t rn = (instruction >> 5) & 0x1F;
  
    if (shift == 1) imm12 <<= 12;
  
    int64_t result = (int64_t)CURRENT_STATE.REGS[rn] - imm12;
  
    update_flags(result);
  
    if (rd != 31) {
      NEXT_STATE.REGS[rd] = result;
    }
  }
  
  void Subs_cmp_extended_reg(uint32_t instruction) {
    uint8_t rd = instruction & 0x1F;
    uint8_t rn = (instruction >> 5) & 0x1F;
    uint8_t rm = (instruction >> 16) & 0x1F;
  
    int64_t result = (int64_t)CURRENT_STATE.REGS[rn] - CURRENT_STATE.REGS[rm];
  
    update_flags(result);
  
    if (rd != 31) {
      NEXT_STATE.REGS[rd] = result;
    }
  }
