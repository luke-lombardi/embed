#include "sub.h"
#include "worker.h"

#include <stdio.h>
#include <pthread.h>


struct AppWorkers {
  worker_t sub;
  worker_t cache;
};

static struct AppWorkers Workers = {0};

void* test(void *param)
{
  printf("it went\n");
  return NULL;
}


void create_worker_threads() {
  Workers.sub.thread = pthread_create(&(Workers.sub.thread), NULL, test, NULL);
  Workers.cache.thread = pthread_create(&(Workers.cache.thread), NULL, test, NULL);

}


int main() {
  Sub_Init();

  create_worker_threads();

  // if(pthread_join(inc_x_thread, NULL)) {
  //   fprintf(stderr, "Error joining thread\n");
  //   return 2;
  // }

  while(1) {


    break;
  }

  // Sub_Uninit();
  return 0;
}