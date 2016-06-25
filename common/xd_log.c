
#include <stdio.h>
#include <time.h>
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

char *make_time_tag(char *buf, size_t buf_len) {

    time_t now_time = time(NULL);
    struct tm tm_time;
#ifndef _WIN32
    localtime_r(&now_time, &tm_time);
#else
    localtime_s(&tm_time, &now_time);
#endif

    snprintf(buf, buf_len, "%d-%02d-%02d %02d:%02d:%02d",
            1900+tm_time.tm_year,
            1+tm_time.tm_mon,
            tm_time.tm_mday,
            tm_time.tm_hour,
            tm_time.tm_min,
            tm_time.tm_sec);

    return buf;
}

void   log_default_handler(const char   *log_domain,
                           xd_log_level  log_level,
                           const char   *message,
                           void *        unused_data) {

    char buf[128];
    

    fprintf(stdout, "%s[%s]-%s: %s\n",make_time_tag(buf, 128),
            log_message_tbl[log_level],
            log_domain,
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
