#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "cpu.h"




int main (int argc, char **argv)
{
    cpu_init();
    for (size_t i = 0; i < 10; ++i) {
	cpu_fetch();
	cpu_execute();
    }

    cpu_print_state();


    return 0;
}
