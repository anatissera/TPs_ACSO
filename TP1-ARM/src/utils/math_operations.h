#include "structures.h"

void Adds_extended_reg(uint32_t instruction);
void Adds_imm(uint32_t instruction);
void Add_extended_reg(uint32_t instruction);
void Add_imm(uint32_t instruction);

int64_t add(uint32_t instruction, bool updateFlags, bool extended, bool immediate);

void Subs_cmp_extended_reg(uint32_t instruction);
void Subs_cmp_imm(uint32_t instruction);

void mul(uint32_t instruction);

/*
La operación de CMP es prácticamente idéntica a SUBS, 
que también realiza una resta y actualiza las banderas. 
La diferencia es que CMP no guarda el resultado de la resta en un registro, mientras que SUBS sí lo hace.
Por eso las metí juntas*/
