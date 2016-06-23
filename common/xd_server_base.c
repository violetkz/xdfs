
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#if 0
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#endif


/* 
 * 1. socket
 * 2. bind 
 * 3. fork
 * 4. try to lock 
 * 5. accept
 * 6. event loop
 * 7. exit
 */

#if 0
int  create_listen_socket(char *bind_addr, int port);

int  create_listen_socket(char *bind_addr, int port) {

}
#endif


typedef struct {
    pid_t  *worker_pid_list; 
    int     max_worker_num;
    int    (*action)(void *param);
    void   *action_param;
} worker_processes_ctx;

worker_processes_ctx *worker_processes_ctx_new(int max_worker_num) {

    worker_processes_ctx *new_ctx = malloc(sizeof(worker_processes_ctx));
    
    if (new_ctx) {
        memset(new_ctx, 0x00, sizeof(worker_processes_ctx));
        new_ctx->max_worker_num = max_worker_num;
        if (max_worker_num > 0) {
            new_ctx->worker_pid_list = malloc(sizeof(pid_t) * max_worker_num);
        }
    }

    return new_ctx;
}

void worker_processes_ctx_free(worker_processes_ctx *ctx) {
    if (ctx) {
        free(ctx->worker_pid_list);
        free(ctx);
    }
}

void worker_action_setup(worker_processes_ctx *ctx, 
                         int (*func)(void *), 
                         void *param) {
    if (ctx) {
        ctx->action = func;
        ctx->action_param = param;
    }
}


int create_worker_proecess(worker_processes_ctx *ctx) {
    
    int rc = 0;
    int is_in_child_process = 0;

    if (ctx) {
        int i =0;
        for (; i < ctx->max_worker_num; ++i) {
            pid_t p = fork();
            if (p > 0) {
                ctx->worker_pid_list[i] = p;
            }
            else if (p == 0) {
                is_in_child_process = 1;
                break;  //break out `for` loop
            }
            else {
                rc = -1;            
                perror("fork failed");
            }
        }

        if (is_in_child_process) {
            ctx->action(ctx->action_param);
        }
        else {
            printf("is in master\n");
        }
    }
    
    return rc;
}

int worker_func(void *param) {
    printf("Nothing to doing. ==\n");
    return 0;
}

int main(int argv, char **args){
    
//    int listen_sock = create_listen_socket("0.0.0.0", 7878);

    worker_processes_ctx *wp = worker_processes_ctx_new(10);
    worker_action_setup(wp, worker_func, NULL);
    
    create_worker_proecess(wp);

    wait(NULL); 
}
