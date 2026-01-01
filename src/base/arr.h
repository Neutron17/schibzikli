#ifndef _NTR_ARR_H_
#define _NTR_ARR_H_ 1
#include <stddef.h>
#include "error.h"
#include "alloc.h"

/* Function type for comparing array members
 * should return true(1) if a and b are equal */
typedef bool (*ArrayCmp)(void *a, void *b);

typedef bool (*ArrayCondIter)(const void *it);

typedef struct Array {
	Allocator allocator;
	void *members;
	ArrayCmp cmp;

	size_t mem_sz;
	size_t size;
	size_t used;

	bool isValid;
} Array_t;

typedef struct {
	void *members;
	size_t mem_sz;
	size_t size;
	size_t used;
} ArrayPartial;

// [Option: complex Array_t]
Option _array_init(Allocator a, size_t mem_sz, size_t def_sz);
#define array_init(A, TYPE, SZ) _array_init(A,sizeof(TYPE), SZ)
#define array_init2(TYPE, SZ) _array_init(pageAllocator(),sizeof(TYPE), SZ)
void array_destroy(Array_t *array);

void array_set_cmp_fn(Array_t *arr, ArrayCmp fn);

//[Option: void *]
Option array_index(Array_t arr, size_t n);
#define array_at array_index
#define array_nth array_index
#define array_get array_index

/**[Option void *]
 * return the last element of the array */
Option array_last(Array_t arr);
/**[Option void *]
 * return a pointer to the last element of the array */
Option array_lastptr(Array_t arr);
// Resize "arr" to new size "nsz"
Error array_resize(Array_t *arr, size_t nsz);
// resize array to arr.size * 2
#define array_resize2(ARR) array_resize(ARR, ARR->size*2*ARR->mem_sz)

// Add element to the end of the array
Error array_push_ptr(Array_t *restrict arr, const void *restrict data);
#define array_push(A, V) (__extension__ ({__typeof__(V) __arr_push_tmp = (V);array_push_ptr((A),&__arr_push_tmp); }))
// Add element to be the nth element to the array
Error array_add_ptr(Array_t *arr, size_t n, void *data);
#define array_add(A,N,V) (__extension__ ({__typeof__(V) __arr_add_tmp = (V);array_add_ptr((A),(N),&__arr_add_tmp); }))
#define array_insert array_add
#define array_insert_ptr array_add_ptr
// Append "other" to the end of "dest"
Error array_append(Array_t *dest, Array_t other);
Error array_append_simple(Array_t *dest, void *other, size_t count);

/**[Option: void *]
 * remove the last element of "arr" 
 * returns the poped element */
Option array_pop(Array_t *arr);
/** Remove the last "n" elements of "arr" */
Error array_pops(Array_t *arr, size_t n);
/** Remove the "n"th element of "arr" */
Error array_remove(Array_t *arr, size_t n);
/** Remove the "n"th element of "arr" by overwriting it with the last */
Error array_remove_swapback(Array_t *arr, unsigned n);
/** Remove the first element for which "iter" returns true */
Error array_remove_first(Array_t *arr, ArrayCondIter iter);
/**[Option int]
 * Remove all elements for which "iter" returns true
 * returns the number of elements removed */
Option array_remove_if(Array_t *arr, ArrayCondIter iter);
/** Zeros out the value at index n */
Error array_null(Array_t *arr, size_t n);
/** Remove all elements of "arr" */
Error array_clear(Array_t *arr);
/** Copy "src"'s elements to "dest" */
Error array_clone(Allocator alltor, Array_t src, Array_t *dest);
/** Set the "n"th element of "arr" to "data" */
Error array_set(Array_t *arr, size_t n, void *data);

/**[Option: size_t]
 * find the first accurance of "data" in "arr" and return it's index */
Option array_index_of(Array_t arr, void *data);
/**[Option: bool]
 * does "arr" contain "data"? */
Option array_contains(Array_t arr, void *data);
/**[Option: bool]
 * does "arr" have no elements? */
Option array_is_empty(Array_t err);
/**[Option: complex Array_t]
 * make a new array from "arr"'s elements from index "from" until "to", [from; to] */
Option array_sub_array(Allocator alltor, Array_t arr, size_t from, size_t to);

typedef void (*ArrayForEach)(void *data);
typedef void (*ArrayForEachIndexed)(void *data, int index);
/** Execute "fn" on each element of "arr" */
Error array_for_each(Array_t arr, ArrayForEach fn);
/** Execute "fn" on each element of "arr" */
Error array_for_each_indexed(Array_t arr, ArrayForEachIndexed fn);

// Print "arr"
void array_print(Array_t arr);
void array_debuginfo(Array_t arr);

#endif // !_NTR_ARR_H_

