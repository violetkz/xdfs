
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "xd_log.h"
#include "xd_worker_pool.h"


int worker_func(void *param) {
    printf("child: Nothing to doing. ==\n");
    //sleep(1);
    return 0;
}

int main(int argv, char **args){

    //    int listen_sock = create_listen_socket("0.0.0.0", 7878);
    worker_pool_ctx *wp = worker_pool_ctx_new(10);
    if (!wp) { 
        exit(1);
    }
    worker_pool_ctx_set_action(wp, worker_func, NULL);

    worker_pool_start(wp);

    worker_pool_wait(wp);

    worker_pool_ctx_free(wp);
    xd_debug("master process exit");

    return 0;
}
