#include "structures.h"
#pragma once

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

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
void Stur(char * restOfInstruction);
void Sturb(char * restOfInstruction);
void Sturh(char * restOfInstruction);
void Ldur(char * restOfInstruction);
void Ldurb(char * restOfInstruction);
void Ldurh(char * restOfInstruction);