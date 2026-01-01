#include "alloc.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

__attribute__ ((alloc_size (2)))
void *pageAlloc(__attribute__((unused)) BareAllocator *a, size_t s) {
	a->used += s;
	return malloc(s);
}
__attribute__ ((alloc_size (2, 3)))
void *pageCalloc(__attribute__((unused)) BareAllocator *a, size_t n,size_t s) {
	a->used += n*s;
	return calloc(n,s);
}
void *pageRealloc(__attribute__((unused)) BareAllocator *a, void *ptr,size_t s) {
	a->used += s;
	return realloc(ptr,s);
}
void pageFree(__attribute__((unused)) BareAllocator *a, void *ptr, size_t sz) {
	a->used -= sz;
	free(ptr);
}

Allocator pageAllocator(void) {
	return (Allocator){
		.alloc=pageAlloc,
		.calloc=pageCalloc,
		.realloc=pageRealloc,
		.free=pageFree
	};
}
#include <stdio.h>
__attribute__ ((alloc_size (2)))
void *arenaAlloc(BareAllocator *a, size_t size) {
	a->used+=size;
	if(a->used > a->size)
		return NULL;
	return PTR_ARITH_WARN(a->memory, a->used-size);
}
__attribute__ ((alloc_size (2, 3)))
void *arenaCalloc(BareAllocator *a, size_t n, size_t size) {
	//a->used += size*n;
	if(a->used+n*size > a->size)
		return NULL;
	memset(PTR_ARITH_WARN(a->memory, a->used), 0, size*n);
	a->used += size*n;
	return PTR_ARITH_WARN(a->memory, a->used-size*n);
}
void arenaFree(BareAllocator *a, void *ptr,size_t sz) {
	if(PTR_ARITH_WARN(a->memory, a->used-sz) == ptr)
		a->used -= sz;
}
__attribute__ ((alloc_size (3)))
void *arenaRealloc(BareAllocator *a, void *ptr, size_t size) {
	arenaFree(a, ptr, size);
	return arenaAlloc(a, size);
}

Allocator arenaInit(size_t size) {
	Allocator ret = {
		.data = (BareAllocator) {
			.size = size,
			.memory = malloc(size)}
		,
		.alloc = arenaAlloc,
		.calloc = arenaCalloc,
		.realloc = arenaRealloc,
		.free = arenaFree
	};

	return ret;
}


__attribute__ ((alloc_size (2)))
void *fixedBuffAlloc(BareAllocator *a, size_t size) {
	a->used+=size;
	if(a->used > a->size)
		return NULL;
	return PTR_ARITH_WARN(a->memory,a->used-size);
}
__attribute__ ((alloc_size (2, 3)))
void *fixedBuffCalloc(BareAllocator *a, size_t n, size_t size) {
	const int sz = n*size;
	if(a->used+sz > a->size)
		return NULL;
	memset(PTR_ARITH_WARN(a->memory,a->used), 0, sz);
	a->used += sz;
	return PTR_ARITH_WARN(a->memory, a->used-sz);
}
void fixedBuffFree(BareAllocator *a, void *ptr, size_t sz) {
	if(PTR_ARITH_WARN(a->memory, a->used-sz) == ptr)
		a->used -= sz;
}
__attribute__ ((alloc_size (3)))
void *fixedBuffRealloc(BareAllocator *a, void *ptr, size_t size) {
	fixedBuffFree(a,ptr,size);
	return fixedBuffAlloc(a, size);
}

Allocator fixedBuffInit(size_t size, void *buff) {
	Allocator ret = {
		.data = (BareAllocator) {
			.size = size,
			.memory = buff
		},
		.alloc = fixedBuffAlloc,
		.calloc = fixedBuffCalloc,
		.realloc = fixedBuffRealloc,
		.free = fixedBuffFree
	};

	return ret;
}

