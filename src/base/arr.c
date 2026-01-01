#include "alloc.h"
#include "global.h"
#include "arr.h"

#include "error.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// default compare function
static bool defaultCmp(void *B, void *a) {
	return B==a;
}

/** pointer to the Nth element of void *M with members of size MSZ 
 * M + (N*MSZ) */
#define NTH(M, N, MSZ) PTR_ARITH_WARN(M, (N)*(MSZ)
#define NTH_A(A, N) PTR_ARITH_WARN((A).members, (N)*(A).mem_sz)
#define NTH_AP(A, N) PTR_ARITH_WARN((A)->members, (N)*(A)->mem_sz)

// Option: complex Array_t
Option _array_init(Allocator a, size_t mem_sz, size_t def_sz) {
	Option ret = {0};
	Array_t arr = {
		.allocator = a,
		.mem_sz = mem_sz,
		.size = def_sz,
		.used = 0,
	};
	arr.members = a.alloc(&a.data, def_sz * mem_sz);
	if(!arr.members)
		return OPT_WRAP_PRIM_ERR("alloc failed");
	arr.cmp = defaultCmp;
	arr.isValid=true;
	//array_print(arr);
	ret = OPT_WRAP_COMPLEX_OK(&arr, Array_t);
	//array_print(*((Array_t*)ret.ret));
	return ret;
}
void array_destroy(Array_t *arr) {
	free(arr->members);
	*arr = (Array_t){0};
}

void array_set_cmp_fn(Array_t *arr, ArrayCmp fn) {
	arr->cmp = fn;
}

// Option: void * (prim)
Option array_index(Array_t arr, size_t n) {
	if(n >= arr.used)
		return OPT_WRAP_PRIM_ERR("n >= arr.used");
	return OPT_WRAP_PRIM_OK(NTH_A(arr, n));
}

Option array_last(Array_t arr) {
	if(arr.used == 0)
		return OPT_WRAP_PRIM_ERR("array is empty");
	return array_index(arr, arr.used-1);
}
Option array_lastptr(Array_t arr) {
	return OPT_WRAP_PRIM_OK(NTH_A(arr, arr.used-1));
}

Error array_resize(Array_t *arr, size_t nsz) {
	if(nsz < arr->used)
		return ERROR_FAIL;
	if(nsz == arr->size)
		return ERROR_SUCC;
	arr->members = arr->allocator.realloc(&arr->allocator.data, arr->members, nsz*arr->mem_sz);
	arr->size = nsz;
	if(!arr->members)
		return ERROR_FAIL;
	return ERROR_SUCC;
}

Error array_push_ptr(Array_t *restrict arr, const void *restrict data) {
	if(!arr || !data || arr->used >= arr->size) 
		if(IS_ERROR(array_resize2(arr))) 
			return ERROR_FAIL;
	memcpy(NTH_AP(arr, arr->used), data, arr->mem_sz);
	arr->used++;
	return ERROR_SUCC;
}

Error array_add_ptr(Array_t *arr, size_t n, void *data) {
	if(n > arr->used)
		return ERROR_FAIL;
	if(n == arr->used)
		return array_push_ptr(arr, data);
	if(arr->used >= arr->size) 
		if(IS_ERROR(array_resize2(arr))) 
			return ERROR_FAIL;

	void *old = NTH_AP(arr, n);
	void *new = NTH_AP(arr, n+1);

	memmove(new,
		old,
	      	(arr->used - n)*arr->mem_sz);
	memmove(NTH_AP(arr, n), data, arr->mem_sz);
	arr->used++;
	return ERROR_SUCC;
}

Error array_append(Array_t *dest, Array_t other) {
	if(dest->mem_sz != other.mem_sz)
		return ERROR_FAIL;
	if(dest->used + other.used >= dest->size) 
		if(IS_ERROR(array_resize(dest, dest->size + other.used + 2))) 
			return ERROR_FAIL;

	memmove(PTR_ARITH_WARN(dest->members, dest->used*dest->mem_sz),
		other.members, 
		other.used * other.mem_sz);
	dest->used += other.used;

	return ERROR_SUCC;
}
Error array_append_simple(Array_t *dest, void *other, size_t count) {
	if(!dest || !other)
		return ERROR_FAIL;
	if(dest->used+count > dest->size)
		if(IS_ERROR(array_resize(dest, dest->size + count + 2))) 
			return ERROR_FAIL;

	memmove(PTR_ARITH_WARN(dest->members, dest->used*dest->mem_sz),
		other, 
		count * dest->mem_sz);
	dest->used += count;

	return ERROR_SUCC;
}

Option array_pop(Array_t *arr) {
	if(arr->used == 0)
		return OPT_WRAP_PRIM_ERR("empty");
	Option last = array_last(*arr);
	arr->used--;
	return last;
}
Error array_pops(Array_t *arr, size_t n) {
	for(size_t i = 0; i < n; i++)
		if(IS_ERROR_OPT(array_pop(arr)))
			return ERROR_FAIL;
	return ERROR_SUCC;
}

