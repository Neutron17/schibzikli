#ifndef _NTR_ERR_H_
#define _NTR_ERR_H_ 1
#include <string.h>
#include "global.h"
#include "log.h"

/** How should I decide what type to use?
 * - no return value, only error/success -> Error:
 *   	OT_SIMPLE_SUCC or OT_SIMPLE_ERROR 
 *   	(ERROR_SUCC/ERROR_FAIL)
 * - primitive return type (char, long, pointers) -> Option:
 *   	OT_FULL_PRIM_SUCC or OT_FULL_PRIM_ERROR
 * - a struct/union -> Option:
 *   	OT_FULL_SUCC or OT_FULL_ERROR  
 *
 * void -> Error
 * some type -> Option */
enum OptionType {
	OT_SUCC,
	OT_ERROR 	= 1<<0,

	OT_SIMPLE 	= 1<<1,
	OT_FULL 	= 0<<1,

	OT_PRIMITIVE 	= 1<<2, 

	/** ONLY FOR ASSIGNMENT, bitwise AND won't work */
	OT_SIMPLE_SUCC 		= OT_SUCC | OT_SIMPLE,
	OT_SIMPLE_ERROR 	= OT_ERROR| OT_SIMPLE,
	OT_FULL_SUCC 		= OT_SUCC | OT_FULL,
	OT_FULL_ERROR 		= OT_ERROR| OT_FULL,
	// WARNING: Incomplete
	OT_FULL_PRIM 		= OT_FULL | OT_PRIMITIVE, 
	OT_FULL_PRIM_SUCC 	= OT_SUCC | OT_FULL 	 | OT_PRIMITIVE,
	OT_FULL_PRIM_ERROR 	= OT_ERROR| OT_FULL 	 | OT_PRIMITIVE
};

/* Option: Either error or return value
 * Functions:
 *  - unwrap
 *  - except 
 * ret type should be documented above 
 * the implementer function */
typedef struct Option {
	size_t flags; // OptionTypes or-ed together
	union {
		// if isErr
		char *msg;
		// if !isErr
		void *ret;
	};
} Option;

// Should only be used when there is nothing else to 
// return other than the error (void)
// 	can be casted to Option, and passed 
// 	to wrap/unwrap
typedef unsigned Error; 

#define ERROR_SUCC (OT_SIMPLE_SUCC)
#define ERROR_FAIL (OT_SIMPLE_ERROR)

#define IS_ERROR(U) (U & OT_ERROR)
#define IS_ERROR_OPT(O) ((O).flags & OT_ERROR)

#define TO_OPT(E) ((Option) {.flags=(E),.ret=NULL})

/**************
 *  WRAPPING  *
 **************/

/** For wrapping primitives, for values up to 8bytes, 
 * this method doesn't use malloc, it casts the data to 
 * the union pointer */
#define OPT_WRAP_PRIM_OK(RET) ((Option) {.flags = OT_FULL_PRIM_SUCC, .ret=(N_TO_PTR_WARN(RET)) })
/** See OPT_PRIM_WRAP_OK */
#define OPT_WRAP_PRIM_ERR(MSG) ((Option) {.flags = OT_FULL_PRIM_ERROR, .msg=(MSG) })

// For wrapping structs, ..
Option _Option_ComplexWrap(const void *ret, bool isErr, unsigned sz);
#define OPT_WRAP_COMPLEX(RET, ERR, TYPE) _Option_ComplexWrap(RET, ERR, sizeof(TYPE))
#define OPT_WRAP_COMPLEX_OK(RET, TYPE) _Option_ComplexWrap(RET, false, sizeof(TYPE))
#define OPT_WRAP_COMPLEX_ERR(RET, TYPE) _Option_ComplexWrap(RET, true, sizeof(TYPE))

/****************
 *  UNWRAPPING  *
 ****************/

/** for primitive unwraping */
#define UNWRAP_TO(OPT, DEST, TYPE) { Option __Option_unwrapper = OPT; if(__Option_unwrapper.flags & OT_ERROR) { LOGF(L_ERR, "UNWRAP ERROR: %s\n", __Option_unwrapper.msg); } DEST = *((TYPE *) __Option_unwrapper.ret); }
#define UNWRAP_TO_PTR(OPT, DEST, TYPE) { Option __Option_unwrapper = OPT; if(__Option_unwrapper.flags & OT_ERROR) { LOGF(L_ERR, "UNWRAP ERROR: %s\n", __Option_unwrapper.msg); } DEST = ((TYPE *) __Option_unwrapper.ret); }

/* WARNING: Do not pass a function to OPT
 * exa:
 * Option opt = array_init(...)
 * Array_t arr;
 * UNWRAP_TO_COMPLEX_OPT(opt, &arr, Array_r) */
#define UNWRAP_TO_COMPLEX_OPT(OPT, DEST, TYPE) { memcpy(DEST, OPT.ret, sizeof(TYPE)); free(OPT.ret); }
/* exa: 
 * Array_t arr;
 * UNWRAP_COMPLEX_FN(array_init(...), &arr, Array_r) */
#define UNWRAP_TO_COMPLEX_FN(FN, DEST, TYPE) { Option __Option_unwrapper = FN; DEST = *((TYPE*)__Option_unwrapper.ret); free(__Option_unwrapper.ret); }


void *_unwrap(const Error *restrict ret, const char *restrict file, unsigned line, const char *restrict func);
// On error calls LOG(L_ERR, ...), otherwise returns return value
#define unwrap(RET) (__extension__({ const __typeof__(RET) __unwrap_err_ptr = (RET); _unwrap((Error *)&__unwrap_err_ptr, __FILE__, __LINE__, __func__); }))

void *_except(const Error *restrict ret, const char *msg, const char *restrict file, unsigned line, const char *restrict func);
// Same as unwrap, but with additional message on error
#define except(RET, MSG) (__extension__({ const __typeof__(RET) __unwrap_err_ptr = (RET); _except((Error *)&__unwrap_err_ptr, MSG, __FILE__, __LINE__, __func__); }))

#endif // !_NTR_ERR_H_

