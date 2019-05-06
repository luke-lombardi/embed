#ifndef SUB_H
#define SUB_H

typedef enum sub_retcode_t {
  sub_retcode__SUCCESS = 0
  , sub_retcode__ERROR_ALLOC = 1
} sub_retcode_t;

sub_retcode_t sub_init();
sub_retcode_t sub_uninit();
void* sub_worker(void *params);

#endif