Error array_remove(Array_t *arr, size_t n) {
	if(n == arr->used - 1)
		return array_pop(arr).flags | OT_SIMPLE;

	void *old = NTH_AP(arr, n);
	void *new = NTH_AP(arr, n+1);

	memmove(old,new,
		(arr->used - n - 1)*arr->mem_sz);
	arr->used--;
	return ERROR_SUCC;
}

Error array_remove_swapback(Array_t *arr, unsigned n) {
	if(n == arr->used - 1) {
		array_pop(arr);
        return ERROR_SUCC;
    }
	memcpy(NTH_AP(arr, n), NTH_AP(arr, arr->used), arr->mem_sz);
	arr->used--;
	return ERROR_SUCC;
}

Error array_remove_first(Array_t *arr, ArrayCondIter iter) {
	for(size_t i = 0; i < arr->used; i++)
		if(iter(NTH_AP(arr, i)))
			return ERROR_SUCC;
	return ERROR_FAIL;
}

// Option: int
Option array_remove_if(Array_t *arr, ArrayCondIter iter) {
	int count = 0;
	for(size_t i = 0; i < arr->used; i++)
		if(iter(NTH_AP(arr, i)))
			count++;
	if(!count)
		return OPT_WRAP_PRIM_ERR("no accurance found");
	return OPT_WRAP_PRIM_OK(count);
}

Error array_null(Array_t *arr, size_t n) {
	/*Entity *dest = NTH_AP(arr, n);
	printf("Nulling %d\n", dest->_moveID);*/
	memset(NTH_AP(arr, n), 0, arr->mem_sz);
	return ERROR_SUCC;
}
Error array_zero(Array_t *arr) {
	memset(arr->members, 0, arr->mem_sz*arr->used);
	arr->used = 0;
	return ERROR_SUCC;
}

Error array_clear(Array_t *arr) { 
	arr->used = 0;
	return ERROR_SUCC;
}
Error array_clone(Allocator a, Array_t src, Array_t *dest) {
	Option opt = array_init(a, src.mem_sz, src.used);
	if(IS_ERROR_OPT(opt))
		return opt.flags | OT_SIMPLE;
	else
		UNWRAP_TO_COMPLEX_OPT(opt, dest, Array_t)

	memcpy(dest->members, src.members, src.used*src.mem_sz);
	return ERROR_SUCC;
}

Error array_set(Array_t *arr, size_t n, void *data) {
	if(!arr || !data || arr->used <= n)
		return ERROR_FAIL;
	memcpy(NTH_AP(arr, n), data, arr->mem_sz);
	return ERROR_SUCC;
}

// Option: size_t
Option array_index_of(Array_t arr, void *data) {
	for(size_t i = 0; i < arr.used; i++)
		if(arr.cmp(NTH_A(arr, i), data))
			return OPT_WRAP_PRIM_OK(i);
	return OPT_WRAP_PRIM_ERR("data not found");
}
// Option: bool
Option array_contains(Array_t arr, void *data) {
	for(size_t i = 0; i < arr.used; i++)
		if(arr.cmp(NTH_A(arr, i), data))
			return OPT_WRAP_PRIM_OK(1);
	return OPT_WRAP_PRIM_ERR("data not found");
}
// Option: bool
Option array_is_empty(Array_t arr) {
	return OPT_WRAP_PRIM_OK(arr.used == 0);
}
// Option: complex Array_t
Option array_sub_array(Allocator a, Array_t arr, size_t from, size_t to) {
	if(to < from || to >= arr.used)
		return OPT_WRAP_PRIM_ERR("invalid");
	const size_t len = to - (from - 1);
	Option opt = _array_init(a,  arr.mem_sz, len);
	Array_t ret;
	if(IS_ERROR_OPT(opt)) 
		return opt;
	else
		UNWRAP_TO_COMPLEX_OPT(opt, &ret, Array_t);

	for(size_t i = 0; i < len; i++)
		unwrap(array_push_ptr(&ret, unwrap(array_get(arr, from+i))));
	return OPT_WRAP_COMPLEX_OK(&ret, Array_t);
}

Error array_for_each(Array_t arr, ArrayForEach fn) {
	for(size_t i = 0; i < arr.used; i++)
		fn(NTH_A(arr, i));
	return ERROR_SUCC;
}
Error array_for_each_indexed(Array_t arr, ArrayForEachIndexed fn) {
	for(size_t i = 0; i < arr.used; i++)
		fn(NTH_A(arr, i), i);
	return ERROR_SUCC;
}

void array_print(Array_t arr) {
	printf("(Array_t(mem_sz=%lu, size=%lu, used=%lu)) [", arr.mem_sz, arr.size, arr.used);
	for(size_t i = 0; i < arr.used; i++) {
		printf("%u", *(unsigned *)NTH_A(arr, i));
		if(i != arr.used-1)
			printf(", ");
	}
	puts("]");
}

void array_debuginfo(Array_t arr) {
	printf("(Array_t(mem_sz=%lu, size=%lu, used=%lu, allocator=(size=%lu, used=%lu))) [", arr.mem_sz, arr.size, arr.used, arr.allocator.data.size, arr.allocator.data.used);
	for(size_t i = 0; i < arr.used; i++) {
		printf("%u", *(unsigned *)NTH_A(arr, i));
		if(i != arr.used-1)
			printf(", ");
	}
	puts("]");
}
