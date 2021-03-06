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
#include "examples.h"
#include <stdio.h>

#include "ai_datatypes_defines.h"
#include "ai_platform.h"
#include "sine_model.h"
#include "sine_model_data.h"

#include "network.h"
#include "network_data.h"

#include "lsm6dsl.h"
#include "custom_bus.h"
//#include  "b_l475e_iot01a1_bus.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
//short set_urled1 = false;

ai_handle network;
float aiInData[AI_NETWORK_IN_1_SIZE];
float aiOutData[AI_NETWORK_OUT_1_SIZE];
uint8_t activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];
const char* activities[AI_NETWORK_OUT_1_SIZE] = {
		"stationary", "walking", "running"
};

LSM6DSL_Object_t MotionSensor;
volatile uint32_t dataRdyIntReceived;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_CRC_Init(void);
/* USER CODE BEGIN PFP */
static void AI_Init(ai_handle w_addr, ai_handle act_addr);
static void AI_Run(float *pIn, float *pOut);
static uint32_t argmax(const float * values, uint32_t len);
static void MEMS_Init(void);
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
//	char buf[50];
//	int buf_len = 0;
//	ai_error ai_err;
//	ai_i32 nbatch;
//	uint32_t timestamp;
//	float y_val;
//
//	AI_ALIGNED(4) ai_u8 activations[AI_SINE_MODEL_DATA_ACTIVATIONS_SIZE];
//
//	AI_ALIGNED(4) ai_i8 in_data[AI_SINE_MODEL_IN_1_SIZE_BYTES];
//	AI_ALIGNED(4) ai_i8 out_data[AI_SINE_MODEL_OUT_1_SIZE_BYTES];
//
//	ai_handle sine_model = AI_HANDLE_NULL;
//
//	ai_buffer ai_input[AI_SINE_MODEL_IN_NUM] = AI_SINE_MODEL_IN;
//	ai_buffer ai_output[AI_SINE_MODEL_OUT_NUM] = AI_SINE_MODEL_OUT;
//
//	ai_network_params ai_params = {
//		AI_SINE_MODEL_DATA_WEIGHTS(ai_sine_model_data_weights_get()),
//		AI_SINE_MODEL_DATA_ACTIVATIONS(activations)
//	};
//
//	ai_input[0].n_batches = 1;
//	ai_input[0].data = AI_HANDLE_PTR(in_data);
//	ai_output[0].n_batches = 1;
//	ai_output[0].data = AI_HANDLE_PTR(out_data);
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
  MX_USART1_UART_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */
