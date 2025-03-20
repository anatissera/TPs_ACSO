#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"


#include "utils/additionals.h"
#include "utils/branches.h"
#include "utils/load_n_store.h"
#include "utils/log_shift.h"
#include "utils/logical_operations.h"
#include "utils/math_operations.h"

#ifndef PROCESS_INSTRUCTION_H
#define PROCESS_INSTRUCTION_H

#include <stdint.h>

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

// Prototipos de funciones de memoria (asumiendo que están definidas en otro archivo)
uint32_t mem_read_32(uint32_t address);
void mem_write_32(uint32_t address, uint32_t value);

#endif // PROCESS_INSTRUCTION_H
