#include "debug.h"
#include <stdint.h>

void print_array(uint64_t array[], int n_elem) {
	int i;

	for (i = 0; i < n_elem; i++) {
		printf(" 0x%012llX |\n", array[i]);
	}
}