//  	buf_len = sprintf(buf, "\r\n\r\nSTM32 X-Cube-AI test\r\n");
//    HAL_UART_Transmit(&huart1, (uint8_t *)buf, buf_len, 100);
//
//    ai_err = ai_sine_model_create(&sine_model, AI_SINE_MODEL_DATA_CONFIG);
//    if (ai_err.type != AI_ERROR_NONE) {
//  	  buf_len = sprintf(buf, "Error: could not create NN instance\r\n");
//  	  HAL_UART_Transmit(&huart1, (uint8_t *)buf, buf_len, 100);
//  	  while(1);
//    }
//
//    if (!ai_sine_model_init(sine_model, &ai_params)) {
//  	  buf_len = sprintf(buf, "Error: could not initialize NN\r\n");
//  	  HAL_UART_Transmit(&huart1, (uint8_t *)buf, buf_len, 100);
//  	  while(1);
//    }
  static void AI_Init(ai_handle w_addr, ai_handle act_addr) {
  	  ai_error err;

  	  err = ai_network_create(&network, AI_NETWORK_DATA_CONFIG);
  	  if (err.type != AI_ERROR_NONE) {
  		  printf("ai_network_create error - type=%d code=%d\r\n", err.type, err.code);
  		  Error_Handler();
  	  }

  	  const ai_network_params params = {
  			  AI_NETWORK_DATA_WEIGHTS(w_addr),
  			  AI_NETWORK_DATA_ACTIVATIONS(act_addr)
  	  };

  	  if (!ai_network_init(network, &params)) {
  		  err = ai_network_get_error(network);
  		  printf("ai_network_init error - type=%d code=%d\r\n", err.type, err.code);
  		  Error_Handler();
  	  }
  }

  dataRdyIntReceived = 0;
  MEMS_Init();
  AI_Init(ai_network_data_weights_get(), activations);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//  float f = 0.01;
  uint32_t write_index = 0;
  while (1)
  {
//	  for(uint32_t i=0; i<AI_SINE_MODEL_IN_1_SIZE; i++) {
//	  		  ((ai_float *)in_data)[i] = (ai_float)2.0f;
//	  	  }
//
//	  	  nbatch = ai_sine_model_run(sine_model, &ai_input[0], &ai_output[0]);
//	  	  if(nbatch != 1) {
//	  		  buf_len = sprintf(buf, "Error: could not run inference \r\n");
//	  		  HAL_UART_Transmit(&huart1, (uint8_t *)buf, buf_len, 100);
//	  	  }
//
//	  	  y_val = ((float *)out_data)[0];
//
//	  	  buf_len = sprintf(buf, "Output: %f \r\n", y_val);
//	  	  HAL_UART_Transmit(&huart1, (uint8_t *)buf, buf_len, 100);
//
//	  	  HAL_Delay(500);
	  if (dataRdyIntReceived != 0) {
		  dataRdyIntReceived = 0;
		  LSM6DSL_Axes_t acc_axes;
		  LSM6DSL_ACC_GetAxes(&MotionSensor, &acc_axes);
		  aiInData[write_index + 0] = (float)acc_axes.x / 4000.0f;
		  aiInData[write_index + 1] = (float)acc_axes.y / 4000.0f;
		  aiInData[write_index + 2] = (float)acc_axes.z / 4000.0f;
		  write_index += 3;

		  if(write_index == AI_NETWORK_IN_1_SIZE) {
			  write_index = 0;

			  printf("Running inference\r\n");
			  AI_Run(aiInData, aiOutData);

			  for (uint32_t i=0; i<AI_NETWORK_OUT_1_SIZE; i++) {
				  printf("%8.6f ", aiOutData[i]);
			  }
			  uint32_t class = argmax(aiOutData, AI_NETWORK_OUT_1_SIZE);
			  printf(": %d - %s\r\n", (int)class, activities[class]);
		  }
	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	  printf("main loop\r\n");
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C2;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(URLED1_GPIO_Port, URLED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(URLED2_GPIO_Port, URLED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : URBTN2_Pin */
  GPIO_InitStruct.Pin = URBTN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(URBTN2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : URLED1_Pin */
  GPIO_InitStruct.Pin = URLED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(URLED1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : URLED2_Pin */
  GPIO_InitStruct.Pin = URLED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(URLED2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD11 URBTN1_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_11|URBTN1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 9, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

static void AI_Init(ai_handle w_addr, ai_handle act_addr) {
	  ai_error err;

	  err = ai_network_create(&network, AI_NETWORK_DATA_CONFIG);
	  if (err.type != AI_ERROR_NONE) {
		  printf("ai_network_create error - type=%d code=%d\r\n", err.type, err.code);
		  Error_Handler();
	  }

	  const ai_network_params params = {
			  AI_NETWORK_DATA_WEIGHTS(w_addr),
			  AI_NETWORK_DATA_ACTIVATIONS(act_addr)
	  };

	  if (!ai_network_init(network, &params)) {
		  err = ai_network_get_error(network);
		  printf("ai_network_init error - type=%d code=%d\r\n", err.type, err.code);
		  Error_Handler();
	  }
}

static void AI_Run(float *pIn, float *pOut) {
	  ai_i32 batch;
	  ai_error err;

	  ai_buffer ai_input[AI_NETWORK_IN_NUM] = AI_NETWORK_IN;
	  ai_buffer ai_output[AI_NETWORK_OUT_NUM] = AI_NETWORK_OUT;

	  ai_input[0].n_batches = 1;
	  ai_input[0].data = AI_HANDLE_PTR(pIn);
	  ai_output[0].n_batches = 1;
	  ai_output[0].data = AI_HANDLE_PTR(pOut);

	  batch = ai_network_run(network, ai_input, ai_output);
	  if (batch != 1) {
		  err = ai_network_get_error(network);
		  printf("AI ai_network_run error - type=%d code=%d\r\n", err.type, err.code);
		  Error_Handler();
	  }
}

static uint32_t argmax(const float * values, uint32_t len) {
	  float max_value = values[0];
	  uint32_t max_index = 0;
	  for (uint32_t i=1; i<len; i++) {
		  if (values[i] > max_value) {
			  max_value = values[i];
			  max_index = i;
		  }
	  }
	  return max_index;
}
static void MEMS_Init(void)
{
  LSM6DSL_IO_t io_ctx;
  uint8_t id;
  LSM6DSL_AxesRaw_t axes;

  /* Link I2C functions to the LSM6DSL driver */
  io_ctx.BusType     = LSM6DSL_I2C_BUS;
  io_ctx.Address     = LSM6DSL_I2C_ADD_L;
  io_ctx.Init        = BSP_I2C2_Init;
  io_ctx.DeInit      = BSP_I2C2_DeInit;
  io_ctx.ReadReg     = BSP_I2C2_ReadReg;
  io_ctx.WriteReg    = BSP_I2C2_WriteReg;
  io_ctx.GetTick     = BSP_GetTick;
  LSM6DSL_RegisterBusIO(&MotionSensor, &io_ctx);

  /* Read the LSM6DSL WHO_AM_I register */
  LSM6DSL_ReadID(&MotionSensor, &id);
  printf("id : %d\n", &id);
  printf("lsm6dsl_id : %d\n", LSM6DSL_ID);
  if (id != LSM6DSL_ID) {
    Error_Handler();
  }

  /* Initialize the LSM6DSL sensor */
  LSM6DSL_Init(&MotionSensor);

  /* Configure the LSM6DSL accelerometer (ODR, scale and interrupt) */
  LSM6DSL_ACC_SetOutputDataRate(&MotionSensor, 26.0f); /* 26 Hz */
  LSM6DSL_ACC_SetFullScale(&MotionSensor, 4);          /* [-4000mg; +4000mg] */
  LSM6DSL_ACC_Set_INT1_DRDY(&MotionSensor, ENABLE);    /* Enable DRDY */
  LSM6DSL_ACC_GetAxesRaw(&MotionSensor, &axes);        /* Clear DRDY */

  /* Start the LSM6DSL accelerometer */
  LSM6DSL_ACC_Enable(&MotionSensor);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_11) {
    dataRdyIntReceived++;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
