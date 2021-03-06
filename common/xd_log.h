
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

#define xd_debug(...) simple_log(XD_LOG_STR, XD_LOG_LEVEL_DEBUG, __VA_ARGS__)
#define xd_info(...)  simple_log(XD_LOG_STR, XD_LOG_LEVEL_INFO,  __VA_ARGS__)
#define xd_warn(...)  simple_log(XD_LOG_STR, XD_LOG_LEVEL_WARNING,__VA_ARGS__)
#define xd_err(...)   simple_log(XD_LOG_STR, XD_LOG_LEVEL_ERROR,__VA_ARGS__)

#define XD_LOG_INTERNAL_BUF_LEN 512
#define xd_errno(msg,errno_) {                                  \
    char err_str_buf[XD_LOG_INTERNAL_BUF_LEN];                  \
    strerror_r((errno_), err_str_buf, XD_LOG_INTERNAL_BUF_LEN); \
    simple_log(XD_LOG_STR, XD_LOG_LEVEL_ERROR, "%s : %s", (msg), err_str_buf); \
}

#endif
