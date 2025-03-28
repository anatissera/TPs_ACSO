#include "structures.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

/*
ADDS (Extended Register, Immediate)

SUBS (Extended Register, Immediate)

CMP (Extended Register, Immediate)
 
ADD (Extended Register & Immediate)

MUL 
mul X0, X1, X2 (descripción X0 = X1 * X2) 
*/

void Adds_extended_reg(uint32_t instruction);
void Adds_imm(uint32_t instruction);
void Add_extended_reg(uint32_t instruction);
void Add_imm(uint32_t instruction);

int64_t add(uint32_t instruction, bool updateFlags, bool extended, bool immediate);

void Subs_cmp_extended_reg(uint32_t instruction);
void Subs_cmp_imm(uint32_t instruction);

int64_t subs_cmp(uint32_t instruction, bool extended, bool immediate);

void mul(uint32_t instruction);

/*CMPrealiza una comparación entre dos registros. 
Internamente, esta operación es una resta, pero no almacena el resultado; 
solo actualiza los flags (banderas del procesador) según el resultado de la operación.

La operación de CMP es prácticamente idéntica a SUBS, 
que también realiza una resta y actualiza las banderas. 
La diferencia es que CMP no guarda el resultado de la resta en un registro, mientras que SUBS sí lo hace.
Por eso las metí juntas*/
