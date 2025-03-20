#include "logical_operations.h"

/*
ANDS (Shifted Register) 

EOR (Shifted Register)

ORR (Shifted Register)
*/

/*
logicalOp(char * restOfInstruction, int operation, bool updateFlags, bool shifted);
¿Qué hace?
Ejecuta operaciones lógicas entre registros (AND, ORR, EOR).
Puede actualizar los flags si updateFlags es true.
Parámetro operation:
0 → AND
1 → ORR
2 → EOR
Cuándo se usa:
Para AND(S), ORR(S) y EOR(S), con o sin actualización de flags.
Soporta operandos normales o registros desplazados.
Ejemplos de uso:
logicalOp(instr, 0, true, true); → ANDS X1, X2, X3, LSL #1
logicalOp(instr, 2, false, false); → EOR X1, X2, X3
*/