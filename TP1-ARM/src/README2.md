# Instructions y disclaimer

## Instructions

### SHIFT
LSL (Immediate) 
lsl X4, X3, 4

LSR (Immediate) 
lsr X4, X3, 4


### LOGICAL OPERATIONS
ANDS (Shifted Register) 

EOR (Shifted Register)

ORR (Shifted Register)


### BRANCHES
B 
b target

BR 
br X1

BEQ (B.Cond) 
cmp X1,X2 
beq target

BNE (B.Cond) 
cmp X1,X2 
bne target

BGT (B.Cond) 
cmp X1,X2 
bgt target

BLT (B.Cond) 
cmp X1,X2 
blt target

BGE (B.Cond) 
cmp X1,X2 
bge target

BLE (B.Cond) 
cmp X1,X2 
ble target

CBZ 
cbz X3, label 
 
CBNZ 
cbnz X3, label


### LOAD AND STORE

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
 
### MATH
ADDS (Extended Register, Immediate)

SUBS (Extended Register, Immediate)

CMP (Extended Register, Immediate)
 
ADD (Extended Register & Immediate)

MUL 
mul X0, X1, X2 (descripción X0 = X1 * X2) 
 

### ADDITIONALS
HLT

MOVZ 
movz X1, 10

## Disclaimer
Correr el makefile de la siguiente manera:

          cd src
          make clean
          make
          make run // para convertir a formato unix si corren con WSL en Windows

Luego para automatizar los tests:

          cd ..
          bash compare_sims.sh