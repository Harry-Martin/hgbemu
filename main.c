#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "cpu.h"
#include "operand.h"
#include "opcode.h"

static cpu_t cpu;


static void print_opcode(uint16_t opcode_value)
{
    opcode_t *opcodes = opcode_get_opcodes();
    opcode_t *opcode = &opcodes[opcode_value];
    printf("Opcode 0x%02X\n", opcode_value);
    printf("	inst: %d\n", opcode->inst); 
    printf("	bytes: %d\n", opcode->bytes); 
    printf("	cycles: %d\n", opcode->cycles); 
    printf("	left operand:\n"); 
    printf("		type:%d\n", opcode->op_left.type); 
    printf("		reg:%d\n", opcode->op_left.reg); 
    printf("		immediate:%d\n", opcode->op_left.immediate); 
}

int main (int argc, char **argv)
{
    cpu_t cpu = {0};
    size_t opcode_count = opcode_get_opcode_count();

    for(size_t i = 0; i < opcode_count; i++)
    {
	print_opcode(i);
    }
    printf("number of instructions: %ld\n", opcode_count);
    

    return 0;
}
