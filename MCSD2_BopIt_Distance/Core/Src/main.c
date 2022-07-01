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
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <float.h>
#include "Si1153.h"
#include "wifible.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define measure_event_flag 1
#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for measureEvent */
osEventFlagsId_t measureEventHandle;
const osEventFlagsAttr_t measureEvent_attributes = {
  .name = "measureEvent"
};
/* USER CODE BEGIN PV */
#define Si1153_ADC_LUT_len 17
static const float Si1153_ADC_LUT[Si1153_ADC_LUT_len] = {
										0.0,
										54.2,
										178.0,
										220.0,
										267.2,
										284.2,
										322.2,
										345.4,
										370.0,
										390.4,
										404.0,
										413.6,
										435.2,
										440.2,
										457.6,
										464.4,
										494.8
										};

/**
 * @brief Used in conjunction with TIM6 to debounce the button after an interrupt
 */
uint8_t button_ready = 1;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM6_Init(void);
void StartDefaultTask(void *argument);
int _write(int file, char *ptr, int len);

/* USER CODE BEGIN PFP */
uint8_t Si1153_ADC_to_cm(uint16_t ADC_value);
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
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

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of measureEvent */
  measureEventHandle = osEventFlagsNew(&measureEvent_attributes);

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
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 32000;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 999;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
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
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

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

  /*Configure GPIO pin : SHIELD_BUTTON_Pin */
  GPIO_InitStruct.Pin = SHIELD_BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SHIELD_BUTTON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

}

/* USER CODE BEGIN 4 */
uint8_t Si1153_ADC_to_cm(uint16_t ADC_value) {
	if (ADC_value > 500)
	{
		return 20;
	}

	float ADC_flt = (float) ADC_value;
	float smallest_diff = FLT_MAX;
	int smallest_diff_i = 0;

	for (int i = 0; i < Si1153_ADC_LUT_len; i++)
	{
		float current_diff = ADC_flt - Si1153_ADC_LUT[i];

		current_diff = (current_diff < 0) ? -current_diff : current_diff;

		if (current_diff < smallest_diff)
		{
			smallest_diff = current_diff;
			smallest_diff_i = i;
		}
	}

	return smallest_diff_i;
}

volatile uint8_t flag = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
  if (button_ready)
  {

	  HAL_UART_Transmit(&huart2, (uint8_t *) "Interrupt!\r\n", strlen("Interrupt!\r\n"), 500);
	  osEventFlagsSet(measureEventHandle, measure_event_flag);


	  button_ready = 0;

	  /* Disable the button until debounced, as reported by TIM6 */
	  HAL_TIM_Base_Start_IT(&htim6);
  }

}

/**
 * @brief Function that writes the char pointer to the UART Interface.
 * @param ptr: char pointer to the String
 * @param len: length of the String
 * @retval EIO: returns IO Error
 * @retval len: returns the length of the String and means no error
 * @return returns the length of the string or in case of erros -1 or IO Error
 */
int _write(int file, char *ptr, int len) {
	if (file == STDOUT_FILENO || file == STDERR_FILENO) {
		//Write String over UART to Terminal
		if (HAL_UART_Transmit(&huart2, (uint8_t*) ptr, len, HAL_MAX_DELAY)
				== HAL_OK) {
			return len;
		} else {
			//return IO Error
			return EIO;
		}
	}
	//Return File Descriptor Error
	errno = EBADF;
	return -1;
}

/**
 * @brief Wrapper funtion to load the desired settings into the Si1153.
 *
 * @retval Returns HAL_OK if every initialisation step was successful. HAL_ERROR otherwise.
 */
