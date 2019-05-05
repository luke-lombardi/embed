#include "worker.h"
#include "sub.h"
#include "cache.h"

#include <stdio.h>
#include <pthread.h>

void* test(void* param) {
  printf("test worked");

  return NULL;
}

static void init() {
  sub_init();
  worker_manager_init();

  worker_manager_create_worker(test, NULL, "test_worker", 0 /*start_on_create*/);
}

static void uninit() {
  worker_manager_uninit();
  sub_uninit();
}

int main(int argc, char **argv) {
  init();

  worker_manager_start_worker("test_worker");

  while(1) {
    // break;
  }
  
  return 0;
}