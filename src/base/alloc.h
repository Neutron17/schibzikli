#ifndef _NTR_ALLOC_H_
#define _NTR_ALLOC_H_ 1
#include <stddef.h>

typedef struct {
	void *memory;
	size_t size;
	size_t used;
} BareAllocator;

typedef struct Alloc_t {
	BareAllocator data;

	void *(*alloc)(BareAllocator *, size_t) __attribute__ ((alloc_size (2)));
	void *(*calloc)(BareAllocator *, size_t,size_t) __attribute__ ((alloc_size (2, 3)));
	void *(*realloc)(BareAllocator *,void *,size_t) __attribute__ ((alloc_size (3)));
	void (*free)(BareAllocator *, void *,size_t);
} Allocator;

Allocator pageAllocator(void);
__attribute__ ((alloc_size (2)))
void *pageAlloc(__attribute__((unused)) BareAllocator *a, size_t s);
__attribute__ ((alloc_size (2, 3)))
void *pageCalloc(__attribute__((unused)) BareAllocator *a, size_t n,size_t s);
void *pageRealloc(__attribute__((unused)) BareAllocator *a, void *ptr,size_t s);
void pageFree(__attribute__((unused)) BareAllocator *a, void *ptr, size_t sz);

Allocator arenaInit(size_t size);
#define arenaInitTyped(T, N) arenaInit((N)*sizeof(T))
__attribute__ ((alloc_size (2)))
void *arenaAlloc(BareAllocator *a, size_t size);
__attribute__ ((alloc_size (2, 3)))
void *arenaCalloc(BareAllocator *a, size_t n, size_t size);
void arenaFree(BareAllocator *a, void *ptr,size_t sz);
__attribute__ ((alloc_size (3)))
void *arenaRealloc(BareAllocator *a, void *ptr, size_t size);

Allocator fixedBuffInit(size_t size, void *buff);
__attribute__ ((alloc_size (2)))
void *fixedBuffAlloc(BareAllocator *a, size_t size);
__attribute__ ((alloc_size (2, 3)))
void *fixedBuffCalloc(BareAllocator *a, size_t n, size_t size);
void fixedBuffFree(BareAllocator *a, void *ptr, size_t sz);
__attribute__ ((alloc_size (3)))
void *fixedBuffRealloc(BareAllocator *a, void *ptr, size_t size);

#endif /* ifndef _NTR_ALLOC_H_ 1 */

