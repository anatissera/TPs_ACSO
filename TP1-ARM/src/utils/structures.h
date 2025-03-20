#ifndef PROCESS_INSTRUCTION_H
#define PROCESS_INSTRUCTION_H

#include <stdint.h>
#include "shell.h"

// Definición de la estructura de instrucción
typedef struct {
    uint32_t opcode;
    uint32_t operand1;
    uint32_t operand2;
} Instruction;

// Declaraciones de funciones
Instruction decode_instruction(uint32_t instruction);
Instruction execute_instruction(Instruction decoded);
void process_instruction(void);

// Declaraciones externas (si CURRENT_STATE y NEXT_STATE están definidos en otro archivo)
extern struct {
    uint32_t pc;
} CURRENT_STATE, NEXT_STATE;


#endif
