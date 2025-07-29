/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED.h"
#include "devRTC.h"
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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId OLEDshowHandle;
osThreadId RTCAlarmHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void vTaskOLEDshow(void const * argument);
void vTaskRTCAlarm(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

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
  /* definition and creation of OLEDshow */
  osThreadDef(OLEDshow, vTaskOLEDshow, osPriorityIdle, 0, 128);
  OLEDshowHandle = osThreadCreate(osThread(OLEDshow), NULL);

  /* definition and creation of RTCAlarm */
  osThreadDef(RTCAlarm, vTaskRTCAlarm, osPriorityIdle, 0, 128);
  RTCAlarmHandle = osThreadCreate(osThread(RTCAlarm), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_vTaskOLEDshow */
/**
  * @brief  Function implementing the OLEDshow thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_vTaskOLEDshow */
void vTaskOLEDshow(void const * argument)
{
  /* USER CODE BEGIN vTaskOLEDshow */
	
	OLED_Init();
	OLED_Clear();
	OLED_ShowString(1,4,"RTC Clock");
	OLED_ShowString(4,1,"Alarm:");
  /* Infinite loop */
  for(;;)
  {
		DisplayCountdown();
		OLED_ShowSignedNum(4, 7,  remainingHour, 2);
		OLED_ShowSignedNum(4, 10, remainingMin , 2);
		OLED_ShowSignedNum(4, 13, remainingSec , 2);
		if(AlarmTrigger == 1)				//每当有闹钟响起，就在OLED上闪烁显示一下已触发
		{
				OLED_ShowString(2,1,"Alarm has Triger");
				DisplayCountdown();
				OLED_ShowSignedNum(4, 7,  remainingHour, 2);
				OLED_ShowSignedNum(4, 10, remainingMin , 2);
				OLED_ShowSignedNum(4, 13, remainingSec , 2);
				osDelay(500);
				DisplayCountdown();
				OLED_ShowSignedNum(4, 7,  remainingHour, 2);
				OLED_ShowSignedNum(4, 10, remainingMin , 2);
				OLED_ShowSignedNum(4, 13, remainingSec , 2);
			  OLED_ShowString(2,1,"                ");
				osDelay(500);
				DisplayCountdown();
				OLED_ShowSignedNum(4, 7,  remainingHour, 2);
				OLED_ShowSignedNum(4, 10, remainingMin , 2);
				OLED_ShowSignedNum(4, 13, remainingSec , 2);
			  OLED_ShowString(2,1,"Alarm has Triger");
				osDelay(500);
				DisplayCountdown();
				OLED_ShowSignedNum(4, 7,  remainingHour, 2);
				OLED_ShowSignedNum(4, 10, remainingMin , 2);
				OLED_ShowSignedNum(4, 13, remainingSec , 2);			
				OLED_ShowString(2,1,"                ");
			
				AlarmTrigger = 0;
		}			
		
    osDelay(2);
  }
  /* USER CODE END vTaskOLEDshow */
}

/* USER CODE BEGIN Header_vTaskRTCAlarm */
/**
* @brief Function implementing the RTCAlarm thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_vTaskRTCAlarm */
void vTaskRTCAlarm(void const * argument)
{
  /* USER CODE BEGIN vTaskRTCAlarm */
  /* Infinite loop */
  for(;;)
  {
		SetAlarm();
    osDelay(2);
  }
  /* USER CODE END vTaskRTCAlarm */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

