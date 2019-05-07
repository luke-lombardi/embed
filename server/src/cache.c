#include "cache.h"
#include "logger.h"
#include "worker.h"

#include <stdint.h>
#include <assert.h>

struct cache_instance {
  uint8_t is_initialized: 1;
};

static struct cache_instance instance = { 0} ;


cache_retcode_t cache_init() {
  log_debug("Cache initialized");
  return cache_retcode__SUCCESS;
}

cache_retcode_t cache_uninit() {

  return cache_retcode__SUCCESS;
}

void* cache_worker(void *params) {
  struct worker_params *wp = (struct worker_params *) params;
  uint8_t running = 1;

  assert(cache_init() == cache_retcode__SUCCESS);

  while(running) {


    pthread_mutex_lock(&(wp->worker_ptr->lock));
    running = wp->worker_ptr->running;
    pthread_mutex_unlock(&(wp->worker_ptr->lock));
  }

  wp->worker_ptr->finished = 1;

  cache_uninit();
  return NULL;
}