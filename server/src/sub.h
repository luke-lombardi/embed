#ifndef SUB_H
#define SUB_H

typedef enum Sub_RetCode {
  Sub_RetCode__Success = 0
  , Sub_RetCode__Error_Alloc = -1
} Sub_RetCode;


Sub_RetCode Sub_Init();
Sub_RetCode Sub_Uninit();

#endif