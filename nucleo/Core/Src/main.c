/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define TRUE 1
#define FALSE 0
#define COUNTOF(__BUFFER__) (sizeof(__BUFFER__)/sizeof(*(__BUFFER__)))
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim16;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

/* USER CODE BEGIN PV */
int led_status = FALSE;
//char data[] = "Test\r\n";
uint8_t a[4];
uint8_t rx_data;
uint8_t buf[1000];

uint8_t mise_buffer[32];
uint8_t mise_send_buffer[7] = {0x42,0x4d,0};
uint8_t receive_complete=0;
uint8_t sleep_mode = 0;

int loop_value = 1;
int list_value = 0;
int i = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM16_Init(void);
/* USER CODE BEGIN PFP */

void print_MISE(void);
void write_MISE(char* cmd);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, 10);

	return 1;
}

int __io_getchar(void) {
	HAL_UART_Receive_IT(&huart2, rx_data, 1);

	return 1;
}

//void delay(uint16_t time) {
//	__HAL_TIM_SET_COUNTER(&htim2, 0);
//	while ((__HAL_TIM_GET_COUNTER(&htim2))<time);
//}
//
//uint8_t Rh_byte1, Rh_byte2, Temp_byte1, Temp_byte2;
//uint16_t SUM, RH, TEMP;

//float Temperature = 0;
//float Humidity = 0;
//uint8_t Presence = 0;
//
//void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	GPIO_InitStruct.Pin = GPIO_Pin;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
//}
//
//void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	GPIO_InitStruct.Pin = GPIO_Pin;
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
//}

//#define DHT22_PORT GPIOA
//#define DHT22_PIN GPIO_PIN_1
//
//void DHT22_Start(void) {
//	Set_Pin_Output(DHT22_PORT, DHT22_PIN);
//	HAL_GPIO_WritePin(DHT22_PORT, DHT22_PIN, 0);
//	HAL_Delay(1200);
//
//	HAL_GPIO_WritePin(DHT22_PORT, DHT22_PIN, 1);
//	HAL_Delay(20);
//
//	Set_Pin_Input(DHT22_PORT, DHT22_PIN);
//}

//uint8_t DHT22_Check_Response(void) {
//	uint8_t Response = 0;
//	HAL_Delay(40);
//	if(!(HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN))) {
//		HAL_Delay(80);
//
//		if((HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN))) Response = 1;
//		else Response = -1;
//	}
//
////	while((HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)));
//	return Response;
//}

//uint8_t DHT22_Read(void) {
//	uint8_t i, j;
//
//	for(j=0; j<8; j++) {
//		while(!(HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)));
//		HAL_Delay(40);
//
//		if(!(HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN))) {
//			i &= ~(1<<(7-j));
//		} else {
//			i |= (1<<(7-j));
//		}
////		while((HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)));
//	}
//	return i;
//}

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
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim2);
  HAL_UART_Receive_IT(&huart2, &rx_data, 1);

  if(HAL_UART_Receive_DMA(&huart3,mise_buffer,32)!=HAL_OK)
  {
  	  printf("fail\r\n");
  }


  write_MISE("Passive");
//  HAL_Delay(2000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  if(loop_value == 1)
//		  printf("loop.\r\n");
//	  else if(loop_value == 2)
//		  printf("loop..\r\n");
//	  else
//		  printf("loop...\r\n");
//	  if(loop_value == 3)
//		  loop_value = 0;
//	  loop_value += 1;
//	  HAL_Delay(500);
//
//	  if (HAL_GPIO_ReadPin(Sensor_GPIO_Port, Sensor_Pin))
//		  printf("Detected!!!\r\n");
//	  else
//		  printf("No Detected...\r\n");


//	  if(led_status == TRUE) {
//		  led_off(URLED2_GPIO_Port, URLED2_Pin);
//	  } else {
//		  led_on(URLED2_GPIO_Port, URLED2_Pin);
//	  }

//	  if(HAL_UART_Receive(&huart2, &a, 4, 10) == HAL_OK) {
//		  HAL_UART_Transmit(&huart2, &a, 4, 10);
//	  }
//	  HAL_UART_Transmit(&huart2, &a, COUNTOF(a), 10);
//	  HAL_Delay(500);
//	  HAL_UART_Transmit(&huart2, (uint8_t *)data, (COUNTOF(data)-1), 50);
//	  scanf("%c", rx_data);
//	  HAL_Delay(500);

//	  DHT22_Start();
//	  Presence = DHT22_Check_Response();
//	  Rh_byte1 = DHT22_Read();
//	  Rh_byte2 = DHT22_Read();
//	  Temp_byte1 = DHT22_Read();
//	  Temp_byte2 = DHT22_Read();
//	  SUM = DHT22_Read();
//
//	  TEMP = ((Temp_byte1<<8)|Temp_byte2);
//	  RH = ((Rh_byte1<<8)|Rh_byte2);
//
//	  Temperature = (float)(TEMP/10);
//	  Humidity = (float)(RH/10);
//	  printf("%d\r\n", TEMP);   //65535
//	  printf("%d\r\n", Temperature);

