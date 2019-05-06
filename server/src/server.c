#include "worker.h"
#include "sub.h"
#include "cache.h"

#include <stdio.h>
#include <pthread.h>


static void init() {
  worker_manager_init();

  // create workers, but don't run them yet
  worker_manager_create_worker(sub_worker, NULL, "cache_worker", 0 /* start_on_create */);
  worker_manager_create_worker(sub_worker, NULL, "sub_worker", 0 /* start_on_create */);

}

int main(int argc, char **argv) {
  init();

  // worker_manager_start_worker("cache_worker");
  worker_manager_start_worker("sub_worker");

  while(1) {
    
    // break;
  }

  worker_manager_uninit();
  return 0;
}