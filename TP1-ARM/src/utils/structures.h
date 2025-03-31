#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "shell.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>


typedef struct {
    uint32_t opcode;
    void (*execute)(uint32_t);
} Opcode_Entry;

void update_flags(int64_t result);

#endif // STRUCTURES_H
