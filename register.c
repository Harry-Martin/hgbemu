
#include "register.h"

char* register_to_string(register_e reg)
{
    switch(reg)
    {
	case REGISTER_NO_REGISTER: { return "NO_REGISTER";} break;
	case REGISTER_AF: { return "AF";}
	case REGISTER_A: { return "A"; }
	case REGISTER_F: { return "F"; }
	case REGISTER_BC: { return "BC"; }
	case REGISTER_C: { return "C"; }
	case REGISTER_B: { return "B"; }
	case REGISTER_DE: { return "DE"; }
	case REGISTER_D: { return "D"; }
	case REGISTER_E: { return "E"; }
	case REGISTER_HL: { return "HL"; }
	case REGISTER_H: { return "H"; }
	case REGISTER_L: { return "L"; }
	case REGISTER_SP: { return "SP"; }
	case REGISTER_PC: { return "PC"; }
    }
}
