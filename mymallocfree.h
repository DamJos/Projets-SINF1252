#ifndef mymallocfree.h
#define mymallocfree.h
struct bloc_header {
	unsigned int size : 29,
		     zero : 2,
		     alloc : 1;
}
void mymalloc();
void myfree();
#endif
