
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

#include "xd_log.h"
#include "xd_worker_pool.h"
#include "xd_locker.h"

typedef struct {
    int *count;
    xd_shmlock_t *lock;
} count_t;

int worker_func(void *param) {
    
    count_t *c =  param;
    
    xd_shmlock_lock(c->lock);
    *c->count +=1;
    printf("%d\n", *(c->count));
    //sleep(1);
    xd_shmlock_unlock(c->lock);
    return 0;
}

int *test(){

#if defined(__linux__)
    int fd = open("/dev/zero", O_RDWR, 0);
    if (fd == -1) {
        xd_err("open /dev/zero failed");
        return NULL;
    }
#elif defined(__APPLE__)
    const char *lock_file_name = "/var/tmp/xd.lock";
    int fd = shm_open(lock_file_name, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
    ftruncate(fd, sizeof(int));
#else
    #error "un-supported OS"
#endif

    printf("xx0\n");
    int *c = mmap(0, sizeof(int), 
                           PROT_READ|PROT_WRITE, 
                           MAP_SHARED, fd, 0);
    printf("xx: %p", c);
    if (c == MAP_FAILED) {
        char err_str_buf[err_str_buf_len];
        strerror_r(errno, err_str_buf, err_str_buf_len);
        
        xd_err("map failed: %s\n", err_str_buf);
    }

    printf("xx: %p", c);

#if defined(__liunx__)
    close(fd);
#elif defined(__APPLE__)
    shm_unlink(lock_file_name);
#endif

    return c;
}


int main(int argv, char **args){

    worker_pool_ctx *wp = worker_pool_ctx_new(10);

    if (wp == NULL) { 
        exit(1);
    }

    count_t  c;

    printf("0\n");
    c.count = test();
    *(c.count) = 0;
    printf("1\n");
    c.lock = xd_shmlock_new();
    
    printf("2\n");
    worker_pool_ctx_set_action(wp, worker_func, &c);

    worker_pool_start(wp);

    worker_pool_wait(wp);

    worker_pool_ctx_free(wp);
    xd_debug("master process exit");

    xd_shmlock_free(c.lock);
    return 0;
}
