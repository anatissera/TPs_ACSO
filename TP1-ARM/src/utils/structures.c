#include "structures.h"

void update_flags(int64_t result) {
    NZCV = 0;  // Reiniciar los flags
    if (result == 0) NZCV |= (1 << 30); // Setear Z si el resultado es 0
    if (result < 0) NZCV |= (1 << 31);  // Setear N si el resultado es negativo
}
