#include "structures.h"
#pragma once

#include "../shell.h" 
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/*

STUR -  
stur X1, [X2, #0x10]

STURB
sturb X1, [X2, #0x10]

STURH 
sturh W1, [X2, #0x10] 

LDUR 
ldur X1, [X2, #0x10] 
 
LDURH 
ldurh  W1,  [X2,  #0x10] 

LDURB 
ldurb  W1,  [X2,  #0x10] 

*/
void Stur_h_b(uint32_t instruction);
void Ldur_h_b(uint32_t instruction);