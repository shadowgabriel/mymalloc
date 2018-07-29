#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "mymalloc.h"

void initMEM(void* start_adr_ptr, size_t mem_size) {
	/* declare headers */
	chunk_header_t start_header;
	chunk_header_t end_header;
	void* end_adr_ptr = (void*) (( (uint8_t*) start_adr_ptr) + mem_size - sizeof(chunk_header_t));

	start_header.size = mem_size - 2 * sizeof(chunk_header_t);
	start_header.state = FREE;
	start_header.prev = NULL;
	start_header.next = end_adr_ptr;

	end_header.size = 0;
	end_header.state = OCUP;
	end_header.prev = start_adr_ptr;
	end_header.next = NULL;

	insert_h(start_adr_ptr, &start_header);
	insert_h(end_adr_ptr, 	&end_header);
}

void print_all(void* start_adr_ptr) {
	chunk_header_t* chunk_ptr = (void*) start_adr_ptr;
	int go_on = 1;
	while(go_on) {
		print_h(chunk_ptr, start_adr_ptr);
		printf("\n");
		chunk_ptr = (chunk_header_t*) (chunk_ptr -> next);
		if(chunk_ptr == NULL) {
			go_on = 0;
		}
	}
}


void print_h(void* start_adr_header_ptr, void* start_adr_ptr) {

	uint8_t* mem_start_ptr = (uint8_t*) start_adr_ptr;
	chunk_header_t* chunk_ptr = (chunk_header_t*) start_adr_header_ptr;

	printf("offset = %u\n", ((uint8_t*)chunk_ptr) - mem_start_ptr);

	if(chunk_ptr -> state == OCUP) {
		printf("state = OCUP\n");
	} else {
		printf("state = FREE\n");
	}

	printf("size = %d\n", chunk_ptr -> size);

	if(chunk_ptr -> prev == NULL) {
		printf("prev = NULL\n");	
	} else {
		printf("prev = %u\n", ((uint8_t*)chunk_ptr -> prev) - mem_start_ptr);
	}

	if(chunk_ptr -> next == NULL) {
		printf("next = NULL\n");
	} else {
		printf("next = %u\n", ((uint8_t*)chunk_ptr -> next) - mem_start_ptr);
	}
}

void insert_h(void* addr, chunk_header_t* header) {

	chunk_header_t* chunk_ptr = (chunk_header_t*) addr;

	chunk_ptr -> size = header -> size;
	chunk_ptr -> state = header -> state;
	chunk_ptr -> prev = header -> prev;
	chunk_ptr -> next = header -> next;
}

void delete_h(chunk_header_t* header) {
	/* declare next & prev */
	chunk_header_t* prev_chunk = header -> prev;
	chunk_header_t* next_chunk = header -> next;
	if(prev_chunk != NULL) {
		/* link next with prev */
		prev_chunk -> next = next_chunk;
		next_chunk -> prev = prev_chunk;
		
		/* add the size of header to the previous one */
		prev_chunk -> size += header -> size + sizeof(chunk_header_t);
	
		/* if the last header is not the last, concatenate with prev */
		if(next_chunk -> next != NULL ) {
			prev_chunk -> size += next_chunk -> size;
			prev_chunk -> next = next_chunk -> next;
		}
	} else {
		if(next_chunk -> next != NULL ) {
			header -> size += next_chunk -> size;
			header -> next = next_chunk -> next;
			header -> state = next_chunk -> state;
		} 
	}
}



void* mymalloc(size_t chunk_size, void* start_adr_ptr) {
	chunk_header_t* chunk_ptr = (chunk_header_t*) start_adr_ptr;
	void* return_value;
	uint8_t found = 0;
	while(!found) {
		/* if the chunk is free */
		if(chunk_ptr -> state == FREE) {
			/* if it has the needed size + the size of a header */
			if( (chunk_ptr -> size) >= chunk_size ) {

				/* calculate remaining memory of chunk */
				size_t remaining_memory = chunk_ptr -> size - chunk_size;
				/* declare a new header for the remaining memory */
				uint8_t* new_header_addr = NULL;
				chunk_header_t new_header;
				

				if(remaining_memory >= sizeof(chunk_header_t)) {
					new_header_addr = (uint8_t*) chunk_ptr + sizeof(chunk_header_t) + chunk_size;
					new_header.size = remaining_memory - sizeof(chunk_header_t);
					new_header.state = FREE;
					new_header.prev = (void*) chunk_ptr;
					new_header.next = chunk_ptr -> next;
					insert_h(new_header_addr, &new_header);
					chunk_ptr -> next = (void*) new_header_addr;

					if(chunk_ptr -> prev != NULL) {
						chunk_header_t* prev = (chunk_header_t*) chunk_ptr -> prev;
						prev -> next = chunk_ptr -> next;
						prev -> size += chunk_size + sizeof(chunk_header_t);
						((chunk_header_t*)new_header_addr) -> prev = (void*) prev;
					} else {
						chunk_ptr -> size = chunk_size;
						chunk_ptr -> state = OCUP;
					}
					found = 1;
					return (void*) ((uint8_t*) chunk_ptr + sizeof(chunk_header_t));
				} 
			}			
		} else {
			chunk_ptr = (chunk_header_t*) chunk_ptr -> next;
		}

	} // while(!found)
	return NULL;
}

