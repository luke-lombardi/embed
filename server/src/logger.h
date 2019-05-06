#ifndef LOGGER_H
#define LOGGER_H

typedef enum logger_retcode_t {
  logger_retcode__SUCCESS = 0
} logger_retcode_t;

logger_retcode_t logger_init();
logger_retcode_t logger_uninit();
void* logger_worker(void *params);

#endif