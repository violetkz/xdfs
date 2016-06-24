
#ifndef log_hpp_______
#define log_hpp_______

#include <stdarg.h>
#include "xd_common_conf.h"

/*
 * log levels and flags.
 */


/* log levels */
typedef enum {
  XD_LOG_LEVEL_ERROR    = 0,       /* always fatal */
  XD_LOG_LEVEL_CRITICAL,
  XD_LOG_LEVEL_WARNING ,
  XD_LOG_LEVEL_MESSAGE , 
  XD_LOG_LEVEL_INFO    ,
  XD_LOG_LEVEL_DEBUG   ,
} xd_log_level;


/* define log func  */
typedef void (*log_func)(const char *log_domain, 
                         xd_log_level level, 
                         const char *message,
                         void *arg);

/* the default log handler. it print the message to stdout */
void   log_default_handler(const char    *log_domain,
                           xd_log_level  log_level,
                           const char    *message,
                           void *        unused_data);

/* set log handler */
log_func  log_set_default_handler(log_func func);

/* set min log level */
void      log_set_log_level(xd_log_level level);

/* base loging func */
void logv(const char    *log_domain,
          xd_log_level     log_level,
          const char    *format,
          va_list       args,
          void*         data);

/* simple log func  */
void simple_log(const char *domain, xd_log_level level, const char* fmt, ...); 


/* some useful macroes for logging */

#define xd_debug(FMT, ...) simple_log(XD_LOG_STR, XD_LOG_LEVEL_DEBUG, FMT, __VA_ARGS__)
#define xd_info(FMT, ...)  simple_log(XD_LOG_STR, XD_LOG_LEVEL_INFO, FMT, __VA_ARGS__)
#define xd_warn(FMT, ...)  simple_log(XD_LOG_STR, XD_LOG_LEVEL_WARNING, FMT, __VA_ARGS__)
#define xd_err(FMT, ...)   simple_log(XD_LOG_STR, XD_LOG_LEVEL_ERROR, FMT, __VA_ARGS__)

#define xd_fn_trace_s       simple_log(XD_LOG_STR, XD_LOG_LEVEL_DEBUG, "[func: %s --->]", __func__) 
#define xd_fn_trace_e       simple_log(XD_LOG_STR, XD_LOG_LEVEL_DEBUG, "[func: %s <---]", __func__) 

#endif
