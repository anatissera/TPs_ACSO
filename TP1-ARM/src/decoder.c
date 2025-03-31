#include "decoder.h"

static const uint32_t OPCODE_6BIT = 0b000101;  // B

static const uint32_t OPCODES_8BIT[] = {
    0xb1, // adds_imm
    0xab, // adds_ext
    0xb4, // add_imm
    0xf1, // subs_imm
    0xea, // cmp_imm
    0xaa, // ands_shift
    0xca, // eor_shift
    0x54, // orr_shift
    0x91, // b.cond
    0xb5  // cbz, cbnz
}; 

static const uint32_t OPCODE_9BIT = 0b110100101;       // movz
static const uint32_t OPCODE_10BIT = 0b1101001101;     // ls

static const uint32_t OPCODES_11BIT[] = {
    0b11010100010, // hlt
    0b10011011000, // mul
    0b11101011001, // subs_ext
    0b11101011001, // cmp_ext
    0b11111000000, // stur
    0b01111000000, // sturh
    0b00111000000, // sturb
    0b11111000010, // ldur
    0b01111000010, // ldurh
    0b00111000010, // ldurb
    0b10001011000  // add_ext
};

static const uint32_t OPCODE_22BIT = 0b1101011000011111000000; // BR

static const uint8_t B_COND_CODES[] = {
    0b0,    // B.eq
    0b1,    // B.ne
    0b1100, // B.gt
    0b1011, // B.lt
    0b1010, // B.ge
    0b1101  // B.le
};

// Función genérica para extraer bits desde una posición con longitud específica
uint32_t extract_bits(uint32_t instruction, int shift, int length) {
    return (instruction >> shift) & ((1 << length) - 1);
}

bool check_subs_ext(uint32_t instruction) {
    uint32_t field = extract_bits(instruction, 21, 11);
    return (field + 1 == 0b11101011001);
}

uint32_t match_opcode(uint32_t value, const uint32_t* set, int len) {
    for (int i = 0; i < len; i++) {
        if (value == set[i]) return set[i];
    }
    return (uint32_t)-1;
}

uint32_t decode(uint32_t instruction) {
    // 6-bits
    uint32_t op6 = extract_bits(instruction, 26, 6);
    if (op6 == OPCODE_6BIT) return op6;

    // 8-bits
    uint32_t op8 = extract_bits(instruction, 24, 8);
    uint32_t found8 = match_opcode(op8, OPCODES_8BIT, 10);
    if (found8 != (uint32_t)-1) return found8;

    // 9-bits
    uint32_t op9 = extract_bits(instruction, 23, 9);
    if (op9 == OPCODE_9BIT) return op9;

    // 10-bits
    uint32_t op10 = extract_bits(instruction, 22, 10);
    if (op10 == OPCODE_10BIT) return op10;

    // 11-bits
    uint32_t op11 = extract_bits(instruction, 21, 11);
    uint32_t found11 = match_opcode(op11, OPCODES_11BIT, 11);
    if (found11 != (uint32_t)-1) return found11;

    // caso especial: subs_ext
    if (check_subs_ext(instruction)) return 0b11101011001;

    // 22-bits
    uint32_t op22 = extract_bits(instruction, 10, 22);
    if (op22 == OPCODE_22BIT) return op22;

    return op22;
}

uint8_t decode_branch_condition(uint32_t instruction) {
    uint8_t condition = extract_bits(instruction, 0, 4);
    for (int i = 0; i < 6; i++) {
        if (condition == B_COND_CODES[i]) return i;
    }
    return (uint8_t)-1;
}
