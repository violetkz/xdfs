#ifndef xd_shm_lock_h
#define xd_shm_lock_h

#include <pthread.h>

typedef struct {
//    pthread_mutexattr_t      *mutex_attr_ptr;
    pthread_mutex_t          *mutex_ptr;
} xd_shm_lock_t;

enum {
    xd_shm_lock_got = 0,
    xd_shm_lock_busy,
    xd_shm_lock_err
};


int xd_shm_init(xd_shm_lock_t *lock);
int xd_shm_lock(xd_shm_lock_t *lock);
int xd_shm_trylock(xd_shm_lock_t *lock);
int xd_shm_unlock(xd_shm_lock_t *lock);
int xd_shm_destory(xd_shm_lock_t *lock);

#endif
