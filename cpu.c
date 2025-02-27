#include <stdio.h>

#include "cpu.h"
#include "opcode.h"
#include "operand.h"
#include "register.h"
#include "errno.h"

typedef struct {
    union {
	struct {
	    uint8_t A; /* Accumulator */
	    union {
		uint8_t F; /* Flags */
		struct {
		    uint8_t _ :4; /* Bit 0-3 unused */
		    uint8_t c :1; /* Bit 4 -> Carry flag */
		    uint8_t h :1; /* Bit 5 -> Bit Half Carry flag (BCD) */
		    uint8_t n :1; /* Bit 6 -> Subtraction flag (BCD) */
		    uint8_t z :1; /* Bit 7 -> Zero flag */
		} F_bits;
	    };
	};
	uint16_t AF;
    };
    union {
	struct {
	    uint8_t B;
	    uint8_t C;
	};
	uint16_t BC;
    };
    union {
	struct {
	    uint8_t D;
	    uint8_t E;
	};
	uint16_t DE;
};
    union {
	struct {
	    uint8_t H;
	    uint8_t L;
	};
	uint16_t HL;
    };
    uint16_t SP; /* Stack Pointer */
    uint16_t PC; /* Program Counter */
    uint16_t IR; /* Instruction Register */
} registers_t;

typedef struct {
    registers_t reg;
    uint8_t RAM[0xFFFF + 0x0001];
} cpu_t;

static cpu_t cpu = {0};


/* ======= PRIVATE FUNCTIONS ======= */
static int read_register(register_e registerType, uint16_t* value)
{
    switch(registerType) 
    {
	case REGISTER_A:
	{
	    *value = cpu.reg.A;
	} break;
	case REGISTER_F:
	{
	    *value = cpu.reg.F;
	} break;
	case REGISTER_C:
	{
	    *value = cpu.reg.C;
	} break;
	case REGISTER_B:
	{
	    *value = cpu.reg.B;
	} break;
	case REGISTER_D:
	{
	    *value = cpu.reg.D;
	} break;
	case REGISTER_E:
	{
	    *value = cpu.reg.E;
	} break;
	case REGISTER_H:
	{
	    *value = cpu.reg.H;
	} break;
	case REGISTER_L:
	{
	    *value = cpu.reg.L;
	} break;
	default:
	{
	    return -EINVAL;
	}
    }
    return 0;
}


static int write_register(register_e registerType, uint16_t value)
{
    switch(registerType) 
    {
	case REGISTER_A:
	{
	    cpu.reg.A = (uint8_t)value;
	} break;
	case REGISTER_F:
	{
	    cpu.reg.F = (uint8_t)value;
	} break;
	case REGISTER_C:
	{
	    cpu.reg.C = (uint8_t)value;
	} break;
	case REGISTER_B:
	{
	    cpu.reg.B = (uint8_t)value;
	} break;
	case REGISTER_D:
	{
	    cpu.reg.D = (uint8_t)value;
	} break;
	case REGISTER_E:
	{
	    cpu.reg.E = (uint8_t)value;
	} break;
	case REGISTER_H:
	{
	    cpu.reg.H = (uint8_t)value;
	} break;
	case REGISTER_L:
	{
	    cpu.reg.L = (uint8_t)value;
	} break;
	case REGISTER_AF:
	{
	    cpu.reg.AF = value;
	} break;
	case REGISTER_HL:
	{
	    cpu.reg.HL = value;
	} break;
	case REGISTER_BC:
	{
	    cpu.reg.BC = value;
	} break;
	case REGISTER_DE:
	{
	    cpu.reg.DE = value;
	} break;
	case REGISTER_SP:
	{
	    cpu.reg.SP = value;
	} break;
	default:
	{
	    return -EINVAL;
	}
    }
    return 0;
}

inline static uint8_t read_byte_at_pc(bool immediate)
{
    uint8_t byte;

    byte = cpu.RAM[cpu.reg.PC];
    cpu.reg.PC++;
    if (!immediate)
    {
	byte = cpu.RAM[byte];
    }

    return byte;
}


inline static uint16_t read_short_at_pc(bool immediate)
{
    /* first read the value at the program counter
     * remembering that it is little endian*/
    uint16_t value = (uint16_t)read_byte_at_pc(true);
    printf("				VALUE BEFORE 0x%04X\n", value);
    value |= ((uint16_t)read_byte_at_pc(true) << 8);
    printf("				VALUE after 0x%04X\n", value);

    /* if this operand is not immediate, we must follow the pointer first */
    if (!immediate)
    {
	value = cpu.RAM[value];
    }

    return value;
}


