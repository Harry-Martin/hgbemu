
#ifndef __OPCODE_H__
#define __OPCODE_H__

#include <stdlib.h>
#include <stdint.h>

#include "instruction.h"
#include "operand.h"

#define OPCODE_COUNT 256

typedef struct {
    instruction_e inst;
    uint8_t bytes;
    uint8_t cycles;
    operand_t op_left;
    operand_t op_right;
} opcode_t;

opcode_t *opcode_get(uint8_t opcode_value);
void opcode_print(uint8_t opcode_value);


#endif  /* __OPCODE_H__ */
