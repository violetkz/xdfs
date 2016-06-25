#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "xd_worker_pool.h"
#include "xd_log.h"

worker_pool_ctx *worker_pool_ctx_new(int max_worker_num) {

    worker_pool_ctx *new_ctx = malloc(sizeof(worker_pool_ctx));
    
    if (new_ctx) {
        memset(new_ctx, 0x00, sizeof(worker_pool_ctx));
        new_ctx->max_worker_num = max_worker_num;
        if (max_worker_num > 0) {
            new_ctx->worker_pid_list = malloc(sizeof(pid_t) * max_worker_num);
        }
    }

    return new_ctx;
}

void worker_pool_ctx_free(worker_pool_ctx *ctx) {
    if (ctx) {
        free(ctx->worker_pid_list);
        free(ctx);
    }
}

void worker_pool_ctx_set_action(worker_pool_ctx *ctx, 
                         int (*func)(void *), 
                         void *param) {
    if (ctx) {
        ctx->action = func;
        ctx->action_param = param;
    }
}


int start_work_pool(worker_pool_ctx *ctx) {
    
    //int rc = 0;
    int is_in_master_process = 1;

    if (ctx) {
        int i =0;
        for (; i < ctx->max_worker_num; ++i) {
            pid_t p = fork();
            if (p > 0) {
                ctx->worker_pid_list[i] = p;
            }
            else if (p == 0) {
                is_in_master_process = 0;
                break;  //break out `for` loop
            }
            else {
                is_in_master_process = -1;            
                xd_err("fork failed :%d", is_in_master_process);
            }
        }

        if (! is_in_master_process) {
            ctx->action(ctx->action_param);
        }
        else {
            xd_debug("is in master\n");
        }
    }
    
    return is_in_master_process;
}

int is_worker_process(worker_pool_ctx *ctx, pid_t p) {
    int rc = 0;
    if (ctx) {
        int i = 0;
        for (; i < ctx->max_worker_num; ++i) {
            if (p == ctx->worker_pid_list[i]) {
                rc = 1;
                break;
            }
        }
    }
    return rc;
}

void wait_for_worker_pool(worker_pool_ctx *ctx) {
    
    int status; 
    if (ctx) {
        int count =  ctx->max_worker_num;
        while (count > 0) {
            /* the 1st paramter of waitpid, -1 means wait for any child process. */
            pid_t p = waitpid(-1, &status, 0);
             
            if (p > 0) {
                if (is_worker_process(ctx, p)) {
                    
                    int ret; 
                    if (WIFEXITED(status))        {
                        ret = WEXITSTATUS(status); 
                        xd_debug("child process %d exited with return code %d\n", p, ret);
                    }
                    //todo, handle exit status. 
                    else if (WIFSIGNALED(status)) { ret = -2; }
                    else if (WIFSTOPPED(status))  { ret = -3; }
                    else                          { ret = -4; }

                    --count;
                }
            }
            else {
                xd_err("func wait failed");
            }
        }
    }
}