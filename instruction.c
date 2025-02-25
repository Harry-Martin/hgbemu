#include "instruction.h"


char *instruction_to_string(instruction_e inst)
{
    switch (inst)
    {
	case INST_NOP: return "INST_NOP";
	case INST_LD: return "INST_LD";
	case INST_LDI: return "INST_LDI";
	case INST_LDD: return "INST_LDD";
	case INST_INC: return "INST_INC";
	case INST_DEC: return "INST_DEC";
	case INST_RLCA: return "INST_RLCA";
	case INST_ADD: return "INST_ADD";
	case INST_RRCA: return "INST_RRCA";
	case INST_STOP: return "INST_STOP";
	case INST_RLA: return "INST_RLA";
	case INST_JR: return "INST_JR";
	case INST_JR_Z: return "INST_JR_Z";
	case INST_JR_NZ: return "INST_JR_NZ";
	case INST_JR_C: return "INST_JR_C";
	case INST_JR_NC: return "INST_JR_NC";
	case INST_RRA: return "INST_RRA";
	case INST_DAA: return "INST_DAA";
	case INST_CPL: return "INST_CPL";
	case INST_SCF: return "INST_SCF";
	case INST_CCF: return "INST_CCF";
	case INST_ADC: return "INST_ADC";
	case INST_SUB: return "INST_SUB";
	case INST_SBC: return "INST_SBC";
	case INST_AND: return "INST_AND";
	case INST_XOR: return "INST_XOR";
	case INST_OR: return "INST_OR";
	case INST_CP: return "INST_CP";
	case INST_RET: return "INST_RET";
	case INST_RETI: return "INST_RETI";
	case INST_RET_Z: return "INST_RET_Z";
	case INST_RET_NZ: return "INST_RET_NZ";
	case INST_RET_NC: return "INST_RET_NC";
	case INST_POP: return "INST_POP";
	case INST_JP: return "INST_JP";
	case INST_JP_Z: return "INST_JP_Z";
	case INST_JP_NZ: return "INST_JP_NZ";
	case INST_JP_C: return "INST_JP_C";
	case INST_JP_NC: return "INST_JP_NC";
	case INST_CALL: return "INST_CALL";
	case INST_CALL_Z: return "INST_CALL_Z";
	case INST_CALL_NZ: return "INST_CALL_NZ";
	case INST_CALL_NC: return "INST_CALL_NC";
	case INST_PUSH: return "INST_PUSH";
	case INST_RST: return "INST_RST";
	case INST_PREFIX: return "INST_PREFIX";
	case INST_LDH: return "INST_LDH";
	case INST_DI: return "INST_DI";
	case INST_EI: return "INST_EI";
	case INST_HALT: return "INST_HALT";
    }
}
