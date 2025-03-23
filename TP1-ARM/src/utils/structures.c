#include "structures.h"

Instruction decode_instruction(uint32_t instruction)
{
    Instruction decoded;
    decoded.opcode = (instruction >> 26) & 0x3F;
    decoded.operand1 = (instruction >> 16) & 0xFF;
    decoded.operand2 = (instruction >> 0) & 0xFFFF;
}

Instruction execute_instruction(Instruction decoded)
{
    Instruction executed;
    executed.opcode = decoded.opcode;
    executed.operand1 = decoded.operand1;
    executed.operand2 = decoded.operand2;
}