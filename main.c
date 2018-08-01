#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "mymalloc.h"

int main(void) {

	memory_t memory_struct; 
	uint8_t* memory = memory_struct.memory;
	
	printf("unused address= %p\n", &(memory_struct.unused) );
	printf("mem address = %p\n", memory);
	printf("align of memory type = %d\n", __alignof__(memory_t) );	
	printf("align_mask = %d\n", align_mask );
	printf("size of header = %d\n", sizeof(chunk_header_t));

/*
	initMEM(memory, memory_size);
	printf("\tSTATE 1: ");
	printf("\n");
	print_all(memory);
	printf("\n");
	
	void* lol1 = mymalloc(30, memory);
	printf("\tSTATE 2: ");
	printf("\n");
	print_all(memory);
	printf("\n");
	
	mymalloc(50, memory);
	printf("\tSTATE 3: ");
	printf("\n");
	print_all(memory);
	printf("\n");

	void* lol2 = mymalloc(40, memory);
	printf("\tSTATE 4: ");
	printf("\n");
	print_all(memory);
	printf("\n");

	mymalloc(300, memory);
	printf("\tSTATE 5: ");
	printf("\n");
	print_all(memory);
	printf("\n");


	myfree(lol1);
	printf("\tSTATE 6: ");
	printf("\n");
	print_all(memory);
	printf("\n");

	myfree(lol2);
	printf("\tSTATE 7: ");
	printf("\n");
	print_all(memory);
	printf("\n");

	mymalloc_best(5, memory);
	printf("\tSTATE 8: ");
	printf("\n");
	print_all(memory);
	printf("\n");
*/
	return 0;
}