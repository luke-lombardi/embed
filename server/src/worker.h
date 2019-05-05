#ifndef WORKER_H
#define WORKER_H

#include <pthread.h>
#include <stdint.h>

typedef struct worker_t {
  pthread_t thread;
  uint8_t started : 1;
  uint8_t done : 1;
} worker_t;


#endif 