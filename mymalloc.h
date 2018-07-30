#ifndef MYMALLOC_H
#define MYMALLOC_H

#define FREE (0u)
#define OCUP (1u)

//__alignof__(max_align_t)


typedef struct {
	size_t size;
	uint8_t state;
	void* prev;
	void* next;
} chunk_header_t;

void myfree(void* loc_ptr);
void initMEM(void* start_adr_ptr, size_t mem_size);
void print_all(void* start_adr_ptr);
void* mymalloc(size_t chunk_size, void* start_adr_ptr);
void print_h(void* start_adr_header_ptr, void* start_adr_ptr);
void insert_h(void* addr, chunk_header_t* header);
void delete_h(chunk_header_t* header);
void* mymalloc_best(size_t chunk_size, void* start_adr_ptr);
#endif /* MYMALLOC_H */

/*


[state][prev][next]























*/