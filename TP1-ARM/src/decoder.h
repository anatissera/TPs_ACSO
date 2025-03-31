#include <stdint.h>
#include "shell.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

uint32_t decode(uint32_t instruction);

bool check_subs_ext(uint32_t instruction);
uint32_t match_opcode(uint32_t value, const uint32_t* set, int len);
uint8_t decode_branch_condition(uint32_t instruction);
