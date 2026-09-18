#include "can_debug_print.h"


//重定向编写函数fputc
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 1000);
  return ch;
}



