#include "load_n_store.h"

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

    if (size == 0b11){ // STUR (64-bit) 
        uint32_t lower_half = Rt_val & 0xFFFFFFFF;   // bits 0-31 (derecha)
        uint32_t upper_half = (Rt_val >> 32) & 0xFFFFFFFF;  // bits 32-63 (izquierda)

        mem_write_32(effective_address, lower_half);
        mem_write_32(effective_address + 4, upper_half);

    }
    
    else {
        uint64_t aligned_address = effective_address & ~0b11; 
        uint32_t original_value = mem_read_32(aligned_address); 

        if (size == 0b01) { // STURH (16-bit)
            int half_offset = (effective_address & 0b10) >> 1; // 0 si es la parte baja, 1 si es la alta
            uint32_t mask = ~(0xFFFF << (half_offset * 16)); 
            uint32_t new_value = (Rt_val & 0xFFFF) << (half_offset * 16);
            original_value = (original_value & mask) | new_value;
        } 
        else if (size == 0b00) { // STURB (8-bit)
            int byte_offset = effective_address & 0b11;
            uint32_t mask = ~(0xFF << (byte_offset * 8)); 
            uint32_t new_value = (Rt_val & 0xFF) << (byte_offset * 8);
            original_value = (original_value & mask) | new_value;
        }

        mem_write_32(aligned_address, original_value);
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
        imm9 |= ~((1LL << 9) - 1); // Extensión de signo (1) a 64 bits
    }

    uint64_t Rn_val = CURRENT_STATE.REGS[Rn_num];  // Dirección base
    uint64_t* Rt_val = &NEXT_STATE.REGS[Rt_num];  // Puntero al registro destino: el valor que cargo debe almacenarse en el estado futuro del procesador 

    uint64_t effective_address = Rn_val + imm9;

    uint32_t size = (instruction >> 30) & 0b11;

    if (size == 0b11) {  // LDUR (64-bit)
        uint64_t lower_half = mem_read_32(effective_address);
        uint64_t upper_half = ((uint64_t)mem_read_32(effective_address + 4)) << 32;
        *Rt_val = lower_half | upper_half;
    } 

    else {  
        uint64_t aligned_address = effective_address & ~0b11; 
        uint32_t word = mem_read_32(aligned_address);  
        int byte_offset = effective_address & 0b11;  

        if (size == 0b01) { // LDURH (16-bit)
            uint16_t loaded_value = (word >> (byte_offset * 8)) & 0xFFFF; 
            *Rt_val = (uint64_t)loaded_value;
        } 
        else if (size == 0b00) { // LDURB (8-bit)
            uint8_t loaded_value = (word >> (byte_offset * 8)) & 0xFF;  
            *Rt_val = (uint64_t)loaded_value;

        }
    }
    
}