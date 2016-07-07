
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

int xd_init_event(const char *ip, int port) {
	struct event_base *base;
	struct evhttp *http;
	struct evhttp_bound_socket *handle;

	unsigned short port = 0;
#ifdef WIN32
	WSADATA WSAData;
	WSAStartup(0x101, &WSAData);
#else
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		return (1);
#endif
	if (argc < 2) {
		syntax();
		return 1;
	}

	base = event_base_new();
	if (!base) {
		fprintf(stderr, "Couldn't create an event_base: exiting\n");
		return 1;
	}

	/* Create a new evhttp object to handle requests. */
	http = evhttp_new(base);
	if (!http) {
		fprintf(stderr, "couldn't create evhttp. Exiting.\n");
		return 1;
	}
}

int main(int argv, char **args){

    worker_pool_ctx *wp = worker_pool_ctx_new(10);

    if (wp == NULL) { 
        exit(1);
    }

    count_t  c;

    c.count = test();
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
