#include "pubnub_sync.h"

#include "core/pubnub_helper.h"
#include "core/pubnub_timers.h"

#include "sub.h"

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
  } else {
    printf("ok\n");
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