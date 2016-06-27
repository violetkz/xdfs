

#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include "xd_common_conf.h"
#include "xd_locker.h"
#include "xd_log.h"


int xd_shm_init(xd_shm_lock_t *lock) {

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

    int rc = pthread_mutex_init(lock->mutex_ptr, &mutex_attr) != 0;
    if (!rc) {
#define err_str_buf_len  256
        char err_str_buf[err_str_buf_len]; 
        //fixme: errno ? rc?
        xd_err("pthread_mutex_init failed: %s", 
               strerror_r(rc, err_str_buf, err_str_buf_len));
                return XD_RET_ERR;
    }

    return XD_RET_OK; 
}

int xd_shm_lock(xd_shm_lock_t *lock) {
   
    int ret = XD_RET_ERR;
    if (lock && lock->mutex_ptr) {
        ret = pthread_mutex_lock(lock->mutex_ptr);
    }
    
    return ret;
}

int xd_shm_trylock(xd_shm_lock_t *lock) {
    
    //fixme? return code?
    if (lock && lock->mutex_ptr) {
        int ret = pthread_mutex_trylock(lock->mutex_ptr);
        if (ret == 0) {
            return xd_shm_lock_got;
        }
        if (ret == EBUSY) {
            return xd_shm_lock_busy;     
        }
    }
    
    return xd_shm_lock_err;
}

int xd_shm_unlock(xd_shm_lock_t *lock) {
    
    int ret = XD_RET_ERR;
    if (lock && lock->mutex_ptr) {
        ret = pthread_mutex_unlock(lock->mutex_ptr);
    }
    
    return ret;
}

int xd_shm_destory(xd_shm_lock_t *lock) {
    
    int ret = XD_RET_ERR;
    if (lock && lock->mutex_ptr) {
        ret = pthread_mutex_destroy(lock->mutex_ptr);
    }
    
    return ret;
}
