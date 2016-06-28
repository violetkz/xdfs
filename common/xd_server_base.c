
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
    xd_shm_lock_t lock;
} count_t;
int worker_func(void *param) {
    
    count_t *c =  param;
    
    xd_shm_lock(&(c->lock));
    *c->count +=1;
    printf("%d\n", *(c->count));
    //sleep(1);
    xd_shm_unlock(&(c->lock));
    return 0;
}

int * test(){

    int fd = open("/dev/zero", O_RDWR, 0);
    if (fd == -1) {
        xd_err("open /dev/zero failed");
        return NULL;
    }

    int *count = mmap(0, sizeof(int), 
                           PROT_READ|PROT_WRITE, 
                           MAP_SHARED, fd, 0);

    close(fd);
    return count;
}


int main(int argv, char **args){

    //    int listen_sock = create_listen_socket("0.0.0.0", 7878);
    worker_pool_ctx *wp = worker_pool_ctx_new(10);
    if (!wp) { 
        exit(1);
    }

    count_t  c;
    c.count = test();
    *(c.count) = 0;
    xd_shm_init(&c.lock);
    
    worker_pool_ctx_set_action(wp, worker_func, &c);

    worker_pool_start(wp);

    worker_pool_wait(wp);

    worker_pool_ctx_free(wp);
    xd_debug("master process exit");

    xd_shm_destory(&c.lock);
    return 0;
}
