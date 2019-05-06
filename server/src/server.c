#include "worker.h"
#include "sub.h"
#include "cache.h"

#include <stdio.h>
#include <pthread.h>

void* test(void* params) {
  struct worker_params *wp = (struct worker_params *) params;
  uint8_t running = 1;

  while(running) {
    pthread_mutex_lock(&(wp->worker_ptr->lock));
    running = wp->worker_ptr->running;
    pthread_mutex_unlock(&(wp->worker_ptr->lock));
  }

  return NULL;
}

static void init() {
  sub_init();
  worker_manager_init();

  worker_manager_create_worker(test, NULL, "test_worker", 0 /* start_on_create */);
  worker_manager_create_worker(test, NULL, "test_worker_2", 0 /* start_on_create */);
}

static void uninit() {
  worker_manager_uninit();
  sub_uninit();
}

int main(int argc, char **argv) {
  init();

  worker_manager_start_worker("test_worker");

  while(1) {
    worker_manager_stop_worker("test_worker");
    // break;
  }

  /*
      if(pthread_join(inc_x_thread, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
      }
  */

  return 0;
}