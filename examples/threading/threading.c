#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    
    // Wait for the specified time before obtaining the mutex
    usleep(thread_func_args->wait_to_obtain_ms * 1000);

    // Try to obtain the mutex
    if (pthread_mutex_lock(thread_func_args->mutex) != 0) {
        ERROR_LOG("Failed to obtain mutex");
        thread_func_args->thread_complete_success = false;
        return thread_param;
    }

    // Hold the mutex for the specified time
    usleep(thread_func_args->wait_to_release_ms * 1000);

    // Release the mutex
    if (pthread_mutex_unlock(thread_func_args->mutex) != 0) {
        ERROR_LOG("Failed to release mutex");
        thread_func_args->thread_complete_success = false;
        return thread_param;
    }

    // Mark thread as successfully completed
    thread_func_args->thread_complete_success = true;
    return thread_param;
}

bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex, int wait_to_obtain_ms, int wait_to_release_ms)
{
    // Allocate memory for thread_data structure
    struct thread_data* tdata = (struct thread_data*) malloc(sizeof(struct thread_data));
    if (tdata == NULL) {
        ERROR_LOG("Failed to allocate memory for thread_data");
        return false;
    }

    // Initialize the thread_data structure
    tdata->mutex = mutex;
    tdata->wait_to_obtain_ms = wait_to_obtain_ms;
    tdata->wait_to_release_ms = wait_to_release_ms;
    tdata->thread_complete_success = false;

    // Create the thread
    if (pthread_create(thread, NULL, threadfunc, (void*) tdata) != 0) {
        ERROR_LOG("Failed to create thread");
        free(tdata);
        return false;
    }

    return true;
}

