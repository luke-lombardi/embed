#include "pubnub_sync.h"

#include "core/pubnub_helper.h"
#include "core/pubnub_timers.h"

#include "sub.h"
#include "worker.h"

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>


struct sub_instance {
  pubnub_t *ctx; 
  uint8_t is_initialized: 1;
};

static struct sub_instance instance = { 0} ;


sub_retcode_t sub_init() {
  assert(instance.is_initialized == 0);
  pubnub_t *ctx = pubnub_alloc();

  if (ctx == NULL) {
    puts("Couldn't allocate a Pubnub context");
    return sub_retcode__ERROR_ALLOC;
  }

  instance.ctx = ctx;
  instance.is_initialized = 1;

  return sub_retcode__SUCCESS;
}

sub_retcode_t sub_uninit() {
  assert(instance.is_initialized == 1);

  pubnub_free(instance.ctx);

  instance.is_initialized = 0;
  instance.ctx = NULL;

  return sub_retcode__SUCCESS;
}

void* sub_worker(void *params) {
  struct worker_params *wp = (struct worker_params *) params;
  uint8_t running = 1;

  assert(sub_init() == sub_retcode__SUCCESS);

  while(running) {


    pthread_mutex_lock(&(wp->worker_ptr->lock));
    running = wp->worker_ptr->running;
    pthread_mutex_unlock(&(wp->worker_ptr->lock));
  }

  wp->worker_ptr->finished = 1;

  sub_uninit();
  return NULL;
}