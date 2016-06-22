
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

#define worker_num  10
int main(int argv, char **args){
    
    int listen_sock = create_listen_socket("0.0.0.0", 7878);
    
    int i = 0;
    pid_t *worker_list = (pid_t *)malloc(sizeof(pid_t) * worker_num)
    // TODO. worker  pool
    for (; i < worker_num; ++i) {
        worker_list[i] = p;
    }
    pid_t p = fork() 
}