static int operand_get_value(operand_t *operand, uint16_t* value)
{
    switch(operand->type)
    {
	case OPERAND_TYPE_REGISTER_8BIT:
	case OPERAND_TYPE_REGISTER_16BIT:
	{
	    read_register(operand->reg, value);
	    if (operand->immediate)
		printf("%s", register_to_string(operand->reg));
	    else
		printf("[%s]", register_to_string(operand->reg));
	} break;  

	case OPERAND_TYPE_N8:
	case OPERAND_TYPE_E8:
	{
	    if (!operand->immediate)
	    {
		/* 8bit values must be immediate */
		return -EINVAL;
	    }
	   *value = read_byte_at_pc(operand->immediate);
	    printf("0x%02X", *value);
	} break;

	case OPERAND_TYPE_N16:
	case OPERAND_TYPE_A16:
	{
	    *value = read_short_at_pc(operand->immediate);
	    if (operand->immediate)
		printf("0x%04X", *value);
	    else
		printf("[0x%04X]", *value);
	    
	} break;             

	case OPERAND_TYPE_A8:
	{
	    uint8_t HRAM_offset; 
	    HRAM_offset = read_byte_at_pc(operand->immediate);
	    *value = 0xFF00 + HRAM_offset;
	    printf("0x%02X", *value);
	} break;             

	default:
	{
	    /* invalid operand type */
	    return -EINVAL;
	}
    }
    return 0;
}

static int handle_add(operand_t *left, operand_t *right)
{
    uint16_t src;
    uint16_t dst;

    if (!(left->type == OPERAND_TYPE_REGISTER_8BIT ||
	  left->type == OPERAND_TYPE_REGISTER_16BIT))
    {
	printf("%s: SOMETHING HAS GONE WRONG", __FUNCTION__);
	/* first operand must be a register */
	return -EINVAL;
    }

    operand_get_value(left, &dst);
    printf(", ");
    operand_get_value(right, &src);
    //printf("operand_get_value: 0x%04X\n", src);
    write_register(left->reg, dst + src);
    //printf("write_register: 0x%04X into %s\n", left->reg + src, register_to_string(left->reg));
    return 0;
}


static int handle_ld(operand_t *left, operand_t *right)
{
    uint16_t src;

    if (!(left->type == OPERAND_TYPE_REGISTER_8BIT ||
	  left->type == OPERAND_TYPE_REGISTER_16BIT))
    {
	printf("%s: SOMETHING HAS GONE WRONG", __FUNCTION__);
	/* first operand must be a register */
	return -EINVAL;
    }

    printf("%s, ", register_to_string(left->reg));
    operand_get_value(right, &src);
    //printf("operand_get_value: 0x%04X\n", src);
    write_register(left->reg, src);
    //printf("write_register: 0x%04X into %s\n", src, register_to_string(left->reg));
    return 0;
}

/* ======= PRIVATE FUNCTIONS ======= */
void cpu_print_state()
{
    printf("CPU STATE:\n");
    printf("PC: 0x%04X\n", cpu.reg.PC);
    printf("A: 0x%02X\n", cpu.reg.A);
    printf("B: 0x%02X\n", cpu.reg.B);
    //printf("BC: 0x%04X\n", cpu.reg.BC);

}


void cpu_init() 
{
    cpu.reg.PC = 0x0000;
    cpu.RAM[0x0000] = 0x3E;
    cpu.RAM[0x0001] = 0x00; 
    cpu.RAM[0x0002] = 0x06;
    cpu.RAM[0x0003] = 0x01; 
    cpu.RAM[0x0004] = 0x80;
    cpu.RAM[0x0005] = 0x80;
    cpu.RAM[0x0006] = 0x80;
    cpu.RAM[0x0007] = 0x80;
    cpu.RAM[0x0008] = 0x80;
}


void cpu_fetch()
{
    cpu.reg.IR = cpu.RAM[cpu.reg.PC];
    cpu.reg.PC++;
}


void cpu_execute()
{
    opcode_t *opcode = opcode_get(cpu.reg.IR);
    printf("[0x%04X] %s ",
	    cpu.reg.PC - 1,
	    instruction_to_string(opcode->inst));
    switch(opcode->inst)
    {
	case INST_NOP:
	{
	    /* Do nothing with NOP instruction */
	} break;
	case INST_LD:
	{
	    handle_ld(&opcode->op_left, &opcode->op_right);
	} break;
	case INST_ADD:
	{
	    handle_add(&opcode->op_left, &opcode->op_right);
	} break;
	default:
	    printf("ERROR: failed to execute instruction");

    }
    printf("\n");
}
