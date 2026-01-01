#ifndef _NTR_S_LOG
#define _NTR_S_LOG 1
#include <stdarg.h>

enum LogLevel {
	L_ALL = 0,
	L_DEBUG,
	L_INFO,
	L_WARN,
	L_ERR,
	L_NONE
};

#ifndef LOGFILE_NAME
# define LOGFILE_NAME "logs.txt"
#endif

#ifndef NO_FUNC
	#define LOG(LVL, MSG) __log_(LVL, __FILE__, __func__, __LINE__, MSG)
	#define LOGF(LVL, ...) __log_f_(LVL, __FILE__, __func__, __LINE__, __VA_ARGS__)
#else
	#define LOG(LVL, MSG) __log_(LVL, __FILE__, __LINE__, MSG)
	#define LOGF(LVL, ...) __logf_(LVL, __FILE__, __LINE__, __VA_ARGS__)
#endif

void __log_f_(enum LogLevel level, const char *file,
#ifndef NO_FUNC
		const char *func,
#endif
		int line, const char *format, ...) 
#ifndef NO_FUNC
	__attribute__ ((format (printf, 5, 6)));
#else
	__attribute__ ((format (printf, 4, 5)));
#endif

void __log_(enum LogLevel level, const char *file,
#ifndef NO_FUNC
		const char *func,
#endif
		int line, const char *msg);

void loginit	(enum LogLevel stdoutMask, enum LogLevel fileMask);
void logdestroy	();

#endif //_NTR_S_LOG

