/**
 * Author......: See docs/credits.txt
 * License.....: MIT
 */

#ifndef _THREAD_H
#define _THREAD_H

#include <signal.h>
#include <pthread.h>
#include <semaphore.h>

#define hc_thread_create(t,f,a)     pthread_create (&t, NULL, f, a)
#define hc_thread_wait(n,a)         for (int i = 0; i < n; i++) pthread_join ((a)[i], NULL)
#define hc_thread_exit(t)           pthread_exit (&t)
#define hc_thread_detach(t)         pthread_detach (t)

#define hc_thread_mutex_init(m)     pthread_mutex_init     (&m, NULL)
#define hc_thread_mutex_lock(m)     pthread_mutex_lock     (&m)
#define hc_thread_mutex_unlock(m)   pthread_mutex_unlock   (&m)
#define hc_thread_mutex_delete(m)   pthread_mutex_destroy  (&m)

#define hc_thread_sem_init(s)       sem_init  (&s, 0, 0)
#define hc_thread_sem_post(s)       sem_post  (&s)
#define hc_thread_sem_wait(s)       sem_wait  (&s)
#define hc_thread_sem_close(s)      sem_close (&s)

int mycracked (hashcat_ctx_t *hashcat_ctx);
int myabort_runtime (hashcat_ctx_t *hashcat_ctx);
int myabort_checkpoint (hashcat_ctx_t *hashcat_ctx);
int myabort_finish (hashcat_ctx_t *hashcat_ctx);
int myabort (hashcat_ctx_t *hashcat_ctx);
int myquit (hashcat_ctx_t *hashcat_ctx);
int bypass (hashcat_ctx_t *hashcat_ctx);
int SuspendThreads (hashcat_ctx_t *hashcat_ctx);
int ResumeThreads (hashcat_ctx_t *hashcat_ctx);
int stop_at_checkpoint (hashcat_ctx_t *hashcat_ctx);
int finish_after_attack (hashcat_ctx_t *hashcat_ctx);

#endif // _THREAD_H
