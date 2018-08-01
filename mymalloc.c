/* std library dependencies */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* application specific headers */
#include "mymalloc.h"

/* local function declarations */
static void print_h(void* start_adr_header_ptr, void* start_adr_ptr);
static void insert_h(void* addr, chunk_header_t* header);
static void delete_h(chunk_header_t* header_ptr);


/* implementation */

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

void myfree(void* loc_ptr) {
	chunk_header_t* header_ptr = (chunk_header_t*) ( ((uint8_t*) loc_ptr) - sizeof(chunk_header_t) );
	delete_h(header_ptr);
}

void* mymalloc(size_t chunk_size, void* start_adr_ptr) {

	chunk_header_t* chunk_ptr = (chunk_header_t*) start_adr_ptr;

	while(1) {
		/* if the chunk is free */
		/* if it has the needed size + the size of a header */
		if( (chunk_ptr -> state == FREE) && (chunk_ptr -> size >= chunk_size) ) {
			/* calculate remaining memory of chunk */
			size_t remaining_memory = chunk_ptr -> size - chunk_size;
			/* declare a new header for the remaining memory */
			uint8_t* new_header_addr = NULL;
			chunk_header_t new_header;
			
			/* if there is enough memory, add another free header or give all chunk */
			if(remaining_memory > sizeof(chunk_header_t)) {
				/* find the address for next chunk */
				new_header_addr = (uint8_t*) chunk_ptr + sizeof(chunk_header_t) + chunk_size;
				/* configure the new header */
				new_header.size = remaining_memory - sizeof(chunk_header_t);
				new_header.state = FREE;
				new_header.prev = (void*) chunk_ptr;
				new_header.next = chunk_ptr -> next;
				insert_h(new_header_addr, &new_header);

				/* update the next node */
				chunk_ptr -> next = (void*) new_header_addr;
				chunk_ptr -> state = OCUP;
				chunk_ptr -> size = chunk_size;
			} else { 
				/* no memory left for another chunk */
				/* we just update the state */
				chunk_ptr -> state = OCUP;
			}
			/* update flag, return pointer */
			return (void*) ((uint8_t*) chunk_ptr + sizeof(chunk_header_t));
		} 

		/* if end reached */
		if(chunk_ptr -> next == NULL) {
			return NULL;
		}

		/* move to next chunk */
		chunk_ptr = (chunk_header_t*) chunk_ptr -> next;
	} // while(1)
}

void* mymalloc_best(size_t chunk_size, void* start_adr_ptr) {

	chunk_header_t* chunk_ptr = (chunk_header_t*) start_adr_ptr;
	chunk_header_t* best_chunk_ptr = NULL;
	size_t best_size = 0;
	uint8_t traversed_all = 0;

	/* find best chunk */
	while(!traversed_all) {
		/* if it's a viable chunk */
		if( (chunk_ptr -> state == FREE) && (chunk_ptr -> size >= chunk_size) ) {
			if (chunk_ptr -> size > best_size) {
				best_size = chunk_ptr -> size;
				best_chunk_ptr = chunk_ptr;
			}
		} 
		/* move to next chunk */
		chunk_ptr = (chunk_header_t*) chunk_ptr -> next;
		/* if end reached */
		if(chunk_ptr == NULL) {
			traversed_all = 1;
		}
	} // while(!traversed_all)

	if(best_chunk_ptr != NULL) {
		chunk_ptr = best_chunk_ptr;
		/* calculate remaining memory of chunk */
		size_t remaining_memory = chunk_ptr -> size - chunk_size;
		/* declare a new header for the remaining memory */
		uint8_t* new_header_addr = NULL;
		chunk_header_t new_header;
		/* if there is enough memory, add another free header or give all chunk */
		if(remaining_memory > sizeof(chunk_header_t)) {
			/* find the address for next chunk */
			new_header_addr = (uint8_t*) chunk_ptr + sizeof(chunk_header_t) + chunk_size;
			/* configure the new header */
			new_header.size = remaining_memory - sizeof(chunk_header_t);
			new_header.state = FREE;
			new_header.prev = (void*) chunk_ptr;
			new_header.next = chunk_ptr -> next;
			insert_h(new_header_addr, &new_header);
			/* update the next node */
			chunk_ptr -> next = (void*) new_header_addr;
			chunk_ptr -> state = OCUP;
			chunk_ptr -> size = chunk_size;
		} else { 
			/* no memory left for another chunk */
			/* we just update the state */
			chunk_ptr -> state = OCUP;
		}
		/* update flag, return pointer */
		return (void*) ((uint8_t*) chunk_ptr + sizeof(chunk_header_t));
	} 
	return NULL;
}

static void print_h(void* start_adr_header_ptr, void* start_adr_ptr) {

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

static void insert_h(void* addr, chunk_header_t* header) {

	chunk_header_t* chunk_ptr = (chunk_header_t*) addr;

	chunk_ptr -> size = header -> size;
	chunk_ptr -> state = header -> state;
	chunk_ptr -> prev = header -> prev;
	chunk_ptr -> next = header -> next;
}

static void delete_h(chunk_header_t* header_ptr) {
	/* declare next & prev */
	chunk_header_t* prev_chunk = header_ptr -> prev;
	chunk_header_t* next_chunk = header_ptr -> next;

	/* free chunk */
	header_ptr -> state = FREE;

	/* if it's not the last chunk */
	if( (prev_chunk != NULL) && (prev_chunk -> state == FREE) ) {
		/* link next with prev */
		prev_chunk -> next = next_chunk;
		next_chunk -> prev = prev_chunk;
		
		/* add the size of header to the previous one */
		prev_chunk -> size += header_ptr -> size + sizeof(chunk_header_t);
	
		/* if the next chunk is free and not the last, concatenate with prev */
		header_ptr = prev_chunk;
		
	} 

	/* link with next */
	if( (next_chunk -> next != NULL) && (next_chunk -> state == FREE) ) {
		header_ptr -> size += next_chunk -> size + sizeof(chunk_header_t);
		header_ptr -> next = next_chunk -> next;
	} 
}

