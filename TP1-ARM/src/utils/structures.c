#include "structures.h"

void update_flags(int64_t result) {
    if (result == 0) {
        NEXT_STATE.FLAG_Z = 1;
        NEXT_STATE.FLAG_N = 0;
    } else if (result < 0) {
        NEXT_STATE.FLAG_Z = 0;
        NEXT_STATE.FLAG_N = 1;
    } else {
        NEXT_STATE.FLAG_Z = 0;
        NEXT_STATE.FLAG_N = 0;
    }
}
