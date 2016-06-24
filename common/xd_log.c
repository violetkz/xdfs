
#include <stdio.h>
#include "xd_common_conf.h"
#include "xd_log.h"

static log_func global_log_handler = NULL;
static xd_log_level min_log_level  = XD_LOG_LEVEL_DEBUG;

static const char * log_message_tbl[] = {
    "ERROR" ,
    "CRITICAL",
    "WARNING" ,
    "MESSAGE" ,
    "INFO"    ,
    "DEBUG"   ,
    NULL
};

void   log_default_handler(const char   *log_domain,
                           xd_log_level  log_level,
                           const char   *message,
                           void *        unused_data) {

    fprintf(stdout, "-%s- [%-8s]: %s\n", log_domain,
            log_message_tbl[log_level],
            message
           );
}

log_func  log_set_default_handler(log_func log_func_) {
    log_func old = global_log_handler;
    global_log_handler = log_func_;
    return old;
}


void      log_set_log_level(xd_log_level level) {
    
}

void logv(const char    *log_domain,
          xd_log_level     log_level,
          const char    *format,
          va_list       args,
          void*         data) {

    char buf[XD_LOG_BUFF_LEN];
    vsnprintf(buf,XD_LOG_BUFF_LEN, format, args);

    if (min_log_level > log_level) {
        return; // do nothing 
    }

    if (global_log_handler) {
        (*global_log_handler)(log_domain, log_level, buf, data);
    } else {
        log_default_handler(log_domain, log_level, buf, NULL);
    }
}

void simple_log(const char* domain, xd_log_level level, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    logv(domain, level, fmt, ap, NULL);
    va_end(ap);
}
