/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.h
  * @brief   This file contains all the function prototypes for
  *          the can.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_H__
#define __CAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stm32f1xx_hal_conf.h"
#include "can_debug_print.h"
/* USER CODE END Includes */

extern CAN_HandleTypeDef hcan;

/* USER CODE BEGIN Private defines */
/* CAN消息结构�?? */
  typedef struct {
  uint32_t id;           // CAN标识�??
  uint8_t  data[8];      // 数据
  uint8_t  len;          // 数据长度
  uint8_t  format;       // 0=标准�??, 1=扩展�??
  uint8_t  type;         // 0=数据�??, 1=远程�??
  } CAN_Message_t;

/* USER CODE END Private defines */

void MX_CAN_Init(void);

/* USER CODE BEGIN Prototypes */
/**
  * @brief  CAN GPIO配置
  * @param  hcan: CAN句柄
  * @retval None
*/
  void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan);

/**
  * @brief  CAN发�?�函�??
  * @param  msg: 要发送的消息
  * @retval HAL状�??
*/
HAL_StatusTypeDef CAN_Transmit(CAN_Message_t* msg);

/**
  * @brief  CAN接收回调函数
  * @param  hcan: CAN句柄
  * @retval None
*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

/**
  * @brief  CAN错误回调函数
  * @param  hcan: CAN句柄
  * @retval None
*/
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan);

/**
  * @brief  从接收缓冲区读取消息
  * @param  msg: 消息存储指针
  * @retval 1=成功, 0=无消�??
*/
uint8_t CAN_Receive(CAN_Message_t* msg);

/**
  * @brief  获取CAN错误状�??
  * @param  None
  * @retval 错误代码
*/
uint32_t CAN_GetErrorStatus(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */

