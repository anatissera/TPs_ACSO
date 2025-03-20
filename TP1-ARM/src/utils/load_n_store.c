#include "load_n_store.h"


// leemos la memoria
// load y store

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

Stur(Instruction decoded){
    uint32_t Rn = (decoded >> 5) & 0x1F;
    uint32_t Rt = (decoded >> 0) & 0x1F;
    uint32_t imm = (decoded >> 12) & 0x1FF;
    uint32_t address = CURRENT_STATE.reg[Rn] + imm;
    uint32_t value = CURRENT_STATE.reg[Rt];
    mem_write_32(address, value);
    NEXT_STATE.PC += 4;
}

/*

*/

/*
Las flags para leer el ADDS immediate serían
opcode: 1111 1111 0000 0000 0000 0000 0000 0000
shift: 0000 0000 1100 0000 0000 0000 0000 0000
immediate: 0000 0000 0011 1111 1111 1100 0000 0000
Rn: 0000 0000 0000 0000 0000 0011 1110 0000
Rd: 0000 0000 0000 0000 0000 0000 0001 1111
cabe aclarar que con un and &
*/

/*
Significado de los nombres en ARMv8-A:
STUR: Store Register Unsigned offset (Almacenar registro con desplazamiento sin signo).
LDUR: Load Register Unsigned offset (Cargar registro con desplazamiento sin signo).
B: Byte (8 bits).
H: Halfword (16 bits o 2 bytes).
*/

/*
A implementar: 

STORE

Qué hace cada función (en detalle):
1. Stur (Almacenar un registro completo de 64 bits)
✅ Qué hace:

Guarda el contenido completo (64 bits) de un registro en memoria.
🔨 Cómo lo hace:

Decodifica el desplazamiento (inmediato) de 9 bits con extensión de signo.
Extrae el registro base (Rn) y el registro a guardar (Rt).
Divide el valor de 64 bits en dos partes de 32 bits.
Usa mem_write_32() para escribir los dos bloques de 32 bits en direcciones consecutivas.
Incrementa el contador de programa (PC) en 4 bytes.
✍️ Alternativa de nombre: store64() o guardarRegistroCompleto().

2. Sturb (Almacenar un byte de 8 bits)
✅ Qué hace:

Guarda 1 byte (los 8 bits menos significativos del registro) en memoria.
🔨 Cómo lo hace:

Decodifica el desplazamiento de 9 bits con extensión de signo.
Extrae el registro base (Rn) y el registro a guardar (Rt).
Lee los 32 bits existentes en la dirección de memoria.
Modifica solo los 8 bits inferiores y conserva los otros 24 bits.
Usa mem_write_32() para escribir el valor combinado.
Actualiza el PC en 4 bytes.
✍️ Alternativa de nombre: storeByte() o guardarByte().

3. Sturh (Almacenar media palabra de 16 bits)
✅ Qué hace:

Guarda 16 bits (los 2 bytes menos significativos del registro) en memoria.
🔨 Cómo lo hace:

Decodifica el desplazamiento de 9 bits con extensión de signo.
Extrae el registro base (Rn) y el registro a guardar (Rt).
Lee los 32 bits existentes en la memoria.
Modifica solo los 16 bits inferiores y deja los otros bits intactos.
Usa mem_write_32() para escribir el valor combinado.
Actualiza el PC en 4 bytes.
✍️ Alternativa de nombre: storeHalfword() o guardarMedioRegistro().

LOAD
4. Ldur (Cargar un registro completo de 64 bits)
✅ Qué hace:

Carga 64 bits (8 bytes) desde memoria a un registro.
🔨 Cómo lo hace:

Decodifica el desplazamiento de 9 bits con extensión de signo.
Extrae el registro base (Rn) y el registro de destino (Rt).
Lee dos valores de 32 bits desde la memoria.
Combina ambos valores en un registro de 64 bits.
Actualiza el PC en 4 bytes.
✍️ Alternativa de nombre: load64() o cargarRegistroCompleto().

5. Ldurb (Cargar un byte de 8 bits)
✅ Qué hace:

Carga 8 bits (1 byte) desde memoria en un registro.
🔨 Cómo lo hace:

Decodifica el desplazamiento de 9 bits con extensión de signo.
Extrae el registro base (Rn) y el registro de destino (Rt).
Lee 32 bits desde memoria y se queda solo con los 8 bits inferiores.
Escribe esos 8 bits en el registro de destino.
Actualiza el PC en 4 bytes.
✍️ Alternativa de nombre: loadByte() o cargarByte().

6. Ldurh (Cargar media palabra de 16 bits)
✅ Qué hace:

Carga 16 bits (2 bytes) desde memoria a un registro.
🔨 Cómo lo hace:

Decodifica el desplazamiento de 9 bits con extensión de signo.
Extrae el registro base (Rn) y el registro de destino (Rt).
Lee 32 bits desde memoria y se queda con los 16 bits inferiores.
Escribe esos 16 bits en el registro de destino.
Actualiza el PC en 4 bytes.
✍️ Alternativa de nombre: loadHalfword() o cargarMedioRegistro().
*/