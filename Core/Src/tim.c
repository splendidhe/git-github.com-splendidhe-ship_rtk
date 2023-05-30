/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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
/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "stdio.h"
#include "gpio.h"
unsigned int  dta;

/* USER CODE END 0 */

TIM_HandleTypeDef htim6;

/* TIM6 init function */
void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 83;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 65535;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM6)
  {
  /* USER CODE BEGIN TIM6_MspInit 0 */

  /* USER CODE END TIM6_MspInit 0 */
    /* TIM6 clock enable */
    __HAL_RCC_TIM6_CLK_ENABLE();
  /* USER CODE BEGIN TIM6_MspInit 1 */

  /* USER CODE END TIM6_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM6)
  {
  /* USER CODE BEGIN TIM6_MspDeInit 0 */

  /* USER CODE END TIM6_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM6_CLK_DISABLE();
  /* USER CODE BEGIN TIM6_MspDeInit 1 */

  /* USER CODE END TIM6_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


void for_delay_us(uint16_t nus)//΢�뼶��ʱ��ѭ����
{
	uint16_t i;
	for(i=0;i<nus;i++){
	 __NOP();}
}
 
void  Start(void) //����������ģ��
{
    HAL_GPIO_WritePin(Trig_GPIO_Port,Trig_Pin,GPIO_PIN_SET); //����
    for_delay_us(15);          //15us��ʱ
    HAL_GPIO_WritePin(Trig_GPIO_Port,Trig_Pin,GPIO_PIN_RESET);  //����
}

int Get(void)
{       
    return HAL_GPIO_ReadPin(Echo_GPIO_Port,Echo_Pin); //���Echo�ĵ�ƽ�ߵ�
}

void start(void)//��ʼ��ʱ
{
    __HAL_TIM_SetCounter(&htim6,0);   //��0��ʼ����
    HAL_TIM_Base_Start_IT(&htim6);   // ������ʱ�������ж�       
}
 
unsigned int get_stop(void)//��ȡֹ֮ͣ�������
{  
   unsigned int  num,dta;
   HAL_TIM_Base_Stop_IT (&htim6);       //�رն�ʱ�������ж�
   num = __HAL_TIM_GetCounter(&htim6);  //��ü���ֵ
   dta = (num*346)/20000;               //cmΪ��λ
    return dta;
}


int HCSR04_measure(void)
{
	Start();            //����������ģ��
	while(!Get());      //��Echo����0���ȴ�,��Ϊ1����        
	start();            //��ʼ��ʱ
	while(Get());		//��EchoΪ1�������ȴ�  ,��Ϊ0����     
	dta = get_stop();   //��ȡֹ֮ͣ�������
	return dta;
}

/* USER CODE END 1 */
