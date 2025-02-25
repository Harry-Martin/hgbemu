
#ifndef __OPCODE_H__
#define __OPCODE_H__

#include <stdlib.h>
#include <stdint.h>

#include "instruction.h"
#include "operand.h"

typedef struct {
    instruction_e inst;
    uint8_t bytes;
    uint8_t cycles;
    operand_t op_left;
    operand_t op_right;
} opcode_t;

opcode_t *opcode_get_opcodes();
size_t opcode_get_opcode_count();

#endif  /* __OPCODE_H__ */
