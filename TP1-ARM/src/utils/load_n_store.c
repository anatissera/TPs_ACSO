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
    LDUR
    31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
    size  1  1  1  0  0  0  0  1  0  imm9(9b)                   0  0  Rn(5b)    Rt(5b)
                       
    bits 11 y 10 -> modo de direccionamiento '00' -> inmediato no escalado (Unscaled Immediate Addressing Mode)
    */

    // LDUR size = '11' -> 64-bit variant

    // LDURH size = '01' -> 16-bit variant

    // LDURB size = '00' -> 8-bit variant

    // Extraer Rt [4:0] y Rn [9:5]
    uint32_t Rt_num = instruction & 0x1F; // 11111
    uint32_t Rn_num = (instruction >> 5) & 0x1F; // muevo 5 por Rt

    // Extraer imm9 [20:12] y hacer extensión de signo a 64 bits
    int64_t imm9 = (instruction >> 12) & 0x1FF; // 5 (Rt) + 5 (Rn) + 2 (addressing mode field) = 12 -> muevo 12 y me quedo con 9 -> 0x1FF = 111111111
    
    if (imm9 & (1 << 8)) {  // Si el bit más alto (bit 8) es 1
        imm9 |= ~((1LL << 9) - 1); // Extensión de signo 1 a 64 bits (1LL (long long de valor 1)) 
        // el procesador usa registros de 64 bits, incluso si la operación solo carga 8 o 16 bits
        // máscara = 1111111111111111111111111111111111111111111111111000000000
    }

    uint64_t Rn_val = CURRENT_STATE.REGS[Rn_num];  // Dirección base
    uint64_t* Rt_val = &NEXT_STATE.REGS[Rt_num];  // Puntero al registro destino

    uint64_t effective_address = Rn_val + imm9;

    uint32_t type = (instruction >> 30) & 0b11;

    if (type == 0b11) {  // LDUR (64-bit)
        // little endian
        uint64_t lower_half = mem_read_32(effective_address);
        uint64_t upper_half = ((uint64_t)mem_read_32(effective_address + 4)) << 32;
        *Rt_val = lower_half | upper_half;
    } 
    else if (type == 0b01) {  // LDURH (16-bit)
        *Rt_val = mem_read_32(effective_address) & 0xFFFF;
    } 
    else if (type == 0b00){  // LDURB (8-bit)
        *Rt_val = mem_read_32(effective_address) & 0xFF;
    }
    else{
        printf("Tipo de carga no reconocido: 0x%X\n", type);
    }

    NEXT_STATE.PC += 4;
}



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