#include <stdlib.h>
#include "shell.h"


#include "utils/additionals.h"
#include "utils/branches.h"
#include "utils/load_n_store.h"
#include "utils/log_shift.h"
#include "utils/logical_operations.h"
#include "utils/math_operations.h"

void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. 
     * 
     */

    uint32_t instruction = mem_read_32(CURRENT_STATE.PC);
    Instruction decoded = decode_instruction(instruction);
    Instruction executed = execute_instruction(decoded);
    mem_write_32(NEXT_STATE.PC, executed.result);
}
// una función para decode y otra para excecute
/* 1. Ejecutar la instrucción
2. decodificarla para saber la operación y los operandos a utilizar
3. realizar la operación
4. Actualizar el PC con la proxima instrucción a ejecutar
*/

// IMPORTANTE: asume que la memoria está en LITTLE ENDIAN


