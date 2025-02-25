
#include <stdbool.h>
#include <stdio.h>

#include "opcode.h"

opcode_t opcodes[OPCODE_COUNT] =
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

    /* 0x4X */

    [0x40] = /* LD B, B */
    { 
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
    },
    [0x41] = /* LD B, C */
    { 
	.inst = INST_LD, 
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	},
    },
    [0x42] = /* LD B, D */
    {
	.inst = INST_LD, 
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
    },
    [0x43] = /* LD B, E */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
	.op_right = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_E,
	    .immediate = true
	},
    },
    [0x44] = /* LD B, H */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
	.op_right = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
    },
    [0x45] = /* LD B, L */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_L,
	    .immediate = true
	},
    },
    [0x46] = /* LD B, [HL] */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = false
	},
    },
    [0x47] = /* LD B, A */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
    },
    [0x48] = /* LD C, B */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
    },
    [0x49] = /* LD C, C */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	},
    },
    [0x4A] = /* LD C, D */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_C,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_D,
	    .immediate = true,
	},
    },
    [0x4B] = /* LD C, E */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_C,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_E,
	    .immediate = true,
	},
    },
    [0x4C] = /* LD C, H */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
    },
    [0x4D] = /* LD C, L */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_C,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_L,
	    .immediate = true,
	},
    },
    [0x4E] = /* LD C, [HL] */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_C,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_HL,
	    .immediate = false,
	},
    },
    [0x4F] = /* LD C, A */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_C,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
    },

    /* 0x5X */

    [0x50] = /* LD D, B */
    { 
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
    },
    [0x51] = /* LD D, C */
    { 
	.inst = INST_LD, 
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	},
    },
    [0x52] = /* LD D, D */
    {
	.inst = INST_LD, 
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
    },
    [0x53] = /* LD D, E */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
	.op_right = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_E,
	    .immediate = true
	},
    },
    [0x54] = /* LD D, H */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
	.op_right = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
    },
    [0x55] = /* LD D, L */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_L,
	    .immediate = true
	},
    },
    [0x56] = /* LD D, [HL] */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = false
	},
    },
    [0x57] = /* LD D, A */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
    },
    [0x58] = /* LD E, B */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_E,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
    },
    [0x59] = /* LD E, C */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_E,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	},
    },
    [0x5A] = /* LD E, D */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_E,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_D,
	    .immediate = true,
	},
    },
    [0x5B] = /* LD E, E */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_E,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_E,
	    .immediate = true,
	},
    },
    [0x5C] = /* LD E, H */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
    },
    [0x5D] = /* LD E, L */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_E,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_L,
	    .immediate = true,
	},
    },
    [0x5E] = /* LD E, [HL] */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_E,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_HL,
	    .immediate = false,
	},
    },
    [0x5F] = /* LD E, A */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_E,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
    },

    /* 0x6X */

    [0x60] = /* LD H, B */
    { 
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
    },
    [0x61] = /* LD H, C */
    { 
	.inst = INST_LD, 
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	},
    },
    [0x62] = /* LD H, D */
    {
	.inst = INST_LD, 
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
    },
    [0x63] = /* LD H, E */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
	.op_right = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_E,
	    .immediate = true
	},
    },
    [0x64] = /* LD H, H */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
	.op_right = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
    },
    [0x65] = /* LD H, L */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_L,
	    .immediate = true
	},
    },
    [0x66] = /* LD H, [HL] */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = false
	},
    },
    [0x67] = /* LD H, A */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
    },
    [0x68] = /* LD L, B */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_L,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
    },
    [0x69] = /* LD L, C */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_L,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	},
    },
    [0x6A] = /* LD L, D */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_L,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_D,
	    .immediate = true,
	},
    },
    [0x6B] = /* LD L, E */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_L,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_E,
	    .immediate = true,
	},
    },
    [0x6C] = /* LD L, H */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
    },
    [0x6D] = /* LD L, L */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_L,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_L,
	    .immediate = true,
	},
    },
    [0x6E] = /* LD L, [HL] */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_L,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg = REGISTER_HL,
	    .immediate = false,
	},
    },
    [0x6F] = /* LD L, A */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_L,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
    },

    /* 0x7X */

    [0x70] = /* LD [HL], B */
    { 
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = false
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
    },
    [0x71] = /* LD [HL], C */
    { 
	.inst = INST_LD, 
	.bytes    = 1,
	.cycles   = 8,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = false
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	},
    },
    [0x72] = /* LD [HL], D */
    {
	.inst = INST_LD, 
	.bytes    = 1,
	.cycles   = 8,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = false
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
    },
    [0x73] = /* LD [HL], E */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = false
	},
	.op_right = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_E,
	    .immediate = true
	},
    },
    [0x74] = /* LD [HL], H */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = false
	},
	.op_right = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
    },
    [0x75] = /* LD [HL], L */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  =
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = false
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_L,
	    .immediate = true
	},
    },
    [0x76] = /* HALT */
    {
	.inst = INST_HALT,
	.bytes    = 1,
	.cycles   = 4,
    },
    [0x77] = /* LD [HL], A */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
    },
    [0x78] = /* LD A, B */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
    },
    [0x79] = /* LD A, C */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{ 
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	},
    },
    [0x7A] = /* LD A, D */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_D,
	    .immediate = true,
	},
    },
    [0x7B] = /* LD A, E */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_E,
	    .immediate = true,
	},
    },
    [0x7C] = /* LD A, H */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
    },
    [0x7D] = /* LD A, L */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_L,
	    .immediate = true,
	},
    },
    [0x7E] = /* LD A, [HL] */
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
	    .reg = REGISTER_HL,
	    .immediate = false,
	},
    },
    [0x7F] = /* LD A, A */
    {
	.inst = INST_LD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true,
	},
    },

    /* 0x8X */

    [0x80] = /* ADD A, B */
    { 
	.inst = INST_ADD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
    },
    [0x81] = /* ADD A, C */
    { 
	.inst = INST_ADD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	},
    },
    [0x82] = /* ADD A, D */
    { 
	.inst = INST_ADD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
    },
    [0x83] = /* ADD A, E */
    { 
	.inst = INST_ADD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_E,
	    .immediate = true
	},
    },

    [0x84] = /* ADD A, H */
    { 
	.inst = INST_ADD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
    },

    [0x85] = /* ADD A, L */
    { 
	.inst = INST_ADD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_L,
	    .immediate = true
	},
    },

    [0x86] = /* ADD A, [HL] */
    { 
	.inst = INST_ADD,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = false
	},
    },

    [0x87] = /* ADD A, A */
    { 
	.inst = INST_ADD,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
    },

    [0x88] = /* ADC A, B */
    { 
	.inst = INST_ADC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
    },

    [0x89] = /* ADC A, C */
    { 
	.inst = INST_ADC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	},
    },
    [0x8A] = /* ADC A, D */
    { 
	.inst = INST_ADC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
    },
    [0x8B] = /* ADC A, E */
    { 
	.inst = INST_ADC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_E,
	    .immediate = true
	},
    },

    [0x8C] = /* ADC A, H */
    { 
	.inst = INST_ADC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
    },

    [0x8D] = /* ADC A, L */
    { 
	.inst = INST_ADC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_L,
	    .immediate = true
	},
    },

    [0x8E] = /* ADC A, [HL] */
    { 
	.inst = INST_ADC,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = false
	},
    },

    [0x8F] = /* ADC A, A */
    { 
	.inst = INST_ADC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
    },

    /* 0x9X */

    [0x90] = /* SUB A, B */
    { 
	.inst = INST_SUB,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
    },
    [0x91] = /* SUB A, C */
    { 
	.inst = INST_SUB,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	},
    },
    [0x92] = /* SUB A, D */
    { 
	.inst = INST_SUB,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
    },
    [0x93] = /* SUB A, E */
    { 
	.inst = INST_SUB,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_E,
	    .immediate = true
	},
    },

    [0x94] = /* SUB A, H */
    { 
	.inst = INST_SUB,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
    },

    [0x95] = /* SUB A, L */
    { 
	.inst = INST_SUB,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_L,
	    .immediate = true
	},
    },

    [0x96] = /* SUB A, [HL] */
    { 
	.inst = INST_SUB,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = false
	},
    },

    [0x97] = /* SUB A, A */
    { 
	.inst = INST_SUB,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
    },

    [0x98] = /* SBC A, B */
    { 
	.inst = INST_SBC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
    },

    [0x99] = /* SBC A, C */
    { 
	.inst = INST_SBC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	},
    },
    [0x9A] = /* SBC A, D */
    { 
	.inst = INST_SBC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
    },
    [0x9B] = /* SBC A, E */
    { 
	.inst = INST_SBC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_E,
	    .immediate = true
	},
    },

    [0x9C] = /* SBC A, H */
    { 
	.inst = INST_SBC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
    },

    [0x9D] = /* SBC A, L */
    { 
	.inst = INST_SBC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_L,
	    .immediate = true
	},
    },

    [0x9E] = /* SBC A, [HL] */
    { 
	.inst = INST_SBC,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = false
	},
    },

    [0x9F] = /* SBC A, A */
    { 
	.inst = INST_SBC,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
    },

    /* 0xAX */

    [0xA0] = /* AND A, B */
    { 
	.inst = INST_AND,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
    },
    [0xA1] = /* AND A, C */
    { 
	.inst = INST_AND,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	},
    },
    [0xA2] = /* AND A, D */
    { 
	.inst = INST_AND,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
    },
    [0xA3] = /* AND A, E */
    { 
	.inst = INST_AND,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_E,
	    .immediate = true
	},
    },

    [0xA4] = /* AND A, H */
    { 
	.inst = INST_AND,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
    },

    [0xA5] = /* AND A, L */
    { 
	.inst = INST_AND,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_L,
	    .immediate = true
	},
    },

    [0xA6] = /* AND A, [HL] */
    { 
	.inst = INST_AND,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = false
	},
    },

    [0xA7] = /* AND A, A */
    { 
	.inst = INST_AND,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
    },

    [0xA8] = /* XOR A, B */
    { 
	.inst = INST_XOR,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
    },

    [0xA9] = /* XOR A, C */
    { 
	.inst = INST_XOR,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	},
    },
    [0xAA] = /* XOR A, D */
    { 
	.inst = INST_XOR,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
    },
    [0xAB] = /* XOR A, E */
    { 
	.inst = INST_XOR,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_E,
	    .immediate = true
	},
    },

    [0xAC] = /* XOR A, H */
    { 
	.inst = INST_XOR,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
    },

    [0xAD] = /* XOR A, L */
    { 
	.inst = INST_XOR,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_L,
	    .immediate = true
	},
    },

    [0xAE] = /* XOR A, [HL] */
    { 
	.inst = INST_XOR,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = false
	},
    },

    [0xAF] = /* XOR A, A */
    { 
	.inst = INST_XOR,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
    },

    /* 0xBX */

    [0xB0] = /* OR A, B */
    { 
	.inst = INST_OR,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
    },
    [0xB1] = /* OR A, C */
    { 
	.inst = INST_OR,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	},
    },
    [0xB2] = /* OR A, D */
    { 
	.inst = INST_OR,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
    },
    [0xB3] = /* OR A, E */
    { 
	.inst = INST_OR,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_E,
	    .immediate = true
	},
    },

    [0xB4] = /* OR A, H */
    { 
	.inst = INST_OR,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
    },

    [0xB5] = /* OR A, L */
    { 
	.inst = INST_OR,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_L,
	    .immediate = true
	},
    },

    [0xB6] = /* OR A, [HL] */
    { 
	.inst = INST_OR,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = false
	},
    },

    [0xB7] = /* OR A, A */
    { 
	.inst = INST_OR,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
    },

    [0xB8] = /* CP A, B */
    { 
	.inst = INST_CP,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_B,
	    .immediate = true
	},
    },

    [0xB9] = /* CP A, C */
    { 
	.inst = INST_CP,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	},
    },
    [0xBA] = /* CP A, D */
    { 
	.inst = INST_CP,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_D,
	    .immediate = true
	},
    },
    [0xBB] = /* CP A, E */
    { 
	.inst = INST_CP,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_E,
	    .immediate = true
	},
    },

    [0xBC] = /* CP A, H */
    { 
	.inst = INST_CP,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_H,
	    .immediate = true
	},
    },

    [0xBD] = /* CP A, L */
    { 
	.inst = INST_CP,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_L,
	    .immediate = true
	},
    },

    [0xBE] = /* CP A, [HL] */
    { 
	.inst = INST_CP,
	.bytes    = 1,
	.cycles   = 8,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = false
	},
    },

    [0xBF] = /* CP A, A */
    { 
	.inst = INST_CP,
	.bytes    = 1,
	.cycles   = 4,
	.op_left  =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right =
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
    },
    
    /* 0xCX */

    [0xC0] = /* RET NZ */
    {
	.inst = INST_RET_NZ,
	.bytes    = 1,
	.cycles   = 111,
    },

    [0xC1] = /* POP BC */
    {
	.inst = INST_POP,
	.bytes    = 1,
	.cycles   = 12,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_BC,
	    .immediate = true
	}
    },

    [0xC2] = /* JP NZ, a16 */
    {
	.inst = INST_JP_NZ,
	.bytes    = 3,
	.cycles   = 111,
	.op_left = 
	{
	    .type = OPERAND_TYPE_A16,
	    .immediate = true
	}
    },

    [0xC3] = /* JP a16 */
    {
	.inst = INST_JP,
	.bytes    = 3,
	.cycles   = 111,
	.op_left = 
	{
	    .type = OPERAND_TYPE_A16,
	    .immediate = true
	}
    },

    [0xC4] = /* CALL NZ, a16 */
    {
	.inst = INST_CALL_NZ,
	.bytes    = 3,
	.cycles   = 111,
	.op_left = 
	{
	    .type = OPERAND_TYPE_A16,
	    .immediate = true
	}
    },

    [0xC5] = /* PUSH BC */
    {
	.inst = INST_PUSH,
	.bytes    = 1,
	.cycles   = 16,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_BC,
	    .immediate = true
	}
    },

    [0xC6] = /* ADD A, n8 */
    {
	.inst = INST_ADD,
	.bytes    = 2,
	.cycles   = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_N8,
	    .immediate = true
	}
    },

    [0xC7] = /* RST $00 */
    {
	.inst = INST_RST,
	.bytes    = 1,
	.cycles   = 16,
    },

    [0xC8] = /* RET Z */
    {
	.inst = INST_RET_Z,
	.bytes    = 1,
	.cycles   = 16,
    },

    [0xC9] = /* RET */
    {
	.inst = INST_RET,
	.bytes    = 1,
	.cycles   = 16,
    },

    [0xCA] = /* JP Z, a16 */
    {
	.inst = INST_JP_Z,
	.bytes    = 3,
	.cycles   = 111,
	.op_left =
	{
	    .type = OPERAND_TYPE_A16,
	    .immediate = true
	}
    },

    [0xCB] = /* PREFIX */
    {
	.inst = INST_PREFIX,
	.bytes    = 1,
	.cycles   = 4,
    },

    [0xCC] = /* CALL Z */
    {
	.inst = INST_CALL_Z,
	.bytes    = 3,
	.cycles   = 111,
    },

    [0xCD] = /* CALL a16 */
    {
	.inst = INST_CALL,
	.bytes    = 3,
	.cycles   = 24,
    },

    [0xCE] = /* ADC A, n8 */
    {
	.inst = INST_ADC,
	.bytes    = 2,
	.cycles   = 8,
	.op_left = 
	{
	   .type = OPERAND_TYPE_N8,
	   .immediate = true,
	}
    },

    [0xCF] = /* RST $08 */
    {
	.inst = INST_RST,
	.bytes    = 1,
	.cycles   = 16,
    },

    /* 0xDX */
    [0xD0] = /* RET NC */
    {
	.inst = INST_RET_NC,
	.bytes    = 1,
	.cycles   = 111,
    },

    [0xD1] = /* POP DE */
    {
	.inst = INST_POP,
	.bytes    = 1,
	.cycles   = 12,
	.op_left = 
	{
	   .type = OPERAND_TYPE_REGISTER_16BIT,
	   .reg  = REGISTER_DE,
	   .immediate = true,
	}
    },

    [0xD2] = /* JP NC, a16 */
    {
	.inst = INST_JP_NC,
	.bytes    = 1,
	.cycles   = 12,
	.op_left = 
	{
	   .type = OPERAND_TYPE_A16,
	   .immediate = true,
	}
    },

    [0xD3] = { 0 }, /* INVALID INSTRUCTION */

    [0xD4] = /* CALL NC, a16 */
    {
	.inst   = INST_CALL_NC,
	.bytes  = 3,
	.cycles = 111,
	.op_left = 
	{
	    .type = OPERAND_TYPE_A16,
	    .immediate = true,
	}
    },

    [0xD5] = /* PUSH DE */
    {
	.inst   = INST_PUSH,
	.bytes  = 1,
	.cycles = 16,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_DE,
	    .immediate = true,
	}
    },

    [0xD6] = /* SUB A, n8 */
    {
	.inst   = INST_SUB,
	.bytes  = 2,
	.cycles = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true,
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_N8,
	    .immediate = true,
	}
    },

    [0xD7] = /* RST $10 */
    {
	.inst   = INST_RST,
	.bytes  = 1,
	.cycles = 16,
    },

    [0xD8] = /* RET C */
    {
	.inst   = INST_RET,
	.bytes  = 1,
	.cycles = 16,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_C, 
	    .immediate = true
	}
    },

    [0xD9] = /* RETI */
    {
	.inst   = INST_RETI,
	.bytes  = 1,
	.cycles = 16,
    },

    [0xDA] = /* JP C, a16 */
    {
	.inst   = INST_JP_C,
	.bytes  = 1,
	.cycles = 111,
	.op_left = 
	{
	    .type = OPERAND_TYPE_A16,
	    .immediate = true
	}
    },

    [0xDB] = { 0 }, /* INVALID INSTRUCTION */

    [0xDC] = /* CALL C */
    {
	.inst   = INST_CALL,
	.bytes  = 3,
	.cycles = 24,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = true
	}
    },

    [0xDD] = { 0 }, /* INVALID INSTRUCTION */

    [0xDE] = /* SBC A, n8 */
    {
	.inst   = INST_SBC,
	.bytes  = 2,
	.cycles = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_N8,
	    .immediate = true
	}
    },

    [0xDF] = /* RST $18 */
    {
	.inst   = INST_RST,
	.bytes  = 1,
	.cycles = 16,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_N8,
	    .immediate = true
	}
    },

    /* 0xEX */ 

    [0xE0] = /* LDH [a8], A */
    {
	.inst   = INST_LDH,
	.bytes  = 2,
	.cycles = 12,
	.op_left = 
	{
	    .type = OPERAND_TYPE_A8,
	    .immediate = false
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	}
    },

    [0xE1] = /* POP HL */
    {
	.inst   = INST_POP,
	.bytes  = 1,
	.cycles = 12,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = true
	},
    },

    [0xE2] = /* LDH [C], A */
    {
	.inst   = INST_LDH,
	.bytes  = 1,
	.cycles = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = false
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
    },

    [0xE3] = { 0 }, /* INVALID INSTRUCTION */

    [0xE4] = { 0 }, /* INVALID INSTRUCTION */

    [0xE5] = /* PUSH HL */
    {
	.inst   = INST_PUSH,
	.bytes  = 1,
	.cycles = 16,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = true
	},
    },

    [0xE6] = /* AND A, n8 */
    {
	.inst   = INST_AND,
	.bytes  = 2,
	.cycles = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_N8,
	    .immediate = true
	},
    },

    [0xE7] = /* RST $20 */
    {
	.inst   = INST_RST,
	.bytes  = 1,
	.cycles = 16,
    },
    
    [0xE8] = /* ADD SP,e8 */
    {
	.inst   = INST_ADD,
	.bytes  = 2,
	.cycles = 16,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_SP,
	    .immediate = true
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_E8,
	    .immediate = true
	},
    },

    [0xE9] = /* JP HL */
    {
	.inst   = INST_JP,
	.bytes  = 1,
	.cycles = 4,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = true
	},
    },

    [0xEA] = /* LD [a16], A */
    {
	.inst   = INST_LD,
	.bytes  = 3,
	.cycles = 16,
	.op_left = 
	{
	    .type = OPERAND_TYPE_A16,
	    .immediate = false
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true
	},
    },

    [0xEB] = { 0 }, /* INVALID INSTRUCTION */

    [0xEC] = { 0 }, /* INVALID INSTRUCTION */

    [0xED] = { 0 }, /* INVALID INSTRUCTION */

    [0xEE] = /* XOR A, n8 */
    {
	.inst = INST_XOR,
	.bytes = 2,
	.cycles = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_N8,
	    .immediate = true
	}
    },

    [0xEF] = /* RST $28 */
    {
	.inst = INST_RST,
	.bytes = 1,
	.cycles = 16,
    },

    /* 0xFX */ 

    [0xF0] = /* LDH A, [a8] */
    {
	.inst   = INST_LDH,
	.bytes  = 2,
	.cycles = 12,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_A8,
	    .immediate = false
	}
    },

    [0xF1] = /* POP AF */
    {
	.inst   = INST_POP,
	.bytes  = 1,
	.cycles = 12,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_AF,
	    .immediate = true
	},
    },

    [0xF2] = /* LDH A, [C] */
    {
	.inst   = INST_LDH,
	.bytes  = 1,
	.cycles = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_C,
	    .immediate = false
	},
    },

    [0xF3] = /* DI */
    {
	.inst = INST_DI,
	.bytes = 1,
	.cycles = 4
    },

    [0xF4] = { 0 }, /* INVALID INSTRUCTION */

    [0xF5] = /* PUSH AF */
    {
	.inst   = INST_PUSH,
	.bytes  = 1,
	.cycles = 16,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_AF,
	    .immediate = true
	},
    },

    [0xF6] = /* OR A, n8 */
    {
	.inst   = INST_OR,
	.bytes  = 2,
	.cycles = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg  = REGISTER_A,
	    .immediate = true
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_N8,
	    .immediate = true
	},
    },

    [0xF7] = /* RST $30 */
    {
	.inst   = INST_RST,
	.bytes  = 1,
	.cycles = 16,
    },
    
    [0xF8] = /* LD HL, SP + e8 */
    {
	.inst   = INST_LD,
	.bytes  = 2,
	.cycles = 12,
	.op_left = 
	{
	    .type = OPERAND_TYPE_E8,
	    .immediate = true
	},
    },

    [0xF9] = /* LD SP, HL */
    {
	.inst   = INST_JP,
	.bytes  = 1,
	.cycles = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_16BIT,
	    .reg  = REGISTER_HL,
	    .immediate = true
	},
    },

    [0xFA] = /* LD [a16], A */
    {
	.inst   = INST_LD,
	.bytes  = 3,
	.cycles = 16,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_A16,
	    .immediate = false
	},
    },

    [0xFB] = /* EI */
    {
	.inst = INST_EI,
	.bytes = 1,
	.cycles = 4,
    },

    [0xFC] = { 0 }, /* INVALID INSTRUCTION */

    [0xFD] = { 0 }, /* INVALID INSTRUCTION */

    [0xFE] = /* CP A, n8 */
    {
	.inst   = INST_CP,
	.bytes  = 2,
	.cycles = 8,
	.op_left = 
	{
	    .type = OPERAND_TYPE_REGISTER_8BIT,
	    .reg = REGISTER_A,
	    .immediate = true
	},
	.op_right = 
	{
	    .type = OPERAND_TYPE_A8,
	    .immediate = true
	},
    },

    [0xFF] = /* RST 38 */
    {
	.inst   = INST_CP,
	.bytes  = 2,
	.cycles = 8,
    },
};

inline opcode_t *opcode_get(uint8_t opcode_value)
{
    return &opcodes[opcode_value];
}


void opcode_print(uint8_t opcode_value)
{
    opcode_t *opcode = opcode_get(opcode_value);
    printf("Opcode 0x%02X\n", opcode_value);
    printf("	inst: %d\n", opcode->inst); 
    printf("	bytes: %d\n", opcode->bytes); 
    printf("	cycles: %d\n", opcode->cycles); 
    printf("	left operand:\n"); 
    printf("		type:%d\n", opcode->op_left.type); 
    printf("		reg:%d\n", opcode->op_left.reg); 
    printf("		immediate:%d\n", opcode->op_left.immediate); 
}
