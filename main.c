#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "mymalloc.h"

int main(void) {
	size_t memory_size = 1024;
	uint8_t memory[memory_size];

	printf("\n");
	printf("size of chunk = %d\n", sizeof(chunk_header_t));
	printf("\n");


	initMEM(memory, memory_size);
	printf("STATE 1: ");
	printf("\n");
	print_all(memory);
	

	mymalloc(5, memory);
	printf("STATE 2: ");
	printf("\n");
	print_all(memory);

	mymalloc(10, memory);
	printf("STATE 3: ");
	printf("\n");
	print_all(memory);



	return 0;
}