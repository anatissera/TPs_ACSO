#include "decoder.h"

bool match_opcode(uint32_t instruction, uint32_t mask, uint32_t shift, uint32_t expected) {
    return ((instruction >> shift) & mask) == expected;
}

uint32_t decode(uint32_t instruction) {
    if (match_opcode(instruction, 0b111111, 26, 0b000101)) return 0b000101; // B
    
    uint32_t op8[] = {0xb1, 0xab, 0xf1, 0xea, 0xaa, 0xca, 0x54, 0x91, 0xb4, 0xb5};
    uint32_t extracted_op8 = (instruction >> 24) & 0xFF;
    for (int i = 0; i < 10; i++) {
        if (extracted_op8 == op8[i]) return op8[i];
    }
    
    if (match_opcode(instruction, 0b111111111, 23, 0b110100101)) return 0b110100101; // MOVZ
    if (match_opcode(instruction, 0b1111111111, 22, 0b1101001101)) return 0b1101001101; // LSL/LSR
    
    uint32_t op11[] = {0b11101011001, 0b11010100010, 0b11101011001, 0b11111000000, 0b00111000000,
                        0b01111000000, 0b11111000010, 0b01111000010, 0b00111000010, 0b10001011000, 0b10011011000};
    uint32_t extracted_op11 = (instruction >> 21) & 0b11111111111;
    for (int i = 0; i < 11; i++) {
        if (extracted_op11 == op11[i]) return op11[i];
    }
    
    if (match_opcode(instruction, 0b1111111111111111111111, 10, 0b1101011000011111000000)) return 0b1101011000011111000000; // BR
    
    return -1;
}
