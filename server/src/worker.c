#include <stdint.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#include "worker.h"

struct worker_manager_instance {
  uint8_t is_initialized: 1;
  worker_t workers[MAX_WORKERS];
};

static struct worker_manager_instance instance = { 0 };


void worker_manager_init() {
  assert(instance.is_initialized == 0);

  instance.is_initialized = 1;
}


void worker_manager_uninit() {
  assert(instance.is_initialized == 1);

  instance.is_initialized = 0;
}


worker_manager_retcode_t worker_manager_create_worker(worker_func func, void* params, char *name, uint8_t start_on_create) {
  uint8_t idx;
  int worker_idx = -1;

  for(idx = 0; idx < MAX_WORKERS; idx++) {
    if(instance.workers[idx].in_use == 0) {
      worker_idx = idx;
      break;
    }
  }

  if(worker_idx == -1) {
    return worker_manager_retcode__NO_WORKERS_AVAIL;
  }

  instance.workers[worker_idx].func = func;
  instance.workers[worker_idx].params = params;
  instance.workers[worker_idx].name = name;

  if (start_on_create) {
    pthread_create(&(instance.workers[worker_idx].thread), 
    /* const pthread_attr_t *attr */ NULL,
    instance.workers[worker_idx].func,
    instance.workers[worker_idx].params);
  }

  return worker_manager_retcode__SUCCESS;
}

worker_manager_retcode_t worker_manager_start_worker(const char* name) {
  int worker_idx = -1;
  uint8_t idx;

  for(idx = 0; idx < MAX_WORKERS; idx++) {
    if(instance.workers[idx].name == NULL)
      continue;

    if ( strcmp( name, (const char*) instance.workers[idx].name) == 0) {
      worker_idx = idx;
      break;
    }
  }

  if(worker_idx == -1) {
    return worker_manager_retcode__WORKER_NOT_FOUND;
  }

  pthread_create(&(instance.workers[worker_idx].thread), 
    /* const pthread_attr_t *attr */ NULL,
    instance.workers[worker_idx].func,
    instance.workers[worker_idx].params);

  return worker_manager_retcode__SUCCESS;
}

 /*
    if(pthread_join(inc_x_thread, NULL)) {
      fprintf(stderr, "Error joining thread\n");
      return 2;
    }
 */

