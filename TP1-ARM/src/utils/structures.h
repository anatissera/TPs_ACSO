#ifndef PROCESS_INSTRUCTION_H
#define PROCESS_INSTRUCTION_H

#include <stdint.h>
#include "shell.h"

typedef struct {
    uint32_t opcode_mask;
    uint32_t opcode_value;
    void (*function)(uint32_t);
  } Opcode_Entry;

void execute(uint32_t instruction);
void process_instruction(void);

#endif