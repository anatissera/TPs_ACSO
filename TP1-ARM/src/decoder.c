#include "decoder.h"

uint32_t decode(uint32_t instruction) {
    // Extraer el opcode de la instrucción según las máscaras utilizadas en execute()
    uint32_t op6  = (instruction >> 26) & 0b111111;         // 6 bits
    uint32_t op8  = (instruction >> 24) & 0xFF;            // 8 bits
    uint32_t op9  = (instruction >> 23) & 0b111111111;     // 9 bits
    uint32_t op10 = (instruction >> 22) & 0b1111111111;    // 10 bits
    uint32_t op11 = (instruction >> 21) & 0b11111111111;   // 11 bits
    uint32_t op22 = (instruction >> 10) & 0b1111111111111111111111; // 22 bits

    // Comparar con los valores esperados
    if (op6 == 0b000101) return op6; // B
    if (op9 == 0b110100101) return op9; // MOVZ
    if (op10 == 0b1101001101) return op10; // LSL/LSR IMM
    if (op22 == 0b1101011000011111000000) return op22; // BR

    // Buscar en los arrays predefinidos
    uint32_t array_opcodes_8[] = {0xb1, 0xab, 0xf1, 0xea, 0xaa, 0xca, 0x54, 0x91, 0xb4, 0xb5};
    uint32_t array_opcodes_11[] = {0b11101011001, 0b11010100010, 0b11101011001, 0b11111000000, 0b00111000000, 
                                   0b01111000000, 0b11111000010, 0b01111000010, 0b00111000010, 0b10001011000, 0b10011011000};
    
    for (int i = 0; i < sizeof(array_opcodes_8) / sizeof(array_opcodes_8[0]); i++) {
        if (op8 == array_opcodes_8[i]) return op8;
    }
    
    for (int i = 0; i < sizeof(array_opcodes_11) / sizeof(array_opcodes_11[0]); i++) {
        if (op11 == array_opcodes_11[i]) return op11;
    }

    return -1; // Opcode no reconocido
}
