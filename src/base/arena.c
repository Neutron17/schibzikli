#include <stdlib.h>
#include <string.h>
#include "arena.h"
#include "global.h"

Arena arenaInit(uint32_t size) {
	return (Arena) {
		.size = size,
		.memory = malloc(size)
	};
}

void arenaDestroy(Arena *arena) {
	free(arena->memory);
	arena->memory = NULL;
	arena->size = 0;
}
void arenaFree(Arena *arena, void *ptr, uint32_t size) {
	if(PTR_ARITH_WARN(arena->memory, arena->used-size) == ptr) 
		arena->used -= size;
}
__attribute__ ((alloc_size (2)))
void *arenaAlloc(Arena *a, uint32_t size) {
	a->used += size;
	if(a->used > a->size)
		return NULL;
	return PTR_ARITH_WARN(a->memory, a->used-size);
}
__attribute__ ((alloc_size (2, 3)))
void *arenaCalloc(Arena *a, uint32_t n, uint32_t size) {
	if(a->used+n*size > a->size)
		return NULL;
	memset(PTR_ARITH_WARN(a->memory, a->used), 0, size*n);
	a->used += size*n;
	return PTR_ARITH_WARN(a->memory, a->used-size*n);
}
__attribute__ ((alloc_size (3)))
void *arenaRealloc(Arena *arena, void *ptr, uint32_t size) {
	arenaFree(arena, ptr, size);
	return arenaAlloc(arena, size);
}

