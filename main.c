#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main (int argc, char **argv) {
	int count = atoi(argv[1]);
	
	for (int i = 0; i < count ; i++) {
		printf("Hello, World!\n");
		sleep(1);
	}
}
