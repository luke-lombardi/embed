#include <stdint.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#include "worker.h"


// internal methods
static uint8_t internal__worker_name_exists(char * name);


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

  // TODO: stop all threads that are executing in a safe way
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
  
  // don't allow duplicate worker names
  if(internal__worker_name_exists(name)) {
    return worker_manager_retcode__WORKER_DUPLICATE_NAME;
  }

  instance.workers[worker_idx].func = func;
  instance.workers[worker_idx].params = params;
  instance.workers[worker_idx].name = name;

  // start thread right away if start_on_create is set
  if (start_on_create) {
    pthread_create(&(instance.workers[worker_idx].thread), 
    /* const pthread_attr_t *attr */ NULL,
    instance.workers[worker_idx].func,
    instance.workers[worker_idx].params);
    instance.workers[worker_idx].started = 1;
  }

  return worker_manager_retcode__SUCCESS;
}

worker_manager_retcode_t worker_manager_start_worker(const char* name) {
  int worker_idx = -1;
  uint8_t idx;

  // find worker index in instance.workers
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

  // TODO: add error checking on pthread_create
  // start thread
  pthread_create(&(instance.workers[worker_idx].thread), 
    /* const pthread_attr_t *attr */ NULL,
    instance.workers[worker_idx].func,
    instance.workers[worker_idx].params);

  instance.workers[worker_idx].started = 1;

  return worker_manager_retcode__SUCCESS;
}

/////////////////////////////////////////////////// internal methods

static uint8_t internal__worker_name_exists(char * name) {
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
    return 0;
  }

  return 1;
}

 /*
    if(pthread_join(inc_x_thread, NULL)) {
      fprintf(stderr, "Error joining thread\n");
      return 2;
    }
 */

