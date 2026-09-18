#ifndef __CAN_APP_H
#define __CAN_APP_H


#include "main.h"
#include "can.h"
#include "can_debug_print.h"

/* 车速消息发送 */
void SendVehicleSpeed(uint8_t speed_kph);

/* 发动机转速消息发送 */
void SendEngineRPM(uint16_t rpm);

/* 多参数消息发送示例 */
void SendVehicleStatus(void);

/* 消息接收处理 */
void ProcessCANMessages(void);

#endif
