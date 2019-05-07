#include "logger.h"
#include "ll.h"
#include "worker.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>


struct logger_instance {
  volatile uint8_t is_initialized: 1;
  struct list *messages;
  pthread_mutex_t lock;
  FILE *fp;
};

static struct logger_instance instance = { 0 } ;

logger_retcode_t logger_init(FILE *fp) {
  pthread_mutex_init(&(instance.lock), NULL);
  pthread_mutex_lock(&(instance.lock));

    assert(instance.is_initialized == 0);
    instance.messages = linked_list_create();
    instance.is_initialized = 1;
    instance.fp = fp;

  pthread_mutex_unlock(&(instance.lock));
  
  return logger_retcode__SUCCESS;
}

logger_retcode_t logger_uninit() {

  return logger_retcode__SUCCESS;
}

logger_retcode_t logger_log(enum logger_log_level level, const char* message, const char* file, uint16_t line_number) {
  pthread_mutex_lock(&(instance.lock));

    assert(instance.is_initialized == 1);

    log_message_t *item = malloc(sizeof(log_message_t));
    item->level =  level;
    item->message = message;
    item->file = file;
    item->line_number = line_number;

    linked_list_push(instance.messages, (void*) item);
    
  pthread_mutex_unlock(&(instance.lock));

  return logger_retcode__SUCCESS;
}

void* logger_worker(void *params) {
  struct worker_params *wp = (struct worker_params *) params;
  uint8_t running = 1;

  FILE *fp = fopen(LOGGER_FILE_OUT, "a");
  assert(logger_init(fp) == logger_retcode__SUCCESS);

  log_message_t *item = NULL;
  while(running) {
    pthread_mutex_lock(&(instance.lock));
      item = (log_message_t*) linked_list_pop(instance.messages);

      /* Get current time */
      time_t t = time(NULL);
      struct tm *lt = localtime(&t);

        if ( (instance.fp != NULL) && (item != NULL) ) {
          char buf[256];
          memset(buf, 0x00, sizeof(buf));
          
          buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt)] = '\0';
          
          fprintf(instance.fp, 
          "%s %d %s:%d: %s", buf, 
          item->level, item->file, item->line_number, item->message);

          fprintf(instance.fp, "\n");
          fflush(instance.fp);
        }

    pthread_mutex_unlock(&(instance.lock));



    pthread_mutex_lock(&(wp->worker_ptr->lock));
      running = wp->worker_ptr->running;
    pthread_mutex_unlock(&(wp->worker_ptr->lock));
  }

  wp->worker_ptr->finished = 1;

  return NULL;
}