#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


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
} registers_t;

typedef enum {
    REGISTER_NO_REGISTER = 0,
    REGISTER_AF,
    REGISTER_A,
    REGISTER_F,
    REGISTER_BC,
    REGISTER_C,
    REGISTER_B,
    REGISTER_DE,
    REGISTER_D,
    REGISTER_E,
    REGISTER_HL,
    REGISTER_H,
    REGISTER_L,
    REGISTER_SP,
    REGISTER_PC,
} register_e;

typedef enum {
    OPERAND_TYPE_UNKNOWN = 0,
    OPERAND_TYPE_REGISTER_8BIT,  /* 8-bit Register */
    OPERAND_TYPE_REGISTER_16BIT, /* 16-bit Register */
    OPERAND_TYPE_N8,             /* Immediate 8-bit data */
    OPERAND_TYPE_N16,            /* Immediate 16-bit data (little-endian) */
    OPERAND_TYPE_A8,             /* unsigned 8-bit value as offset from $FF00 */
    OPERAND_TYPE_A16,            /* 16-bit address (little-endian) */
    OPERAND_TYPE_E8,             /* 8-bit signed data */
} operand_type_e;

typedef struct {
    operand_type_e type;
    register_e reg;
    bool immediate;
} operand_t;

typedef struct {
    char* mnemonic;
    uint8_t bytes;
    uint8_t cycles;
    operand_t op_left;
    operand_t op_right;
} opcode_t;

typedef struct {
    registers_t reg;
    uint8_t RAM[0xFFFF];
} cpu_t;

static cpu_t cpu;

opcode_t opcodes[0xFF] =
{
    [0x00] = /* NOP */
    { 
	.mnemonic = "NOP",
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_N8,
	    .immediate = true
	},
    },
    [0x01] = /* LD BC, n16 */
    { 
	.mnemonic = "LD", 
	.bytes    = 3,
	.cycles   = 12,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_BC,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_N16,
	    .immediate = true
	}
    },
    [0x02] = /*LD [BC], A */
    {
	.mnemonic = "LD", 
	.bytes    = 1,
	.cycles   = 8,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_BC,
	    .immediate = false
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true
	}
    },
    [0x03] =
    {
	.mnemonic = "INC",
	.bytes    = 1,
	.cycles   = 8,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_BC,
	    .immediate = false
	},
	.op_right = {0}
    },
    [0x04] = /* INC B */
    {
	.mnemonic = "INC",
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_B,
	    .immediate = true
	},
    },
    [0x05] = /* DEC B */
    {
	.mnemonic = "DEC",
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_B,
	    .immediate = true
	},
    },
    [0x06] = /* LD B, n8 */
    {
	.mnemonic = "DEC",
	.bytes    = 2,
	.cycles   = 8,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_B,
	    .immediate = true
	},
	.op_right  =
	{ 
	    .type = OPERAND_TYPE_N8,
	    .immediate = true
	},
    },
    [0x07] = /* RLCA */
    {
	.mnemonic = "DEC",
	.bytes    = 1,
	.cycles   = 4,
    },
    [0x08] = /* LD [a16], SP */
    {
	.mnemonic = "LD",
	.bytes    = 3,
	.cycles   = 20,
	.op_left = 
	{
	    .type = OPERAND_TYPE_A16,
	    .immediate = false,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_SP,
	    .immediate = true,
	},
    },
    [0x09] = /* ADD HL, BC */
    {
	.mnemonic = "ADD",
	.bytes    = 1,
	.cycles   = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_HL,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_BC,
	    .immediate = true,
	},
    },
};

static void print_opcode(uint16_t opcode_value)
{
    opcode_t *opcode = &opcodes[opcode_value];
    printf("Opcode 0x%02X\n", opcode_value);
    printf("	mnemonic: %s\n", opcode->mnemonic); 
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

    for(size_t i = 0; i < sizeof(opcodes) / sizeof(opcodes[0]); i++)
    {
	print_opcode(i);
    }
    printf("size: %ld", sizeof(opcodes));
    

    return 0;
}
