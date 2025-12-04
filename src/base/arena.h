#ifndef _NTR_ARENA_H_
#define _NTR_ARENA_H_ 1
#include <stdint.h>

typedef struct {
	void *memory;
	uint32_t size;
	uint32_t used;
} Arena;

Arena arenaInit(uint32_t size);
void arenaDestroy(Arena *arena);
void arenaFree(Arena *arena, void *ptr, uint32_t size);
__attribute__ ((alloc_size (2)))
void *arenaAlloc(Arena *arena, uint32_t size);
__attribute__ ((alloc_size (2,3)))
void *arenaCalloc(Arena *arena, uint32_t n, uint32_t size);
__attribute__ ((alloc_size (3)))
void *arenaRealloc(Arena *arena, void *ptr, uint32_t size);

#endif // _NTR_ARENA_H_

