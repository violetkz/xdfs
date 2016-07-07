#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>


#include "xd_log.h"
#include "xd_worker_pool.h"
#include "xd_locker.h"

typedef struct {
    int *count;
    xd_shmlock_t *lock;
    int *raw_count; 
} count_t;

int worker_func(void *param) {
    
    count_t *c =  param;

    xd_shmlock_lock(c->lock);

    *c->count +=1;
    printf("shared mem:%d\n", *(c->count));
    *c->raw_count += 1;
    printf("raw mem:%d\n", *(c->raw_count));

    //sleep(1);
    xd_shmlock_unlock(c->lock);
    return 0;
}

int *make_shared_mem_ptr(){

    int fd = open("/dev/zero", O_RDWR, 0);
    if (fd == -1) {
        xd_err("open /dev/zero failed");
       
        return NULL;
    }

    int *c = mmap(0, sizeof(int), 
            PROT_READ|PROT_WRITE, 
            MAP_SHARED, fd, 0);
    if (c == MAP_FAILED) {
        //        char err_str_buf[err_str_buf_len];
        //        strerror_r(errno, err_str_buf, err_str_buf_len);

        //       xd_err("map failed: %s\n", err_str_buf);
        xd_errno("mmap failed", errno);
    }

    close(fd);

    return c;
}

int main(int argv, char **args){

    int *test_prt = (int*)malloc(sizeof(int)); 
    *test_prt = 0;

//worker_pool_ctx *wp = worker_pool_ctx_new(10);
worker_pool_ctx *wp = worker_pool_ctx_auto_new(10);

    if (wp == NULL) { 
        exit(1);
    }

    count_t  c;
    c.raw_count = test_prt;

    c.count = make_shared_mem_ptr();
    *(c.count) = 0;
    c.lock = xd_shmlock_new();

    worker_pool_ctx_set_action(wp, worker_func, &c);

    worker_pool_start(wp);

    worker_pool_wait(wp);

    worker_pool_ctx_free(wp);
    xd_debug("master process exit");

    xd_shmlock_free(c.lock);
    return 0;
}
