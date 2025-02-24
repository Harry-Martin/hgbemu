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
    INST_NOP,
    INST_LD,
    INST_LDI,
    INST_LDD,
    INST_INC,
    INST_DEC,
    INST_RLCA,
    INST_ADD,
    INST_RRCA,
    INST_STOP,
    INST_RLA,
    INST_JR,
    INST_JR_Z,
    INST_JR_NZ,
    INST_JR_C,
    INST_JR_NC,
    INST_RRA,
    INST_DAA,
    INST_CPL,
    INST_SCF,
    INST_CCF,
    INST_ADC,
    INST_SUB,
    INST_SBC,
    INST_AND,
    INST_XOR,
    ISNT_OR,
    INST_CP,
    INST_RET,
    INST_POP,
    INST_JP,
    INST_JP_Z,
    INST_JP_NZ,
    INST_JP_C,
    INST_JP_NC,
    INST_CALL,
    INST_PUSH,
    INST_RST,
    INST_PREFIX,
    INST_LDH,
    INST_DI,
    INST_EI,
} instruction_e;

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
    instruction_e inst;
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
	.inst = INST_NOP,
	.bytes    = 1,
	.cycles   = 4,
    },
    [0x01] = /* LD BC, n16 */
    { 
	.inst = INST_LD, 
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
	.inst = INST_LD, 
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
    [0x03] = /* INC BC */
    {
	.inst = INST_INC,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_BC,
	    .immediate = true
	},
    },
    [0x04] = /* INC B */
    {
	.inst = INST_INC,
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
	.inst = INST_DEC,
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
	.inst = INST_LD,
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
	.inst = INST_RLCA,
	.bytes    = 1,
	.cycles   = 4,
    },
    [0x08] = /* LD [a16], SP */
    {
	.inst = INST_LD,
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
	.inst = INST_ADD,
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
    [0x0A] = /* LD A [BC] */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_BC,
	    .immediate = false,
	},
    },
    [0x0B] = /* DEC BC*/
    {
	.inst = INST_DEC,
	.bytes    = 1,
	.cycles   = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_BC,
	    .immediate = true,
	},
    },
    [0x0C] = /* INC C */
    {
	.inst = INST_INC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_C,
	    .immediate = true,
	},
    },
    [0x0D] = /* DEC C */
    {
	.inst = INST_INC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_C,
	    .immediate = true,
	},
    },
    [0x0E] = /* LD C, n8 */
    {
	.inst = INST_LD,
	.bytes    = 2,
	.cycles   = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_C,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_N8,
	    .immediate = true,
	},
    },
    [0x0F] = /* RRCA */
    {
	.inst = INST_RRCA,
	.bytes    = 1,
	.cycles   = 4,
    },

    /* 0x1X */

    [0x10] = /* STOP n8 */
    { 
	.inst = INST_STOP,
	.bytes    = 2,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_N8,
	    .immediate = true
	},
    },
    [0x11] = /* LD DE, n16 */
    { 
	.inst = INST_LD, 
	.bytes    = 3,
	.cycles   = 12,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_DE,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_N16,
	    .immediate = true
	}
    },
    [0x12] = /* LD [DE], A */
    {
	.inst = INST_LD, 
	.bytes    = 1,
	.cycles   = 8,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_DE,
	    .immediate = false
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true
	}
    },
    [0x13] = /* INC DE */
    {
	.inst = INST_INC,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_DE,
	    .immediate = true
	},
    },
    [0x14] = /* INC D */
    {
	.inst = INST_INC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_D,
	    .immediate = true
	},
    },
    [0x15] = /* DEC D */
    {
	.inst = INST_DEC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_D,
	    .immediate = true
	},
    },
    [0x16] = /* LD D, n8 */
    {
	.inst = INST_LD,
	.bytes    = 2,
	.cycles   = 8,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_D,
	    .immediate = true
	},
	.op_right  =
	{ 
	    .type = OPERAND_TYPE_N8,
	    .immediate = true
	},
    },
    [0x17] = /* RLA */
    {
	.inst = INST_RLA,
	.bytes    = 1,
	.cycles   = 4,
    },
    [0x18] = /* JR e8 */
    {
	.inst = INST_JR,
	.bytes    = 2,
	.cycles   = 12,
	.op_left = 
	{
	    .type = OPERAND_TYPE_E8,
	    .immediate = true,
	},
    },
    [0x19] = /* ADD HL, DE */
    {
	.inst = INST_ADD,
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
	    .reg = REGISTER_DE,
	    .immediate = true,
	},
    },
    [0x1A] = /* LD A, [DE] */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_DE,
	    .immediate = false,
	},
    },
    [0x1B] = /* DEC DE*/
    {
	.inst = INST_DEC,
	.bytes    = 1,
	.cycles   = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_DE,
	    .immediate = true,
	},
    },
    [0x1C] = /* INC E */
    {
	.inst = INST_INC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_E,
	    .immediate = true,
	},
    },
    [0x1D] = /* DEC E */
    {
	.inst = INST_INC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_E,
	    .immediate = true,
	},
    },
    [0x1E] = /* LD E, n8 */
    {
	.inst = INST_LD,
	.bytes    = 2,
	.cycles   = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_E,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_N8,
	    .immediate = true,
	},
    },
    [0x1F] = /* RRA */
    {
	.inst = INST_RRA,
	.bytes    = 1,
	.cycles   = 4,
    },

    /* 0x2X */

    [0x20] = /* JR NZ, e8 */
    { 
	.inst = INST_JR,
	.bytes    = 2,
	.cycles   = 111,
	.op_left  =
	{
	    .type = OPERAND_TYPE_E8,
	    .immediate = true
	},
    },
    [0x21] = /* LD HL, n16 */
    { 
	.inst = INST_LD, 
	.bytes    = 3,
	.cycles   = 12,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_HL,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_N16,
	    .immediate = true
	}
    },
    [0x22] = /* LDI [HL], A */
    {
	.inst = INST_LDI, 
	.bytes    = 1,
	.cycles   = 8,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_HL,
	    .immediate = false
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true
	}
    },
    [0x23] = /* INC HL */
    {
	.inst = INST_INC,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_HL,
	    .immediate = true
	},
    },
    [0x24] = /* INC H */
    {
	.inst = INST_INC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_H,
	    .immediate = true
	},
    },
    [0x25] = /* DEC H */
    {
	.inst = INST_DEC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_H,
	    .immediate = true
	},
    },
    [0x26] = /* LD H, n8 */
    {
	.inst = INST_LD,
	.bytes    = 2,
	.cycles   = 8,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_H,
	    .immediate = true
	},
	.op_right  =
	{ 
	    .type = OPERAND_TYPE_N8,
	    .immediate = true
	},
    },
    [0x27] = /* DAA */
    {
	.inst = INST_DAA,
	.bytes    = 1,
	.cycles   = 4,
    },
    [0x28] = /* JR Z, e8 */
    {
	.inst = INST_JR_Z,
	.bytes    = 2,
	.cycles   = 111,
	.op_left = 
	{
	    .type = OPERAND_TYPE_E8,
	    .immediate = true,
	},
    },
    [0x29] = /* ADD HL, HL */
    {
	.inst = INST_ADD,
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
	    .reg = REGISTER_HL,
	    .immediate = true,
	},
    },
    [0x2A] = /* LDI A, [HL] */
    {
	.inst = INST_LDI,
	.bytes    = 1,
	.cycles   = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_HL,
	    .immediate = false,
	},
    },
    [0x2B] = /* DEC HL*/
    {
	.inst = INST_DEC,
	.bytes    = 1,
	.cycles   = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_HL,
	    .immediate = true,
	},
    },
    [0x2C] = /* INC L */
    {
	.inst = INST_INC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_L,
	    .immediate = true,
	},
    },
    [0x2D] = /* DEC L */
    {
	.inst = INST_INC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_L,
	    .immediate = true,
	},
    },
    [0x2E] = /* LD L, n8 */
    {
	.inst = INST_LD,
	.bytes    = 2,
	.cycles   = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_L,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_N8,
	    .immediate = true,
	},
    },
    [0x2F] = /* CPL */
    {
	.inst = INST_CPL,
	.bytes    = 1,
	.cycles   = 4,
    },

    /* 0x3X */

    [0x30] = /* JR NC, e8 */
    { 
	.inst = INST_JR_NC,
	.bytes    = 2,
	.cycles   = 111,
	.op_left  =
	{
	    .type = OPERAND_TYPE_E8,
	    .immediate = true
	},
    },
    [0x31] = /* LD SP, n16 */
    { 
	.inst = INST_LD, 
	.bytes    = 3,
	.cycles   = 12,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_SP,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_N16,
	    .immediate = true
	}
    },
    [0x32] = /* LDD [HL], A */
    {
	.inst = INST_LDD, 
	.bytes    = 1,
	.cycles   = 8,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_DE,
	    .immediate = false
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true
	}
    },
    [0x33] = /* INC SP */
    {
	.inst = INST_INC,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_SP,
	    .immediate = true
	},
    },
    [0x34] = /* INC [HL] */
    {
	.inst = INST_INC,
	.bytes    = 1,
	.cycles   = 12,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_HL,
	    .immediate = false
	},
    },
    [0x35] = /* DEC [HL] */
    {
	.inst = INST_DEC,
	.bytes    = 1,
	.cycles   = 12,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_HL,
	    .immediate = false
	},
    },
    [0x36] = /* LD [HL], n8 */
    {
	.inst = INST_LD,
	.bytes    = 2,
	.cycles   = 12,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_HL,
	    .immediate = false
	},
	.op_right  =
	{ 
	    .type = OPERAND_TYPE_N8,
	    .immediate = true
	},
    },
    [0x37] = /* SCF */
    {
	.inst = INST_SCF,
	.bytes    = 1,
	.cycles   = 4,
    },
    [0x38] = /* JR C, e8 */
    {
	.inst = INST_JR_C,
	.bytes    = 2,
	.cycles   = 111,
	.op_left = 
	{
	    .type = OPERAND_TYPE_E8,
	    .immediate = true,
	},
    },
    [0x39] = /* ADD HL, SP */
    {
	.inst = INST_ADD,
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
	    .reg = REGISTER_SP,
	    .immediate = true,
	},
    },
    [0x3A] = /* LDD A, [HL] */
    {
	.inst = INST_LDI,
	.bytes    = 1,
	.cycles   = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_HL,
	    .immediate = false,
	},
    },
    [0x3B] = /* DEC SP*/
    {
	.inst = INST_DEC,
	.bytes    = 1,
	.cycles   = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_SP,
	    .immediate = true,
	},
    },
    [0x3C] = /* INC A */
    {
	.inst = INST_INC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
    },
    [0x3D] = /* DEC A */
    {
	.inst = INST_INC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
    },
    [0x3E] = /* LD A, n8 */
    {
	.inst = INST_LD,
	.bytes    = 2,
	.cycles   = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_N8,
	    .immediate = true,
	},
    },
    [0x3F] = /* CCF */
    {
	.inst = INST_CCF,
	.bytes    = 1,
	.cycles   = 4,
    },
};

static void print_opcode(uint16_t opcode_value)
{
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

    for(size_t i = 0; i < sizeof(opcodes) / sizeof(opcodes[0]); i++)
    {
	print_opcode(i);
    }
    printf("size: %ld", sizeof(opcodes));
    

    return 0;
}
