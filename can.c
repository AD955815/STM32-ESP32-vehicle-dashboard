/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
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
/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */
 /* 接收缓冲�?? */
#define RX_BUFFER_SIZE 10
CAN_Message_t rxBuffer[RX_BUFFER_SIZE];
volatile uint8_t rxWriteIndex = 0;
volatile uint8_t rxReadIndex = 0;
/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */
  HAL_StatusTypeDef status;
  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 4;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = ENABLE;
  hcan.Init.AutoWakeUp = ENABLE;
  hcan.Init.AutoRetransmission = ENABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */
  /* 配置接收滤波�??? */
  CAN_FilterTypeDef canFilter;
  // 滤波�???0: 接收�???有标准帧
  canFilter.FilterBank = 0;
  canFilter.FilterMode = CAN_FILTERMODE_IDMASK;    // 掩码模式
  canFilter.FilterScale = CAN_FILTERSCALE_32BIT;   // 32�???
  canFilter.FilterIdHigh = 0x0000;                 // 标识�???
  canFilter.FilterIdLow = 0x0000;
  canFilter.FilterMaskIdHigh = 0x0000;             // 掩码(0=不关�???)
  canFilter.FilterMaskIdLow = 0x0000;
  canFilter.FilterFIFOAssignment = CAN_RX_FIFO0;   // 分配到FIFO0
  canFilter.FilterActivation = ENABLE;
  canFilter.SlaveStartFilterBank = 14;
  status = HAL_CAN_ConfigFilter(&hcan, &canFilter);
  if (status != HAL_OK) {
  printf("CAN Filter0 Error");
  }
  // 滤波�???1: 接收特定ID（示例：0x100-0x10F�???
  canFilter.FilterBank = 1;
  canFilter.FilterMode = CAN_FILTERMODE_IDLIST;    // 列表模式
  canFilter.FilterScale = CAN_FILTERSCALE_32BIT;   
  canFilter.FilterIdHigh = 0x100 << 5;             // ID左移5�???
  canFilter.FilterIdLow =  0x200 << 5;
  canFilter.FilterMaskIdHigh = 0x300 << 5;         
  canFilter.FilterMaskIdLow = 0x0000;
  canFilter.FilterFIFOAssignment = CAN_RX_FIFO1;   // 分配到FIFO1
  canFilter.FilterActivation = ENABLE;
  status = HAL_CAN_ConfigFilter(&hcan, &canFilter);
  if (status != HAL_OK) {
  printf("CAN Filter1 Error");
  }
  /* 启动CAN */
  status = HAL_CAN_Start(&hcan);
  if (status != HAL_OK) {
  printf("CAN Start Error");
  }
  /* 使能FIFO接收中断 */
  HAL_CAN_ActivateNotification(&hcan,
  CAN_IT_RX_FIFO0_MSG_PENDING |
  CAN_IT_RX_FIFO1_MSG_PENDING |
  CAN_IT_ERROR |
  CAN_IT_BUSOFF);
  /* USER CODE END CAN_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_SCE_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
 HAL_StatusTypeDef CAN_Transmit(CAN_Message_t* msg)
  {
    CAN_TxHeaderTypeDef txHeader;
    uint32_t txMailbox;
    /* 配置发�?�头 */
    if (msg->format == 0) 
    {
    // 标准�??
    txHeader.StdId = msg->id;
    txHeader.IDE = CAN_ID_STD;
    } 
    else 
    {
    // 扩展�??
    txHeader.ExtId = msg->id;
    txHeader.IDE = CAN_ID_EXT;
    }
    txHeader.RTR = (msg->type == 0) ? CAN_RTR_DATA : CAN_RTR_REMOTE;
    txHeader.DLC = msg->len;
    txHeader.TransmitGlobalTime = DISABLE;
    /* 发�?�消�?? */
    return HAL_CAN_AddTxMessage(&hcan, &txHeader, msg->data, &txMailbox);
  }
  /**
  * @brief  CAN接收回调函数
  * @param  hcan: CAN句柄
  * @retval None
  */
  void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
  {
    CAN_RxHeaderTypeDef rxHeader;
    /* 从FIFO0读取消息 */
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader,
    rxBuffer[rxWriteIndex].data) == HAL_OK)
    {
      /* 保存消息信息 */
      if (rxHeader.IDE == CAN_ID_STD) 
      {
      rxBuffer[rxWriteIndex].id = rxHeader.StdId;
      rxBuffer[rxWriteIndex].format = 0;
      } 
      else 
      {
      rxBuffer[rxWriteIndex].id = rxHeader.ExtId;
      rxBuffer[rxWriteIndex].format = 1;
      }
      rxBuffer[rxWriteIndex].len = rxHeader.DLC;
      rxBuffer[rxWriteIndex].type = (rxHeader.RTR == CAN_RTR_DATA) ? 0 : 1;
      /* 更新写指�?? */
      rxWriteIndex = (rxWriteIndex + 1) % RX_BUFFER_SIZE;
    }
  }

  void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
  {
    printf(">>> FIFO1 CALLBACK\r\n");
    CAN_RxHeaderTypeDef rxHeader;

    if (HAL_CAN_GetRxMessage(hcan,
                              CAN_RX_FIFO1,
                              &rxHeader,
                              rxBuffer[rxWriteIndex].data) == HAL_OK)
      {
          if (rxHeader.IDE == CAN_ID_STD)
          {
              rxBuffer[rxWriteIndex].id = rxHeader.StdId;
              rxBuffer[rxWriteIndex].format = 0;
          }
          else
          {
              rxBuffer[rxWriteIndex].id = rxHeader.ExtId;
              rxBuffer[rxWriteIndex].format = 1;
          }

          rxBuffer[rxWriteIndex].len = rxHeader.DLC;
          rxBuffer[rxWriteIndex].type =
              (rxHeader.RTR == CAN_RTR_DATA) ? 0 : 1;

          printf("Callback ID=0x%03lX DLC=%lu\r\n",
              rxHeader.StdId,
              rxHeader.DLC);
          rxWriteIndex =
              (rxWriteIndex + 1) % RX_BUFFER_SIZE;
      }
  }
  /**
  * @brief  CAN错误回调函数
  * @param  hcan: CAN句柄
  * @retval None
  */
  void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
  {
    uint32_t errorCode = HAL_CAN_GetError(hcan);
    if (errorCode & HAL_CAN_ERROR_EWG) 
    {
    // 错误警告
    }
    if (errorCode & HAL_CAN_ERROR_EPV) 
    {
    // 被动错误
    }
    if (errorCode & HAL_CAN_ERROR_BOF) 
    {
    // 总线关闭
    // �??要复位CAN控制�??
    HAL_CAN_ResetError(hcan);
    }
  }

  /**
  * @brief  从接收缓冲区读取消息
  * @param  msg: 消息存储指针
  * @retval 1=成功, 0=无消�??
  */
  uint8_t CAN_Receive(CAN_Message_t* msg)
  {
    if (rxReadIndex == rxWriteIndex) 
    {
    return 0;  // 缓冲区空
    }
    printf("CAN_Receive: R=%d W=%d\r\n",
          rxReadIndex,
          rxWriteIndex);
    /* 复制消息 */
    memcpy(msg, &rxBuffer[rxReadIndex], sizeof(CAN_Message_t));
    /* 更新读指�?? */
    rxReadIndex = (rxReadIndex + 1) % RX_BUFFER_SIZE;
    return 1;
  }

    /**
    * @brief  获取CAN错误状�??
    * @param  None
    * @retval 错误代码
    */
    uint32_t CAN_GetErrorStatus(void)
    {
      uint32_t esr = hcan.Instance->ESR;
      uint8_t tec = (esr >> 16) & 0xFF;  // 发�?�错误计�??
      uint8_t rec = (esr >> 24) & 0xFF;  // 接收错误计数
      printf("CAN ESR = 0x%08lX\r\n",
           (unsigned long)esr);
      printf("TEC: %d, REC: %d\r\n", tec, rec);
      return esr;
    }
/* USER CODE END 1 */