HAL_StatusTypeDef initialize_sensor() {
	uint16_t error_counter = 0;

	if (Si1153_set_param(&hi2c1, SI1153_CHAN_LIST_PARAM_ADDR, SI1153_CHAN_LIST_DEFAULT) != HAL_OK)
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "Error setting channel list\r\n", strlen("Error setting channel list\r\n"), 1000);
		error_counter++;
	}
	else
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "Channel list set\r\n", strlen("Channel list set\r\n"), 1000);
	}

	if (Si1153_set_param(&hi2c1, SI1153_MEASRATE_L_PARAM_ADDR, SI1153_MEASRATE_L_DEFAULT) != HAL_OK)
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "Error setting measure rate l\r\n", strlen("Error setting measure rate l\r\n"), 1000);
		error_counter++;
	}
	else
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "Measure rate l set\r\n", strlen("Measure rate l set\r\n"), 1000);
	}

	if (Si1153_set_param(&hi2c1, SI1153_MEASCOUNT0_PARAM_ADDR, SI1153_MEASCOUNT0_DEFAULT) != HAL_OK)
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "Error setting measure count\r\n", strlen("Error setting measure count\r\n"), 1000);
		error_counter++;
	}
	else
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "Measure count set\r\n", strlen("Measure count set\r\n"), 1000);
	}

	if (Si1153_set_param(&hi2c1, SI1153_ADCCONFIG0_PARAM_ADDR, SI1153_ADCCONFIG_DEFAULT) != HAL_OK)
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "Error setting channel 0 ADC config\r\n", strlen("Error setting channel 0 ADC config\r\n"), 1000);
		error_counter++;
	}
	else
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "Channel 0 ADC config set\r\n", strlen("Channel 0 ADC config set\r\n"), 1000);
	}

	if (Si1153_set_param(&hi2c1, SI1153_ADCSENS0_PARAM_ADDR, SI1153_ADCSENS_DEFAULT) != HAL_OK)
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "Error setting channel 0 ADC sens\r\n", strlen("Error setting channel 0 ADC sens\r\n"), 1000);
		error_counter++;
	}
	else
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "Channel 0 ADC sens set\r\n", strlen("Channel 0 ADC sens set\r\n"), 1000);
	}

	if (Si1153_set_param(&hi2c1, SI1153_ADCPOST0_PARAM_ADDR, SI1153_ADCPOST_DEFAULT) != HAL_OK)
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "Error setting channel 0 ADC post\r\n", strlen("Error setting channel 0 ADC post\r\n"), 1000);
		error_counter++;
	}
	else
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "Channel 0 ADC post set\r\n", strlen("Channel 0 ADC post set\r\n"), 1000);
	}

	if (Si1153_set_param(&hi2c1, SI1153_MEASCONFIG0_PARAM_ADDR, SI1153_MEASCONFIG_DEFAULT) != HAL_OK)
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "Error setting channel 0 measurement config\r\n", strlen("Error setting channel 0 measurement config\r\n"), 1000);
		error_counter++;
	}
	else
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "Channel 0 measurement config set\r\n", strlen("Channel 0 measurement config set\r\n"), 1000);
	}

	if (Si1153_start_autonomous_mode(&hi2c1) != HAL_OK)
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "Error: could not initiate autonomous measurements\r\n", strlen("Error: could not initiate autonomous measurements\r\n"), 1000);
		error_counter++;
	}
	else
	{
		HAL_UART_Transmit(&huart2, (uint8_t *) "Autonomous measurements initiated\r\n", strlen("Autonomous measurements initiated\r\n"), 1000);
	}

	return 0 != error_counter ? HAL_ERROR : HAL_OK;
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	uint8_t msg[50];


	wifible_init(&huart1);
	connectWifi("Florian Handy", "florianparzer");

	while(1)
	{
		if (initialize_sensor() == HAL_OK)
		{
			break;
		}
	}


	/* Infinite loop */

	for(;;) {

		//while(!flag);

		osEventFlagsWait(measureEventHandle, measure_event_flag, 0, osWaitForever);

		uint16_t chan_0 = 0;

		if (Si1153_read_channel_0_16bit(&hi2c1, &chan_0) != HAL_OK)
		{
			HAL_UART_Transmit(&huart2, (uint8_t *) "Error reading channel 0\r\n", strlen("Error reading channel 0\r\n"), 1000);
		}
		else
		{
			/* Interfacing with the WiFi module goes here */
			int cm = Si1153_ADC_to_cm(chan_0);
			int message = 0;

			if (cm <= 7) {
				message = DIST_LESS_THAN_5;
			} else if ((cm > 7) && (cm <=13)) {
				message = DIST_10;
			} else if (cm > 13) {
				message = DIST_MORE_THAN_15;
			}

			sendHttpPost("192.168.43.198", "/api/challenges", 1, message);
			sprintf((char *) msg, "%05d - % 2d cm\r\n", chan_0, cm);
			HAL_UART_Transmit(&huart2, msg, strlen((char *) msg), 1000);
		}

		flag = 0;


		//osDelay(2000);
	}
  /* USER CODE END 5 */
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
  else if (htim->Instance == TIM6)
  {
	  /* TIM6 is the debounce timer */
	  button_ready = 1;
	  HAL_TIM_Base_Stop_IT(&htim6);
  }
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

