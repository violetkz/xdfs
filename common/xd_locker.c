

#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "xd_common_conf.h"
#include "xd_locker.h"
#include "xd_log.h"



xd_shmlock_t *xd_shmlock_new() {

    xd_shmlock_t *new_lock = malloc(sizeof(xd_shmlock_t));

    if (new_lock == NULL) 
        return NULL;
    
    int fd = open("/dev/zero", O_RDWR, 0);
    if (fd == -1) {
        xd_err("open /dev/zero failed");
        return NULL;
    }

    new_lock->mutex_ptr = mmap(0, sizeof(pthread_mutex_t), 
                           PROT_READ|PROT_WRITE, 
                           MAP_SHARED, fd, 0);

    close(fd);

    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);

    int rc = pthread_mutex_init(new_lock->mutex_ptr, &mutex_attr);

    if (rc != 0) {
        char err_str_buf[err_str_buf_len]; 
        strerror_r(rc, err_str_buf, err_str_buf_len);
        xd_err("pthread_mutex_init failed: %s",  err_str_buf);
        return NULL;
    }

    return new_lock; 
}
void  xd_shmlock_lock(xd_shmlock_t *lock) {

    if (lock && lock->mutex_ptr) {
        int ret = pthread_mutex_lock(lock->mutex_ptr);
        xd_debug("%s: %d", __func__, ret);
    }
    else { xd_err("invaild lock context"); }
}

int  xd_shmlock_trylock(xd_shmlock_t *lock) {
    
    if (lock && lock->mutex_ptr) {
        int ret = pthread_mutex_trylock(lock->mutex_ptr);
        if (ret == 0) {
            return xd_shmlock_got;
        }
        if (ret == EBUSY) {
            return xd_shmlock_busy;     
        }
    }
    
    return xd_shmlock_err;
}

void xd_shmlock_unlock(xd_shmlock_t *lock) {

    if (lock && lock->mutex_ptr) {
        pthread_mutex_unlock(lock->mutex_ptr);
    }
    else 
        xd_err("invaild lock context");
}

void xd_shmlock_free(xd_shmlock_t *lock) {
    
    if (lock && lock->mutex_ptr) {
        pthread_mutex_destroy(lock->mutex_ptr);
        
        free(lock);
    }
    else 
        xd_err("invaild lock context");
}

#if 0
int xd_shm_init(xd_shmlock_t *lock) {

    int fd = open("/dev/zero", O_RDWR, 0);
    if (fd == -1) {
        xd_err("open /dev/zero failed");
        return XD_RET_ERR;
    }

    lock->mutex_ptr = mmap(0, sizeof(pthread_mutex_t), 
                           PROT_READ|PROT_WRITE, 
                           MAP_SHARED, fd, 0);

    close(fd);

    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);

    int rc = pthread_mutex_init(lock->mutex_ptr, &mutex_attr);
    if (rc != 0) {
#define err_str_buf_len  256
        char err_str_buf[err_str_buf_len]; 
        //fixme: errno ? rc?
        strerror_r(rc, err_str_buf, err_str_buf_len);
        xd_err("pthread_mutex_init failed: %s",  err_str_buf);
        return XD_RET_ERR;
    }

    return XD_RET_OK; 
}

int xd_shmlock(xd_shmlock_t *lock) {
   
    int ret = XD_RET_ERR;
    if (lock && lock->mutex_ptr) {
        xd_debug("call lock");
        ret = pthread_mutex_lock(lock->mutex_ptr);
    }
    xd_debug("%s: %d", __func__, ret);
    return ret;
}

int xd_shm_trylock(xd_shmlock_t *lock) {
    
    //fixme? return code?
    if (lock && lock->mutex_ptr) {
        int ret = pthread_mutex_trylock(lock->mutex_ptr);
        if (ret == 0) {
            return xd_shmlock_got;
        }
        if (ret == EBUSY) {
            return xd_shmlock_busy;     
        }
    }
    
    return xd_shmlock_err;
}

int xd_shm_unlock(xd_shmlock_t *lock) {
    
    int ret = XD_RET_ERR;
    if (lock && lock->mutex_ptr) {
        ret = pthread_mutex_unlock(lock->mutex_ptr);
    }
    
    return ret;
}

int xd_shm_destory(xd_shmlock_t *lock) {
    
    int ret = XD_RET_ERR;
    if (lock && lock->mutex_ptr) {
        ret = pthread_mutex_destroy(lock->mutex_ptr);
    }
    
    return ret;
}
#endif
