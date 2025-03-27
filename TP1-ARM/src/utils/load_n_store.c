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
    size  1  1  1  0  0  0  0  0  0  imm9(9b)                   0  0  Rn(5b)    Rt(5b)
    */
    // STUR size = '11' -> 64-bit variant

    // STURH size = '01' -> 16-bit variant

    // STURB size = '00' -> 8-bit variant

    // Rt [4:0] y Rn [9:5]
    uint32_t Rt_num = instruction & 0x1F;
    uint32_t Rn_num = (instruction >> 5) & 0x1F;

    // imm9 [20:12] y extensión de signo a 64 bits
    int64_t imm9 = (instruction >> 12) & 0x1FF;

    if (imm9 & (1 << 8)){
        imm9 |= ~((1LL << 9)-1);
    }

    uint32_t size = (instruction >> 30) &0b11;

    uint64_t Rn_val = CURRENT_STATE.REGS[Rn_num];  // Dirección base
    uint64_t Rt_val = CURRENT_STATE.REGS[Rt_num];  // Valor a almacenar (valor actual del registro).

    uint64_t effective_address = Rn_val + imm9;

    if (size == 0b11){ // STUR (64-bit) -> 0xFFFFFFFF de a 32
        uint32_t lower_half = Rt_val & 0xFFFFFFFF;   // bits 0-31 (derecha)
        uint32_t upper_half = (Rt_val >> 32) & 0xFFFFFFFF;  // bits 32-63 (izquierda)

        mem_write_32(effective_address, lower_half);
        mem_write_32(effective_address + 4, upper_half);
    }
    else if (size == 0b01){ // STURH (16-bit)
        mem_write_32(effective_address, (uint32_t)(Rt_val & 0xFFFF)); 
    }
    else if (size == 0b00){ // STURB (8-bit)
        mem_write_32(effective_address, (uint32_t)(Rt_val & 0xFF)); 
    }

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

    // Rt [4:0] y Rn [9:5]
    uint32_t Rt_num = instruction & 0x1F; // 0b11111
    uint32_t Rn_num = (instruction >> 5) & 0x1F; // muevo 5 por Rt

    // imm9 [20:12] y extensión de signo a 64 bits
    int64_t imm9 = (instruction >> 12) & 0x1FF; // 5 (Rt) + 5 (Rn) + 2 (addressing mode field) = 12 -> muevo 12 al final y me quedo con los últimos 9 -> 0x1FF = 0b111111111
    
    if (imm9 & (1 << 8)) {  // Si el bit más alto (bit 8) es 1
        imm9 |= ~((1LL << 9) - 1); // Extensión de signo (1) a 64 bits (1LL (long long de valor 1)) 
        // el procesador usa registros de 64 bits, incluso si la operación solo carga 8 o 16 bits
        // máscara = 1111111111111111111111111111111111111111111111111000000000
    }

    uint64_t Rn_val = CURRENT_STATE.REGS[Rn_num];  // Dirección base
    uint64_t* Rt_val = &NEXT_STATE.REGS[Rt_num];  // Puntero al registro destino: el valor que cargado debe almacenarse en el estado futuro del procesador 

    uint64_t effective_address = Rn_val + imm9;

    uint32_t size = (instruction >> 30) & 0b11;

    if (size == 0b11) {  // LDUR (64-bit)
        // little endian
        uint64_t lower_half = mem_read_32(effective_address);
        uint64_t upper_half = ((uint64_t)mem_read_32(effective_address + 4)) << 32;
        *Rt_val = lower_half | upper_half;
    } 
    else if (size == 0b01) {  // LDURH (16-bit)
        *Rt_val = mem_read_32(effective_address) & 0xFFFF;
    } 
    else if (size == 0b00){  // LDURB (8-bit)
        *Rt_val = mem_read_32(effective_address) & 0xFF;
    }
    // else{
    //     printf("Tipo de carga no reconocido: 0x%X\n", size);
    // }

    // NEXT_STATE.PC += 4;
}