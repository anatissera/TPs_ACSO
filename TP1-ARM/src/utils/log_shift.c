#include "log_shift.h"

/*
1. Descomponer una instrucción ARM en binario para extraer:
- El tipo de desplazamiento (N).
- La cantidad de bits a desplazar (immr).
- Los registros fuente (Rn) y destino (Rd).
2. Aplicar un desplazamiento lógico:
- Izquierda (LSL) si imms != 31.
- Derecha (LSR) si imms == 31.
3. Guarda4 el resultado en el registro de destino (Rd).
4. Avanza el PC al siguiente ciclo de instrucción.     NEXT_STATE.PC  += 4;
*/

