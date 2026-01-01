#include "global.h"
#include "error.h"

#include "log.h"
#include <stdlib.h>

#define IS_SIMPLE(O) ((O).flags & OT_SIMPLE)

/* Dynamic variable problem
 * - Dynamic memory allocation is slow
 * - You can't cast a normal variable to a void *
 * - You shouldn't return a pointer to a local variable
 * - You have to dynamicaly allocate and that is slow 	*/

// WARNING: if is actual(not Error) Option -> needs to be freed
void *_unwrap(const Error *restrict ret, const char *restrict file, unsigned line, const char *restrict func) {
	if(IS_ERROR(*ret)) {
		LOGF(L_ERR, "ERROR: %s, from %s/%s, line: %u\n", 
			(*ret & OT_FULL) ? (*(Option *)ret).msg : "", file, func, line);
	}
	return (*(Option *)ret).ret;
}

// WARNING: if Option is full -> needs to be freed
void *_except(const Error *restrict ret, const char *msg, const char *restrict file, unsigned line, const char *restrict func) {
	if(IS_ERROR(*ret)) {
		LOGF(L_ERR, "ERROR: %s: %s, from %s/%s, line: %u\n", 
			msg, (*ret & OT_FULL) ? (*(Option *)ret).msg : "", file, func, line);
	}
	return (*(Option *)ret).ret;
}

Option _Option_ComplexWrap(const void *ret, bool isErr, unsigned sz) {
	Option r = { 
		.flags = OT_FULL | (isErr ? OT_ERROR : OT_SUCC),
		.ret = malloc(sz)
	};

	if(!r.ret) {
		LOGF(L_ERR, "COMPLEX_UNWRAP ERROR: malloc failed\n");
		return (Option) { .flags=OT_SIMPLE_ERROR };
	}
	memcpy(r.ret, ret, sz);
	return r;
}

