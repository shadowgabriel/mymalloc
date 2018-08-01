#ifndef MYMALLOC_H
#define MYMALLOC_H

#define FREE (0u)
#define OCUP (1u)

// __alignof__(max_align_t)
// __attribute__ (aligned(__alignof__(max_align_t)));

#define memory_size (1024ul)
#define align_mask (__alignof__(max_align_t) - 1)

typedef struct {
	max_align_t unused;
	uint8_t memory[memory_size];
} memory_t; 



#ifdef lol
#error compile with gcc
#endif

typedef struct {
	uint8_t state;
	uint8_t pad[3];
	size_t size;
	void* prev;
	void* next;
} chunk_header_t;


void print_all(void* start_adr_ptr);
void initMEM(void* start_adr_ptr, size_t mem_size);
void myfree(void* loc_ptr);
void* mymalloc(size_t chunk_size, void* start_adr_ptr);
void* mymalloc_best(size_t chunk_size, void* start_adr_ptr);

#endif /* MYMALLOC_H */

/*


[state][prev][next]























*/