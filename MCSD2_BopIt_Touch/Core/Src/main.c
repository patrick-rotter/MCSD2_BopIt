/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include "CY8C201A0.h"
#include "wifible.h"
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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* Definitions for touchSensorTask */
osThreadId_t touchSensorTaskHandle;
const osThreadAttr_t touchSensorTask_attributes = {
  .name = "touchSensorTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for WiFiDispatcherT */
osThreadId_t WiFiDispatcherTHandle;
const osThreadAttr_t WiFiDispatcherT_attributes = {
  .name = "WiFiDispatcherT",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for touch_event_queue */
osMessageQueueId_t touch_event_queueHandle;
const osMessageQueueAttr_t touch_event_queue_attributes = {
  .name = "touch_event_queue"
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
void start_touch_sensor_task(void *argument);
void start_wifi_dispatcher_task(void *argument);

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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of touch_event_queue */
  touch_event_queueHandle = osMessageQueueNew (16, sizeof(uint16_t), &touch_event_queue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of touchSensorTask */
  touchSensorTaskHandle = osThreadNew(start_touch_sensor_task, NULL, &touchSensorTask_attributes);

  /* creation of WiFiDispatcherT */
  WiFiDispatcherTHandle = osThreadNew(start_wifi_dispatcher_task, NULL, &WiFiDispatcherT_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00707CBB;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  huart2.Init.BaudRate = 115200;
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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_start_touch_sensor_task */
/**
  * @brief  Function implementing the touchSensorTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_start_touch_sensor_task */
void start_touch_sensor_task(void *argument)
{
  /* USER CODE BEGIN 5 */
	uint8_t id_msg[50] = {0};


	HAL_StatusTypeDef init_result = CY8_init(	&hi2c1,
												&huart2,
												CY8C201A0_CAPSENSE_0_CONFIG_TOP | CY8C201A0_CAPSENSE_0_CONFIG_BOTTOM,
												CY8C201A0_CAPSENSE_1_CONFIG_ALL,
												5,
												0x10);

	if (HAL_OK == init_result)
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "CY8C201A0 initiated\r\n", strlen("CY8C201A0 initiated\r\n"), 1000);
	}
	else
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "CY8C201A0 initiation failed\r\n", strlen("CY8C201A0 initiation failed\r\n"), 1000);
	}

	/* Used to register when a button has been released */
	uint8_t top_button_prev = 0;
	uint8_t bottom_button_prev = 0;
	uint8_t mid_button_prev = 0;

	/* Infinite loop */
	for(;;) {

		uint8_t cs0_read = 0;
		uint8_t cs1_read = 0;

		if (CY8_read_capsense_0(&hi2c1, &cs0_read) != HAL_OK)
		{
			HAL_UART_Transmit(&huart2, (uint8_t *) "i2c error on cp 0 read\r\n", strlen("i2c error on cp 0 read\r\n"), 1000);
			osDelay(1450);
			continue;

		}

		if (CY8_read_capsense_1(&hi2c1, &cs1_read) != HAL_OK)
		{
			HAL_UART_Transmit(&huart2, (uint8_t *) "i2c error on cp 1 read\r\n", strlen("i2c error on cp 1 read\r\n"), 1000);
			osDelay(1450);
			continue;
		}

		uint8_t top_button_current = cs0_read & CY8C201A0_CAPSENSE_0_CONFIG_TOP;
		uint8_t bottom_button_current = cs0_read & CY8C201A0_CAPSENSE_0_CONFIG_BOTTOM;

		/* Rising edge */
		if (top_button_current && !top_button_prev)
		{
			uint16_t touch_msg = CY8C201A0_CAPSENSE_0_CONFIG_TOP;
			osMessageQueuePut(touch_event_queueHandle, &touch_msg, 0, osWaitForever);
		}

		if (bottom_button_current && !bottom_button_prev)
		{
			uint16_t touch_msg = CY8C201A0_CAPSENSE_0_CONFIG_BOTTOM;
			osMessageQueuePut(touch_event_queueHandle, &touch_msg, 0, osWaitForever);
		}

		if (cs1_read && !mid_button_prev)
		{
			uint16_t touch_msg = CY8C201A0_CAPSENSE_1_CONFIG_ALL;
			osMessageQueuePut(touch_event_queueHandle, &touch_msg, 0, osWaitForever);
		}


		top_button_prev = top_button_current;
		bottom_button_prev = bottom_button_current;
		mid_button_prev = cs1_read;


		osDelay(50);



/*
		sprintf((char *) id_msg, "Scanning i2c\r\n");
		HAL_UART_Transmit(&huart2, id_msg, strlen((char *) id_msg), 1000);

		for (int i = 0; i < 128; i++) {
			ret = HAL_I2C_IsDeviceReady(&hi2c1, i << 1, 3, 10);
			if (ret == HAL_OK) {
				sprintf((char *) id_msg, "Device found at: 0x%x\r\n", i);
				HAL_UART_Transmit(&huart2, id_msg, strlen((char *) id_msg), 1000);
			} else if (ret == HAL_ERROR) {
				HAL_UART_Transmit(&huart2, (uint8_t *) "- ", strlen((char *) "- "), 1000);
			} else if (ret == HAL_BUSY) {
				HAL_UART_Transmit(&huart2, (uint8_t *) "* ", strlen((char *) "* "), 1000);
			}
			osDelay(50);
		}

		sprintf((char *) id_msg, "Scanning done\r\n");
		HAL_UART_Transmit(&huart2, id_msg, strlen((char *) id_msg), 1000); */

	}
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_start_wifi_dispatcher_task */
/**
* @brief Function implementing the WiFiDispatcherT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_start_wifi_dispatcher_task */
void start_wifi_dispatcher_task(void *argument)
{
  /* USER CODE BEGIN start_wifi_dispatcher_task */
  /* Infinite loop */
  for(;;)
  {
    uint16_t message = 0;

    if (osMessageQueueGet(touch_event_queueHandle, &message, 0, osWaitForever) == osOK)
    {
    	switch (message)
    	{
    	case CY8C201A0_CAPSENSE_0_CONFIG_TOP:
    		HAL_UART_Transmit(&huart2, (uint8_t*) "Top button pressed\r\n", strlen("Top button pressed\r\n"), 1000);
    		break;
    	case CY8C201A0_CAPSENSE_0_CONFIG_BOTTOM:
    		HAL_UART_Transmit(&huart2, (uint8_t*) "Bottom button pressed\r\n", strlen("Bottom button pressed\r\n"), 1000);
    		break;
    	case CY8C201A0_CAPSENSE_1_CONFIG_ALL:
    		HAL_UART_Transmit(&huart2, (uint8_t*) "Mid button pressed\r\n", strlen("Mid button pressed\r\n"), 1000);
    		break;
    	default:
    		HAL_UART_Transmit(&huart2, (uint8_t*) "Invalid queue message\r\n", strlen("Invalid queue message\r\n"), 1000);
    		break;
    	}
    }


  }
  /* USER CODE END start_wifi_dispatcher_task */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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

