#include <stdlib.h>

#include "shell.h"

#include "utils/additionals.h"
#include "utils/branches.h"
#include "utils/load_n_store.h"
#include "utils/log_shift.h"
#include "utils/logical_operations.h"
#include "utils/math_operations.h"



// void Subs_Cmp_extended_reg(uint32_t instruction);
// void Subs_Cmp_imm(uint32_t instruction);

void execute(uint32_t instruction) {
  Opcode_Entry opcodes[] = {
      {0xFFC00000, 0b10101011000, Adds_extended_reg},  
      {0xFFFFFC00, 0b10110001, Adds_imm},             
      {0xFFC00000, 0b11101011000, Subs_Cmp_extended_reg}, 
      {0xFFFFFC00, 0b11110001, Subs_Cmp_imm},
      {0xFFFFFC00, 0b11010100, HLT},
      {0xFFFFFC00, 0b110100101, Movz},
      {0xFFFFFC00, 0b11101010, Ands_shifted_reg},
      {0xFFFFFC00, 0b11001010, Eor_shifted_reg},
      {0xFFFFFC00, 0b10101010, Orr_shifted_reg},
      {0xFC000000, 0b000101, B},                   
      {0xFFFFFFFF, 0b1101011000111111000000, Br},  
      {0xFFFFFC00, 0b01010100, B_cond},
      {0xFFFFFC00, 0b110100110, lsl_lsr_imm},
      {0xFFC00000, 0b11111000000, Stur_h_b},
      {0xFFC00000, 0b00111000000, Stur_h_b},
      {0xFFC00000, 0b01111000000, Stur_h_b},
      {0xFFC00000, 0b11111000010, Ldur_h_b},
      {0xFFC00000, 0b00111000010, Ldur_h_b},
      {0xFFC00000, 0b01111000010, Ldur_h_b},

      {0xFFC00000, 0b10001011000, Add_extended_reg},
      {0xFFFFFC00, 0b10010001, Add_immediate},
      {0xFFC00000, 0b10011011000, Mul},
      {0xFFFFFC00, 0b10110100, Cbz},
      {0xFFFFFC00, 0b10110101, Cbnz}
  };

  int num_opcodes = sizeof(opcodes) / sizeof(opcodes[0]);

  for (int i = 0; i < num_opcodes; i++) {
      if ((instruction & opcodes[i].opcode_mask) == opcodes[i].opcode_value) {
          opcodes[i].function(instruction);
          return;
      }
  }
  
  printf("Opcode no reconocido: 0x%X\n", instruction);
}

void process_instruction() {
  uint32_t instruction = mem_read_32(CURRENT_STATE.PC); 
  uint32_t opcode = decode(instruction);

  execute(instruction);

  if (CURRENT_STATE.PC == NEXT_STATE.PC) {
      NEXT_STATE.PC += 4;
  }
}

  /* execute one instruction here. You should use CURRENT_STATE and modify
  * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
  * access memory. 
  * 
  */


// una función para decode y otra para excecute
/* 1. Ejecutar la instrucción
2. decodificarla para saber la operación y los operandos a utilizar
3. realizar la operación
4. Actualizar el PC con la proxima instrucción a ejecutar
*/

// IMPORTANTE: asume que la memoria está en LITTLE ENDIAN