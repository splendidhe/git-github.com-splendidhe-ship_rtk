/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */


/* USART1 */
#define USART1_MAX_RECV_LEN     200                        /* 最大接收缓存字节数 200 */
#define USART1_MAX_SEND_LEN     200                        /* 最大发送缓存字节数 200 */
extern uint8_t USART1_RX_BUF[USART1_MAX_RECV_LEN];         /* 接收缓冲,最大USART1_MAX_RECV_LEN个字节.末字节为换行符 */
extern uint16_t USART1_RX_STA;                             /* 接收状态标记 */
extern uint8_t USART1_SingleByte;                          /* 单字节接收 */
extern uint8_t USART1_TX_BUF[USART1_MAX_SEND_LEN];     /* 发送缓冲,最大USART1_MAX_SEND_LEN个字节 */

/* USART2 */
#define USART2_MAX_RECV_LEN     400                        /* 最大接收缓存字节数 1024 */
#define USART2_SEN_LEN          400                        /* 最大发送缓存字节数 1024 */
extern uint8_t USART2_RX_BUF[USART2_MAX_RECV_LEN];         /* 接收缓冲,最大USART2_MAX_RECV_LEN个字节.末字节为换行符 */
extern uint16_t USART2_RX_STA;                             /* 接收状态标记 */
extern __align(8) uint8_t USART2_TX_BUF[USART2_SEN_LEN];   /* 发送缓冲,最大USART2_MAX_SEND_LEN个字节 */
extern uint8_t USART2_RX_Flag;                         	   /* 串口2接收标志位 */
extern uint8_t USART2_SingleByte;                          /* 单字节接收 */

/* USART3 */
#define USART3_MAX_RECV_LEN     400                        /* 最大接收缓存字节数 1024 */
extern uint8_t USART3_RX_BUF[USART3_MAX_RECV_LEN];         /* 接收缓冲,最大USART3_MAX_RECV_LEN个字节.末字节为换行符 */
extern uint16_t USART3_RX_STA;                         	   /* 接收状态标记 */
extern uint8_t USART3_RX_Flag;                        	   /* 串口3接收标志位 */
extern uint8_t USART3_SingleByte;                          /* 单字节接收 */

extern int waterlevel;

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart2;

extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */

void printf1(char *fmt, ...);                           //串口1打印函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);  //串口接收中断处理函数
void USER_UART2_IRQHander(UART_HandleTypeDef *huart);   //串口2中断处理函数
void USER_UART3_IRQHander(UART_HandleTypeDef *huart);   //串口3中断处理函数

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

