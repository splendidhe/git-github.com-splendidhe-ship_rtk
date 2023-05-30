/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dht11.h"
#include "ds18b20.h"
#include "string.h"
#include "stdio.h"
#include "sys.h"
#include "delay.h"
#include "jy61p.h"
#include "ec20.h"
#include "ms5837iic.h"
#include "MS5837.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "rtk_parse.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
 uint8_t temperature; // 温度
 uint8_t humidity;    // 湿度
 uint32_t Adc_buffer[6]; // ADC采集数据缓存
/*
    数组数据
    Adc_buffer[0]  --  水浸传感器  
    Adc_buffer[1]  --  风速传感器
    Adc_buffer[2]  --  XXX传感器
    Adc_buffer[3]  --  浊度传感器
    Adc_buffer[4]  --  PH传感器
    Adc_buffer[5]  --  TDS传感器
*/

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

float wind_direction_check(float winddirection);    // 风向检测
void ADC_Process_Function(void);                    // ADC数据处理函数
float turbidity_check(float turbidity);             // 浊度检测
float PH_check(float PH);                           // PH值检测
float tds_check(float tdsvalue);                    // TDS检测
uint8_t check_seepage(float watervo);               // 水渗检测
void HCSR04_Detect(void);                           // 超声波检测
void MPU6050_Detect(void);                          // MPU6050检测
void MS5837_Detect(void);                           // MS5837检测
void Temper_Detect(void);                           // 温度检测
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM6_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_USART3_UART_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
    delay_init(168);  // 初始化延时函数
    while(dht11_init())  // 初始化DHT11
    {
        printf("DHT11 init failed!\r\n");
        HAL_Delay(1000);
    }
    while(DS18B20_Init())  // 初始化DS18B20
    {
        printf("DS18B20 init failed!\r\n");
        HAL_Delay(1000);
    }
    MS5837_init();	     //初始化MS5837
    HAL_ADC_Start_DMA(&hadc1,Adc_buffer,6);                 // 启动ADC DMA传输
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&USART1_SingleByte, 1);     // 串口1中断接收
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);                        // 串口2使能空闲中断
    HAL_UART_Receive_DMA(&huart2, USART2_RX_BUF, USART2_MAX_RECV_LEN);  // 串口2中断接收  DMA
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);                        // 串口3使能空闲中断
    HAL_UART_Receive_DMA(&huart3, USART3_RX_BUF, USART3_MAX_RECV_LEN);  // 串口3中断接收  DMA
    while(EC20_init_check(0) != 0x0F);  // EC20初始化检测
    MPU_Init();  //初始化MPU6050
    while(mpu_dmp_init());  //初始化DMP
    printf1("MPU6050 init Success!\r\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
        Temper_Detect();    // 温度检测
        ADC_Process_Function();   // ADC数据处理函数
        HCSR04_Detect();    // 超声波检测
        MPU6050_Detect();   // MPU6050检测
        MS5837_Detect();    // MS5837检测
        GPS_Analysis();     // GPS数据解析
        Parase_RTK();       // 解析RTK数据
        Mqtt_Senddata();    // MQTT发送数据      
        HAL_Delay(1000);          // 延时1s
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USART3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART3_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);
}

/* USER CODE BEGIN 4 */

void Temper_Detect(void)
{
    /* DHT11 */
    dht11_read_data(&temperature, &humidity);  //读取温湿度
    printf1("temperature: %d, humidity: %d\r\n", temperature, humidity);  //打印温湿度
    /* DS18B20 */
    watertemperature = DS18B20_Get_Temp();
    watertemperature = (watertemperature + 5) / 10;  //水温取整
    printf1("watertemperature: %d\r\n", watertemperature);  //打印水温
}

void MS5837_Detect(void)
{
    MS5837_Getdata();   //获取大气压
}


void HCSR04_Detect(void)  //超声波检测
{  
    distance = HCSR04_measure();  //测量距离
    printf1("Distance = %dmm\r\n",distance);   //打印距离
}

