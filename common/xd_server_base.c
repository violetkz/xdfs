
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>



/* 
 * 1. socket
 * 2. bind 
 * 3. fork
 * 4. try to lock 
 * 5. accept
 * 6. event loop
 * 7. exit
 */

int  create_listen_socket(char *bind_addr, int port);

int  create_listen_socket(char *bind_addr, int port) {

}


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
                         int (*action)(void *param) func, 
                         void *param) {
    if (ctx) {
        ctx->action = func;
        ctx->action_param = param;
    }
}


int create_worker_proecess(worker_processes_ctx *ctx) {
    
    int rc = 0;

    if (ctx) {
        int i =0;
        for (; i < ctx->max_worker_num; ++i) {
            pid_t p = fork();
            if (p > 0) {
                ctx->worker_pid_list[i] = p;
            }
            else if (p == 0) {
                break;  //break out `for` loop
                ctx->action(ctx->action_param);
            }
            else {
                rc = -1;            
            }
        }
    }
    
    return rc;
}

int worker_func(void *param) {
    printf("Nothing to doing. ==\n");
}

int main(int argv, char **args){
    
//    int listen_sock = create_listen_socket("0.0.0.0", 7878);

    worker_processes_ctx *wp = worker_processes_ctx_new(10);
    worker_action_setup(wp, worker_func, NULL);
    
    create_worker_proecess(wp)
    
}
