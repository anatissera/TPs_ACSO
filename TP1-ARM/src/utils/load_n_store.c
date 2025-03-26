#include "load_n_store.h"

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

void Stur_h_b(uint32_t instruction) {
     /*
    31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
    1  1  1  1  1  0  0  0  0  0  0  imm9(9b)                   0  0  Rn(5b)    Rt(5b)
    */
    // 64-bit variant -> size = '11'

    // imm9
}

void Ldur_h_b(uint32_t instruction) {
    /*
    31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
    1  1  1  1  1  0  0  0  0  1  0  imm9(9b)                   0  0  Rn(5b)    Rt(5b)
    */
    // 64-bit variant -> size = '11'

    // imm9

    // Extraer Rt (bits 4-0) y Rn (bits 9-5)
    uint32_t RtNum = instruction & 0x1F;
    uint32_t RnNum = (instruction >> 5) & 0x1F;

    // Extraer imm9 (bits 20-12) y hacer extensión de signo a 64 bits
    int64_t imm9 = (instruction >> 12) & 0x1FF;
    if (imm9 & (1 << 8)) {  // Si el bit más alto (bit 8) es 1
        imm9 |= ~((1LL << 9) - 1); // Extensión de signo a 64 bits
    }

    // Obtener los valores de los registros
    uint64_t Rn_val = CURRENT_STATE.REGS[RnNum];  // Dirección base
    uint64_t* Rt_val = &NEXT_STATE.REGS[RtNum];  // Puntero al registro destino

    // Calcular la dirección efectiva
    uint64_t effectiveAddress = Rn_val + imm9;

    // Determinar el tipo de carga
    uint32_t type = (instruction >> 30) & 0b11;

    if (type == 0b11) {  // LDUR (64-bit)
        uint64_t lower_half = mem_read_32(effectiveAddress);
        uint64_t upper_half = ((uint64_t)mem_read_32(effectiveAddress + 4)) << 32;
        *Rt_val = lower_half | upper_half;
    } 
    else if (type == 0b01) {  // LDURH (16-bit)
        *Rt_val = mem_read_32(effectiveAddress) & 0xFFFF;
    } 
    else {  // LDURB (8-bit)
        *Rt_val = mem_read_32(effectiveAddress) & 0xFF;
    }

    // Avanzar el PC
    NEXT_STATE.PC += 4;
}


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
*/