//	  HAL_Delay(3000);


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	  printf("Main Loop...\r\n");
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 35999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 2999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 32000-1;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 5000-1;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */
  HAL_TIM_Base_Start_IT(&htim16);
  /* USER CODE END TIM16_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel2_3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|URLED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : URBTN_Pin */
  GPIO_InitStruct.Pin = URBTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(URBTN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 URLED2_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_1|URLED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Sensor_Pin */
  GPIO_InitStruct.Pin = Sensor_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Sensor_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == URBTN_Pin) {
    if(led_status == FALSE) {
    	led_status = TRUE;
    } else {
    	led_status = FALSE;
    }
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if(huart->Instance == USART2) {
		printf("Key Press!!!\r\n");
		HAL_UART_Receive_IT(&huart2, &rx_data, 1);
		buf[list_value] = rx_data;
		list_value++;
		if(rx_data == 'p') {
			HAL_UART_Transmit(&huart2, &buf, (list_value-1), 1);
			for(int i=0; i<list_value; i++) {
				buf[i] = 'null';
			}
			list_value = 0;
		}
//		HAL_UART_Transmit(&huart2, &rx_data, 1, 1);
	}

	if(huart->Instance == USART3) {
		print_MISE();
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART3)
	{

	}
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	printf("TIMER_Interrupt!!!\r\n");
//	if(htim->Instance==TIM16)
//	{
//		printf("TIMER16_Interrupt!!!\r\n");
//		write_MISE("Read");
//		printf("TIMER EXIT!!!\r\n");
//	}
//
//}

void print_MISE(void)
{
	uint16_t combine_value, check_byte_receive, check_byte_calculate=0;

	check_byte_receive=mise_buffer[30]<<8|mise_buffer[31];
	for(uint8_t i=0;i<30;i++)
	{
		check_byte_calculate+=mise_buffer[i];
	}

	if(check_byte_receive==check_byte_calculate)
	{
		printf("PM1.0 : %d	",(combine_value=(mise_buffer[10]<<8)|mise_buffer[11]));
		printf("PM2.5 : %d	",(combine_value=(mise_buffer[12]<<8)|mise_buffer[13]));
		printf("PM10 : %d	",(combine_value=(mise_buffer[14]<<8)|mise_buffer[15]));
		printf("0.3um : %d	",(combine_value=(mise_buffer[16]<<8)|mise_buffer[17]));
		printf("0.5um : %d	",(combine_value=(mise_buffer[18]<<8)|mise_buffer[19]));
		printf("1.0um : %d	",(combine_value=(mise_buffer[20]<<8)|mise_buffer[21]));
		printf("2.5um : %d	",(combine_value=(mise_buffer[22]<<8)|mise_buffer[23]));
		printf("5.0um : %d	",(combine_value=(mise_buffer[24]<<8)|mise_buffer[25]));
		printf("10.0um : %d\r\n",(combine_value=(mise_buffer[26]<<8)|mise_buffer[27]));
	}
	else
	{
	}
}

void write_MISE(char* cmd)
{
	uint16_t verify_byte=0;

	printf("Enter the write_MISE\r\n");

	if(strcmp(cmd,"Read")==0)
	{
		mise_send_buffer[2]=0xe2;
		mise_send_buffer[3]=0x00;
		mise_send_buffer[4]=0x00;
		printf("if's Read\r\n");
	}
	else if(strcmp(cmd,"Passive")==0)
	{
		mise_send_buffer[2]=0xe1;
		mise_send_buffer[3]=0x00;
		mise_send_buffer[4]=0x00;
		printf("if's Passive\r\n");
	}
	else if(strcmp(cmd,"Active")==0)
	{
		mise_send_buffer[2]=0xe1;
		mise_send_buffer[3]=0x00;
		mise_send_buffer[4]=0x01;
		printf("if's Active\r\n");

	}
	else if(strcmp(cmd,"Sleep")==0)
	{
		mise_send_buffer[2]=0xe4;
		mise_send_buffer[3]=0x00;
		mise_send_buffer[4]=0x00;
		printf("if's Sleep\r\n");
	}
	else if(strcmp(cmd,"WakeUp")==0)
	{
		mise_send_buffer[2]=0xe4;
		mise_send_buffer[3]=0x00;
		mise_send_buffer[4]=0x01;
		printf("if's WakeUp\r\n");
	}
	for(uint8_t i=0;i<5;i++)
	{
		verify_byte+=mise_send_buffer[i];
	}
	mise_send_buffer[5]=verify_byte>>8;
	mise_send_buffer[6]=verify_byte;

	while(HAL_UART_GetState(&huart3)!=HAL_UART_STATE_READY)
	{
	}

	if(HAL_UART_Transmit_IT(&huart3,(uint8_t*)mise_send_buffer,7)!=HAL_OK)
	{

	}
	if(strcmp(cmd,"Read")==0)
	{
		while(HAL_UART_GetState(&huart3)!=HAL_UART_STATE_READY)
		{
		}
		if(HAL_UART_Receive_IT(&huart3,mise_send_buffer, 32)!=HAL_OK)
		{

		}
	}

	printf("%d\r\n", mise_send_buffer);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
