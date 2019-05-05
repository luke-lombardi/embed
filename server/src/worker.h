/*
  Provides a high level interface to pthreads.

  With it, you can do the following:
    - create a worker thead
    - start/stop it
    - pass data between threads safely

*/

#ifndef WORKER_H
#define WORKER_H

#include <pthread.h>
#include <stdint.h>

#define MAX_WORKERS 4

typedef void* (*worker_func) (void*);

typedef struct worker_t {
  pthread_t thread;
  uint8_t in_use;
  uint8_t started;  // set on pthread_create
  uint8_t finished; // set when 'func' returns
  worker_func func;
  void* params;
  char *name;       // worker name, used to reference worker
} worker_t;

typedef enum worker_manager_retcode_t {
  worker_manager_retcode__SUCCESS = 0
  , worker_manager_retcode__NO_WORKERS_AVAIL = 1
  , worker_manager_retcode__WORKER_NOT_FOUND = 2
  , worker_manager_retcode__WORKER_DUPLICATE_NAME = 3
} worker_manager_retcode_t;

void worker_manager_init();
void worker_manager_uninit();

worker_manager_retcode_t worker_manager_create_worker(worker_func func, void* params, char *name, uint8_t start_on_create);
worker_manager_retcode_t worker_manager_start_worker(const char* name);

#endif 