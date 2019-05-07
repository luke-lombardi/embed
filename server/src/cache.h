#ifndef CACHE_H
#define CACHE_H

typedef enum cache_retcode_t {
  cache_retcode__SUCCESS = 0
} cache_retcode_t;

cache_retcode_t cache_init();
cache_retcode_t cache_uninit();
void* cache_worker(void *params);

#endif