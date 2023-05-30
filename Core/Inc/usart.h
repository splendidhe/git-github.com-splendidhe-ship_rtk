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
#define USART1_MAX_RECV_LEN     200                        /* �����ջ����ֽ��� 200 */
#define USART1_MAX_SEND_LEN     200                        /* ����ͻ����ֽ��� 200 */
extern uint8_t USART1_RX_BUF[USART1_MAX_RECV_LEN];         /* ���ջ���,���USART1_MAX_RECV_LEN���ֽ�.ĩ�ֽ�Ϊ���з� */
extern uint16_t USART1_RX_STA;                             /* ����״̬��� */
extern uint8_t USART1_SingleByte;                          /* ���ֽڽ��� */
extern uint8_t USART1_TX_BUF[USART1_MAX_SEND_LEN];     /* ���ͻ���,���USART1_MAX_SEND_LEN���ֽ� */

/* USART2 */
#define USART2_MAX_RECV_LEN     400                        /* �����ջ����ֽ��� 1024 */
#define USART2_SEN_LEN          400                        /* ����ͻ����ֽ��� 1024 */
extern uint8_t USART2_RX_BUF[USART2_MAX_RECV_LEN];         /* ���ջ���,���USART2_MAX_RECV_LEN���ֽ�.ĩ�ֽ�Ϊ���з� */
extern uint16_t USART2_RX_STA;                             /* ����״̬��� */
extern __align(8) uint8_t USART2_TX_BUF[USART2_SEN_LEN];   /* ���ͻ���,���USART2_MAX_SEND_LEN���ֽ� */
extern uint8_t USART2_RX_Flag;                         	   /* ����2���ձ�־λ */
extern uint8_t USART2_SingleByte;                          /* ���ֽڽ��� */

/* USART3 */
#define USART3_MAX_RECV_LEN     400                        /* �����ջ����ֽ��� 1024 */
extern uint8_t USART3_RX_BUF[USART3_MAX_RECV_LEN];         /* ���ջ���,���USART3_MAX_RECV_LEN���ֽ�.ĩ�ֽ�Ϊ���з� */
extern uint16_t USART3_RX_STA;                         	   /* ����״̬��� */
extern uint8_t USART3_RX_Flag;                        	   /* ����3���ձ�־λ */
extern uint8_t USART3_SingleByte;                          /* ���ֽڽ��� */

extern int waterlevel;

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart2;

extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */

void printf1(char *fmt, ...);                           //����1��ӡ����
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);  //���ڽ����жϴ�����
void USER_UART2_IRQHander(UART_HandleTypeDef *huart);   //����2�жϴ�����
void USER_UART3_IRQHander(UART_HandleTypeDef *huart);   //����3�жϴ�����

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

