#ifndef __CAN_DEBUG_PRINT_H
#define __CAN_DEBUG_PRINT_H


#include "usart.h"
#include "stdio.h"
#include "stdarg.h"
#include <string.h>

//使用宏定义方式在打印日志之前，先添加文件名和行号
#define DEBUG_LOG_ENALE 1

#ifdef DEBUG_LOG_ENALE

//使用宏定义，只打印文件名称，不打印路径名称
//strrchr()从后向前查找字符串中的字符，+1表示指针向后移动一个字节，指向文件名的首地址
#define __FILE_NAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define __FILE_NAME (strrchr(__FILE_NAME__, '/') ? strrchr(__FILE_NAME__, '/') + 1 : __FILE_NAME__)


#define debug_printf(format, ...) printf("[%s:%d]  " format, __FILE_NAME, __LINE__, ##__VA_ARGS__)

#else
#define debug_printf(format, ...) 
#endif









#endif

