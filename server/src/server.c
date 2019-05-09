#include "worker.h"
#include "sub.h"
#include "cache.h"
#include "logger.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <regex.h>
#include <assert.h>

#define MAX_COMMAND_LENGTH 100

// regex for handling commands (when server is not run as daemon)
regex_t re_command_types[2];
const char *re_cache = "^abc$";
const char *re_sub = "bcd";

static void init() {
  worker_manager_init();

  // create workers, but don't run them yet
  worker_manager_create_worker(cache_worker, NULL, "cache_worker", 0 /* start_on_create */);
  worker_manager_create_worker(sub_worker, NULL, "sub_worker", 0 /* start_on_create */);
  worker_manager_create_worker(logger_worker, NULL, "logger", 0 /* start_on_create */);

  // compile regexes for handling different commands to the worker threads
  int reti = regcomp(&re_command_types[0], re_cache, 0);
  assert(reti == 0);
  reti = regcomp(&re_command_types[1], re_cache, 0);
  assert(reti == 0);

}

uint8_t handle_command(char* cmd) {

  // subscriber command
  if ( !(regexec(&re_command_types[0], cmd, 0, NULL, 0)) ) {
  
  // cache command
  } else if ( !(regexec(&re_command_types[0], cmd, 0, NULL, 0)) ) {

  } else if ( strcmp("exit", cmd) == 0) {
    printf("\nshutting down workers... \n");
    return 1;   //  exit_flag = 1
  
  // command didn't match
  } else {
    printf("\ninvalid command. \n");
  }

  return 0;
}

int main(int argc, char **argv) {
  init();

  worker_manager_start_worker("logger");
  
  // short delay to allow the logger to initialize
  for(uint32_t i=0;i<1000000;i++) {}

  worker_manager_start_worker("cache_worker");
  worker_manager_start_worker("sub_worker");

  uint8_t exit_flag = 0;
  char *cmd = malloc( sizeof(char) * MAX_COMMAND_LENGTH );

  while(!exit_flag) {
    printf("server> ");
    scanf("%s[^\n]", cmd);
    exit_flag = handle_command(cmd);
  }

  free(cmd);
  worker_manager_uninit();
  return 0;
}