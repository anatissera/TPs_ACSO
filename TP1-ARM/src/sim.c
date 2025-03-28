// // void Subs_Cmp_extended_reg(uint32_t instruction);
// // void Subs_Cmp_imm(uint32_t instruction);

#include <stdlib.h>

#include "shell.h"

#include "decoder.h"
#include "utils/structures.h"
#include "utils/additionals.h"
#include "utils/branches.h"
#include "utils/load_n_store.h"
#include "utils/log_shift.h"
#include "utils/logical_operations.h"
#include "utils/math_operations.h"

// typedef struct {
//   uint32_t opcode;
//   void (*execute)(uint32_t);
// } Instruction;

void execute(uint32_t opcode, uint32_t instruction) {
  Opcode_Entry instructions[] = {

      {0x91, Add_imm},
      {0x8b, Add_extended_reg},
      {0xb1, Adds_imm},
      {0xab, Adds_extended_reg},
      {0xf1, Subs_cmp_imm},
      {0xb1, Subs_cmp_extended_reg},
      {0x92, mul},
      {0x6a2, HLT},
      {0xea, Ands_shifted_reg},
      {0xca, Eor_shifted_reg},
      {0xaa, Orr_shifted_reg},
      {0b000101, B},
      {0x54, B_cond},
      {0b1101011000011111000000, Br},
      {0b110100101, Movz},
      {0b1101001101, lsl_lsr_imm},
      {0b11111000000, Stur_h_b}, // Stur
      {0b00111000000, Stur_h_b}, // Sturb
      {0b01111000000, Stur_h_b}, // Sturh
      {0b11111000010, Ldur_h_b}, // Ldur
      {0b00111000010, Ldur_h_b}, // Ldurb
      {0b01111000010, Ldur_h_b}, // Ldurh
      {0b10110100, Cbz_Cbnz}, // Cbz
      {0b10110101, Cbz_Cbnz} // Cbnz
  };
  
  for (int i = 0; i < sizeof(instructions) / sizeof(instructions[0]); i++) {
      if (opcode == instructions[i].opcode) {
          instructions[i].execute(instruction);
          return;
      }
  }
}

void process_instruction() {
  uint32_t instruction = mem_read_32(CURRENT_STATE.PC);
  uint32_t opcode = decode(instruction);
  execute(opcode, instruction);
  if (CURRENT_STATE.PC == NEXT_STATE.PC) {
      NEXT_STATE.PC += 4;
  }
}


// una función para decode y otra para excecute
/* 1. Ejecutar la instrucción
2. decodificarla para saber la operación y los operandos a utilizar
3. realizar la operación
4. Actualizar el PC con la proxima instrucción a ejecutar
*/

// IMPORTANTE: asume que la memoria está en LITTLE ENDIAN