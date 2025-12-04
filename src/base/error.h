#ifndef _NTR_ERR_H_
#define _NTR_ERR_H_ 1
#include <string.h>
#include "global.h"

/* Option: Either error or return value
 * Functions:
 *  - unwrap
 *  - except 
 * ret type should be documented above 
 * implementer function */
typedef struct Option {
	union {
		// if isErr
		char *msg;
		// if !isErr
		void *ret;
	};
	bool isErr;
	bool isntOpt;
} Option;

// Should only be used when there is nothing else to 
// return other than the error
typedef Option Error;

#define ERROR_SUCC ((Error) { .isntOpt = 1, .isErr = 0, .ret = NULL })
#define ERROR_FAIL ((Error) { .isntOpt = 1, .isErr = 1 })

void _unwrap_err(Option ret, const char *restrict file, unsigned line, const char *restrict func);
#define unwrap_err(RET) _unwrap_err(RET, __FILE__, __LINE__, __func__)

/**************
 *  WRAPPING  *
 **************/

// For wrapping primitives
#define Option_WRAP_OK(RET) ((Option) {.isErr = 0, .ret=(N_TO_PTR_WARN(RET)) })
#define Option_WRAP_ERR(MSG) ((Option) {.isErr = 1, .msg=(MSG) })

// For wrapping structs, ..
Option _Option_ComplexWrap(const void *ret, bool isErr, unsigned sz);
#define Option_COMPLEX_WRAP(RET, ERR, TYPE) _Option_ComplexWrap(RET, ERR, sizeof(TYPE))
#define Option_COMPLEX_WRAP_OK(RET, TYPE) _Option_ComplexWrap(RET, false, sizeof(TYPE))
#define Option_COMPLEX_WRAP_ERR(RET, TYPE) _Option_ComplexWrap(RET, true, sizeof(TYPE))

/****************
 *  UNWRAPPING  *
 ****************/

#define UNWRAP_TO(OPT, DEST, TYPE) { Option __Option_unwrapper = OPT; if(__Option_unwrapper.isErr) { LOG(L_ERR, ""); } DEST = *((TYPE *) __Option_unwrapper.ret); }
#define UNWRAP_TO_PTR(OPT, DEST, TYPE) { Option __Option_unwrapper = OPT; if(__Option_unwrapper.isErr) { LOG(L_ERR, ""); } DEST = ((TYPE *) __Option_unwrapper.ret); }

/* WARNING: Do not pass a function to OPT
 * exa:
 * Option opt = array_init(...)
 * Array_t arr;
 * UNWRAP_COMPLEX_(opt, &arr, Array_r) */
#define UNWRAP_TO_COMPLEX_(OPT, DEST, TYPE) { memcpy(DEST, OPT.ret, sizeof(TYPE)); free(OPT.ret); }
/* exa: 
 * Array_t arr;
 * UNWRAP_COMPLEX(array_init(...), &arr, Array_t) */
#define UNWRAP_TO_COMPLEX(FN, DEST, TYPE) { Option __Option_unwrapper = FN; DEST = *((TYPE*)__Option_unwrapper.ret); free(__Option_unwrapper.ret); }


void *_unwrap(Option ret, const char *restrict file, unsigned line, const char *restrict func);
// On error calls LOG(L_ERR, ...), otherwise returns return value
#define unwrap(RET) _unwrap(RET, __FILE__, __LINE__, __func__)

void *_except(Option ret, const char *msg, const char *restrict file, unsigned line, const char *restrict func);
// Same as unwrap, but with additional message on error
#define except(RET, MSG) _except(RET, MSG, __FILE__, __LINE__, __func__)

#endif // !_NTR_ERR_H_

