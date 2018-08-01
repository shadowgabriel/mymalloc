#ifndef MYMALLOC_H
#define MYMALLOC_H

#define FREE (0u)
#define OCUP (1u)

#define MEMORY_SIZE (1024ul)

#ifdef __alignof_is_defined
	#define MAX_ALIGN (alignof(max_align_t))
#else
	#ifdef __GNUC__
		#define MAX_ALIGN (__alignof__(max_align_t))
	#else
		#error Compile with GCC or C11
	#endif
#endif

#define ALIGN_MASK (~( MAX_ALIGN - 1))

/* memory map 
usused assures that the memory array is properly aligned */
typedef struct {
	max_align_t unused;
	uint8_t memory[MEMORY_SIZE];
} memory_t; 

/* chunk header structure double linked list  
pading added for keeping alignment 
even if packed */
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