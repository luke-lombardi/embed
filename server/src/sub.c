#include "pubnub_sync.h"

#include "core/pubnub_helper.h"
#include "core/pubnub_timers.h"

#include "sub.h"
#include "worker.h"
#include "logger.h"
#include "utils.h"

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>


#define PUBNUB_PUBLISH_KEY  "pub-c-030e0611-2f65-49db-b496-29f6f0ff25f9"
#define PUBNUB_SUBSCRIBE_KEY  "sub-c-41a2f9e6-6e96-11e9-8724-8269f6864ada"


struct sub_instance {
  pubnub_t *ctx; 
  uint8_t is_initialized: 1;
};

static struct sub_instance instance = { 0} ;

sub_retcode_t sub_init() {
  assert(instance.is_initialized == 0);
  pubnub_t *ctx = pubnub_alloc();

  if (ctx == NULL) {
    log_error("Couldn't allocate a Pubnub context");
    return sub_retcode__ERROR_ALLOC;
  }

  log_debug("Pubnub context allocated");
  pubnub_init(ctx, /* publish key */ PUBNUB_PUBLISH_KEY, /* subscribe key */ PUBNUB_SUBSCRIBE_KEY);

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

  // pubnub_subscribe(instance.ctx, "my_channel", NULL);
  // pbresult = pubnub_await(instance.ctx);
  // if (pbresult == PNR_OK) {
  //     char const *message = pubnub_get(instance.ctx);
  //     while (message != NULL) {
  //         message = pubnub_get(instance.ctx);
  //     }
  // }

  while(running) {



    log_info("Still alive");

    pthread_mutex_lock(&(wp->worker_ptr->lock));
    running = wp->worker_ptr->running;
    pthread_mutex_unlock(&(wp->worker_ptr->lock));
    delay(1);
  }

  wp->worker_ptr->finished = 1;

  sub_uninit();
  return NULL;
}