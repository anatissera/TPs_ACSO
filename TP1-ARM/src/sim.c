#include <stdio.h>
#include <assert.h>
#include <string.h>

void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. 
     * */

}
// una función para decode y otra para excecute
/* 1. Ejecutar la instrucción
2. decodificarla para saber la operación y los operandos a utilizar
3. realizar la operación
4. Actualizar el PC con la proxima instrucción a ejecutar
*/

// IMPORTANTE: utiliza little endian

/*
Las flags para leer el ADDS serían
opcode: 1111 1111 0000 0000 0000 0000 0000 0000
shift: 0000 0000 1100 0000 0000 0000 0000 0000
immediate: 0000 0000 0011 1111 1111 1100 0000 0000
Rn: 0000 0000 0000 0000 0000 0011 1110 0000
Rd: 0000 0000 0000 0000 0000 0000 0001 1111
cabe aclarar que con un and &
*/