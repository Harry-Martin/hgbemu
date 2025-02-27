#include "instruction.h"


char *instruction_to_string(instruction_e inst)
{
    switch (inst)
    {
	case INST_NOP: return "NOP";
	case INST_LD: return "LD";
	case INST_LDI: return "LDI";
	case INST_LDD: return "LDD";
	case INST_INC: return "INC";
	case INST_DEC: return "DEC";
	case INST_RLCA: return "RLCA";
	case INST_ADD: return "ADD";
	case INST_RRCA: return "RRCA";
	case INST_STOP: return "STOP";
	case INST_RLA: return "RLA";
	case INST_JR: return "JR";
	case INST_JR_Z: return "JR Z";
	case INST_JR_NZ: return "JR NZ";
	case INST_JR_C: return "JR C";
	case INST_JR_NC: return "JR NC";
	case INST_RRA: return "RRA";
	case INST_DAA: return "DAA";
	case INST_CPL: return "CPL";
	case INST_SCF: return "SCF";
	case INST_CCF: return "CCF";
	case INST_ADC: return "ADC";
	case INST_SUB: return "SUB";
	case INST_SBC: return "SBC";
	case INST_AND: return "AND";
	case INST_XOR: return "XOR";
	case INST_OR: return "OR";
	case INST_CP: return "CP";
	case INST_RET: return "RET";
	case INST_RETI: return "RETI";
	case INST_RET_Z: return "RET Z";
	case INST_RET_NZ: return "RET NZ";
	case INST_RET_NC: return "RET NC";
	case INST_POP: return "POP";
	case INST_JP: return "JP";
	case INST_JP_Z: return "JP Z";
	case INST_JP_NZ: return "JP NZ";
	case INST_JP_C: return "JP C";
	case INST_JP_NC: return "JP NC";
	case INST_CALL: return "CALL";
	case INST_CALL_Z: return "CALL Z";
	case INST_CALL_NZ: return "CALL NZ";
	case INST_CALL_NC: return "CALL NC";
	case INST_PUSH: return "PUSH";
	case INST_RST: return "RST";
	case INST_PREFIX: return "PREFIX";
	case INST_LDH: return "LDH";
	case INST_DI: return "DI";
	case INST_EI: return "EI";
	case INST_HALT: return "HALT";
	default: return "UNKNOWN";
    }
}
