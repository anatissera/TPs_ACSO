#ifndef PROCESS_INSTRUCTION_H
#define PROCESS_INSTRUCTION_H

#include <stdint.h>
#include "shell.h"


typedef struct {
    uint32_t opcode;
    void (*execute)(uint32_t);
  } Opcode_Entry;

// void execute(uint32_t instruction);
// void process_instruction(void);


uint32_t global_nzcv = 0;  // Registro de estado simulado
#define NZCV global_nzcv   // Definimos NZCV como alias de la variable

// Definiciones de los flags
#define FLAG_N  ((NZCV >> 31) & 1)  // Bit 31: Flag N (1 si negativo)
#define FLAG_Z  ((NZCV >> 30) & 1)  // Bit 30: Flag Z (1 si cero)

void update_flags(int64_t result);


#endif