void MPU6050_Detect(void)  
{
    short aacx,aacy,aacz;			//加速度传感器原始数据
	short gyrox,gyroy,gyroz;		//陀螺仪原始数据
	short temp;						//温度
    while(mpu_dmp_get_data(&pitch, &roll, &yaw));  //获取姿态角
    temp = MPU_Get_Temperature();	//得到温度值
    MPU_Get_Accelerometer(&aacx, &aacy, &aacz);	//得到加速度传感器数据
    MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);	//得到陀螺仪数据
    if(temp<0)
    {
        temp = -temp;		//转为正数
        printf1("temp: -%d.%d C\r\n",-temp/100,-temp%10);	//打印温度
    }
    else
    {
        printf1("temp: %d.%d C\r\n",temp/100,temp%10);	//打印温度
    }
    temp = pitch*10;
    if(temp<0)
    {
        temp=-temp;		//转为正数
        printf1("pitch: -%d.%d°\r\n",temp/10,temp%10);	//打印俯仰角
    }
    else
    {
        printf1("pitch: %d.%d°\r\n",temp/10,temp%10);	//打印俯仰角
    }
    temp = roll*10;
    if(temp<0)
    {
        temp=-temp;		//转为正数
        printf1("roll: -%d.%d°\r\n",temp/10,temp%10);	//打印横滚角
    }
    else
    {
        printf1("roll: %d.%d°\r\n",temp/10,temp%10);	//打印横滚角
    }
    temp = yaw*10;
    if(temp<0)
    {
        temp=-temp;		//转为正数
        printf1("yaw: -%d.%d°\r\n",temp/10,temp%10);	//打印航向角
    }
    else
    {
        printf1("yaw: %d.%d°\r\n",temp/10,temp%10);	//打印航向角
    }
}

/**
  * @brief  ADC数据处理函数
  * @param  None
  * @retval None
  */
void ADC_Process_Function(void)
{
    static float temp = 0;
    printf1("水浸传感器: %d    ", Adc_buffer[0]);
    temp = (float)Adc_buffer[0] * 3.3f / 4095.0f;  // 电压值
    seepage = check_seepage(temp);  // 漏水检测
    printf1("seepage: %d\r\n", seepage);

    /* 风速传感器 */
    printf1("风速传感器: %d     ", Adc_buffer[1]);
    temp = (3.3f * (float)Adc_buffer[1] / 4095.0f) * 1000.0f;  // 电压值
    windspeed = 0.027f * temp;  // 风速值
    printf1("风速值: %f\r\n", windspeed);

    /* XX传感器 */
    printf1("XX传感器: %d     \r\n", Adc_buffer[2]);

    /* 浊度传感器 */
    printf1("浊度传感器: %d     ", Adc_buffer[3]);
    temp = (float)Adc_buffer[3] * 3.3f / 4095.0f;  // 电压值
    temp = temp * 100 / 3.3f;  // 浊度值
    turbidity = turbidity_check(temp);  // 浊度值
    printf1("浊度值: %f\r\n", turbidity);

    /* PH传感器 */
    printf1("PH传感器: %d     ", Adc_buffer[4]);
    temp = (float)Adc_buffer[4] * 3.3f / 4095;  // 电压值
    temp = -5.7541 * temp + 16.654;  // PH值
    ph = PH_check(temp);  // PH值
    printf1("PH值: %f\r\n", ph);

    /* TDS传感器 */
    printf1("TDS传感器: %d     ", Adc_buffer[5]);
    temp = (float)Adc_buffer[5] * 3.3f / 4095.0f;  // 电压值
    temp = (float)((float)66.71 * temp * temp * temp - (float)127.93 * temp * temp + (float)428.7 * temp); // TDS值
    tds = tds_check(temp);  // TDS等级
    printf1("TDS等级: %f\r\n", tds);

}

float wind_direction_check(float winddirection)  // 风向阈值检查
{
    if(winddirection > 360)
    {
        winddirection = 360;
    }

    return winddirection;
}

float turbidity_check(float turbidity)  // 浊度阈值检查
{
    if(turbidity > 100)
    {
        turbidity = 100;
    }

    return turbidity;
}

float PH_check(float PH)  // PH阈值检查
{
    if(PH > 14)
    {
        PH = 14;
    }
    else if(PH < 0)
    {
        PH = 0;
    }

    return PH;
}


float tds_check(float tdsvalue)  // TDS等级判断
{
    if(tdsvalue < 0.0f)
    {
        tdsvalue = 0.0f;
    }

    return tdsvalue;
}

uint8_t check_seepage(float watervo)  // 漏水检测
{
    if((double)watervo > 1.6)
    {
        return 0;  // 无漏水
    }
    else
    {
        return 1;  // 有漏水
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
