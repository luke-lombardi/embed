#ifndef LOGGER_H
#define LOGGER_H

enum logger_log_level {
  logger_log_level__TRACE = 0
  , logger_log_level__INFO = 1
  , logger_log_level__DEBUG = 2
  , logger_log_level__WARNING = 3
};

#define log_trace(...) logger_log(logger_log_level__TRACE, __VA_ARGS__)
#define log_debug(...) logger_log(logger_log_level__DEBUG, __VA_ARGS__)
#define log_info(...)  logger_log(logger_log_level__INFO, __VA_ARGS__)
#define log_warn(...)  logger_log(logger_log_level__WARNING, __VA_ARGS__)

typedef enum logger_retcode_t {
  logger_retcode__SUCCESS = 0
} logger_retcode_t;


typedef struct log_message_t {
  enum logger_log_level level;
  const char *message;
} log_message_t;

logger_retcode_t logger_init();
logger_retcode_t logger_uninit();
logger_retcode_t logger_log(enum logger_log_level level, const char* message);

void* logger_worker(void *params);

#endif