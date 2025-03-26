#include <stdint.h>
#include "shell.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>


bool is_subs_ext(uint32_t instruction);

uint32_t is_opcode_length_8(uint32_t instruction, uint32_t* array_opcodes);

uint32_t is_opcode_length_11(uint32_t instruction, uint32_t* array_opcodes);

uint32_t decode(uint32_t instruction); 

uint8_t decode_b_cond(uint32_t instruction);


uint32_t decode(uint32_t instruction);