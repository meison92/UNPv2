/* include tryrdlock */
#include	"unpipc.h"
#include	"pthread_rwlock.h"

int my_pthread_rwlock_tryrdlock(my_pthread_rwlock_t *rw)
{
	int		result;

	if (rw->rw_magic != RW_MAGIC)
		return(EINVAL);

	if ( (result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return(result);

	if (rw->rw_refcount < 0 || rw->rw_nwaitwriters > 0)
		result = EBUSY;			/* held by a writer or waiting writers 不阻塞，直接返回错误*/
	else
		rw->rw_refcount++;		/* increment count of reader locks */

	pthread_mutex_unlock(&rw->rw_mutex);
	return(result);
}
/* end tryrdlock */

int Pthread_rwlock_tryrdlock(my_pthread_rwlock_t *rw)
{
	int		n;

	if ( (n = my_pthread_rwlock_tryrdlock(rw)) != 0) {
		errno = n;
		err_sys("my_pthread_rwlock_tryrdlock error");
	}
	return(n);
}
