#ifndef __OPERAND_H__
#define __OPERAND_H__

#include <stdbool.h>

#include "register.h"

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

#endif /* __OPERAND_H__ */
