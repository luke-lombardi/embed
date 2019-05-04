#include "pubnub_sync.h"

#include "core/pubnub_helper.h"
#include "core/pubnub_timers.h"

#include "sub.h"

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include <time.h>

struct Sub_Instance {
  pubnub_t *Ctx; 
  uint8_t IsInitialized: 1;
};

static struct Sub_Instance Instance = { 0} ;


Sub_RetCode Sub_Init() {
  assert(Instance.IsInitialized == 0);
  pubnub_t *ctx = pubnub_alloc();

  if (NULL == ctx) {
    puts("Couldn't allocate a Pubnub context");
    return Sub_RetCode__Error_Alloc;
  } else {
    printf("ok\n");
  }

  Instance.Ctx = ctx;
  Instance.IsInitialized = 1;

  return Sub_RetCode__Success;
}


Sub_RetCode Sub_Uninit() {
  assert(Instance.IsInitialized == 1);

  pubnub_free(Instance.Ctx);

  return Sub_RetCode__Success;
}