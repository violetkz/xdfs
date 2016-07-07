#ifndef xd_shm_lock_h
#define xd_shm_lock_h

#include <pthread.h>

typedef struct {
    pthread_mutex_t          *mutex_ptr;
} xd_shmlock_t;

enum {
    xd_shmlock_got = 0,
    xd_shmlock_busy,
    xd_shmlock_err
};

xd_shmlock_t *xd_shmlock_new();
void           xd_shmlock_lock(xd_shmlock_t *lock);
int            xd_shmlock_trylock(xd_shmlock_t *lock);
void           xd_shmlock_unlock(xd_shmlock_t *lock);
void           xd_shmlock_free(xd_shmlock_t *lock);

#if 0
int xd_shm_init(xd_shmlock_t *lock);
int xd_shm_lock(xd_shmlock_t *lock);
int xd_shm_trylock(xd_shmlock_t *lock);
int xd_shm_unlock(xd_shmlock_t *lock);
int xd_shm_destory(xd_shmlock_t *lock);
#endif

#endif
