#ifndef mymallocfree.h
#define mymallocfree.h
struct bloc_header {
	unsigned int size : 29,
		     zero : 2,
		     alloc : 1;
}
void *mymalloc(size_t size);
void myfree(void *ptr);
void *mycalloc(size_t size);
#endif
