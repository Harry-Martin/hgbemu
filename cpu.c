#include <stdio.h>

#include "cpu.h"
#include "opcode.h"

static cpu_t cpu = {0};

void cpu_init() 
{
    cpu.reg.PC = 0x0000;
}


void cpu_fetch()
{
    cpu.reg.IR = cpu.RAM[cpu.reg.PC];
    cpu.reg.PC++;
}


void cpu_execute()
{
    opcode_t *opcode = opcode_get(cpu.reg.IR);
    printf("[0x%04X] %s\n",
	    cpu.reg.PC - 1,
	    instruction_to_string(opcode->inst));


}
