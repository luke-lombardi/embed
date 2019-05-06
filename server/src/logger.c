#include "logger.h"

#include <stdio.h>
#include <stdint.h>

struct logger_instance {
  uint8_t is_initialized: 1;
};

static struct logger_instance instance = { 0 } ;

logger_retcode_t logger_init() {

  return logger_retcode__SUCCESS;
}

logger_retcode_t logger_uninit() {

  return logger_retcode__SUCCESS;
}

void* logger_worker(void *params) {


  return NULL;
}