#ifndef __xd_worker_pool_h
#define __xd_worker_pool_h


/* worker process pool context information*/
typedef struct {
    pid_t  *worker_pid_list;        /* save the all worker process pid */ 
    int     max_worker_num;         /* max worker process number */
    int    (*action)(void *);       /* the function pointer of worker action */
    void   *action_param;           /* worker action functon paramter */
} worker_pool_ctx;


worker_pool_ctx *worker_pool_ctx_auto_new();
worker_pool_ctx *worker_pool_ctx_new(int max_worker_num);

void worker_pool_ctx_free(worker_pool_ctx *ctx); 

void worker_pool_ctx_set_action(worker_pool_ctx *ctx, 
                         int (*func)(void *), 
                         void *param); 


int worker_pool_start(worker_pool_ctx *ctx); 

void worker_pool_wait(worker_pool_ctx *ctx);

void worker_pool_kill(worker_pool_ctx *ctx);

#endif
