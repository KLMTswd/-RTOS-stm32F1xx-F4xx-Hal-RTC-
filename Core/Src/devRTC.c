#include "rtc.h"
#include "devRTC.h"
#include "OLED.h"


aTime Time;
aTime alarmTime;
aTime currentTime;


uint32_t currentSec, alarmSec, remainingSec;
uint8_t remainingMin, remainingHour;
volatile uint8_t second_time_flag = 1;
volatile uint8_t sAlarm_flag = 0;
volatile uint8_t AlarmTrigger = 0;


void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	

		sAlarm_flag = 1;				//只设立一个闹钟标志位，不在中断函数里面放太多东西
		AlarmTrigger = 1;
	
	// 再次启动闹钟中断事件
    if (HAL_RTC_SetAlarm_IT(hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }
	  HAL_RTC_SetAlarm_IT(hrtc,&sAlarm,RTC_FORMAT_BIN);
		
}



/****************************************************************************
**Description:			设置闹钟时间
**Input parameters:		
**Remark:		second_time_flag == 1表示第二次闹钟时间		
*****************************************************************************/

                      
void dev_set_alarm_time(void)
{
		if(second_time_flag == 1)
		{
				Time.Hours = 12;
				Time.Minutes = 12;
				Time.Seconds = 20;
				second_time_flag = 0;   // 关闭，意思就是每次只设置一次
		}	
		Time.Seconds += 10;
		if(Time.Seconds >= 60)     //多次对秒，分是否超过上限而选择进位，防止无效溢出，当然会出现second ==70时而只是minute +1，剩下的10秒就丢掉的情况，不管了
		{
				Time.Minutes += 1;
				if(Time.Minutes >= 60)     
				{
						Time.Hours += 1;
						if(Time.Hours >= 24)
						{
								Time.Hours = 0;
						}	
						Time.Minutes = 0;		
				}	
				Time.Seconds = 0;
		}			
		
		sAlarm.AlarmTime.Hours = Time.Hours;
		sAlarm.AlarmTime.Minutes = Time.Minutes;
		sAlarm.AlarmTime.Seconds = Time.Seconds;
}


/****************************************************************************
**Description:			闹钟函数
**Input parameters:	
**Remark:				闹钟掩码和日期选择是不可避免的
*****************************************************************************/

void dev_rtc_alarm(void)
{
		if(sAlarm_flag == 1)    //收到闹钟标志位，说明可以开始来设置第二个闹钟
		{
				HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			
				dev_set_alarm_time();
			
				sAlarm.Alarm = RTC_ALARM_A;
				
				if(HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
				{
						Error_Handler();
				}	
				
				sAlarm_flag = 0;
		}

}	

/****************************************************************************
**Description:			自定义了读取时间/设定时间的函数
**Input parameters:	
**Remark:				
*****************************************************************************/

void RTC_GetTime(aTime* time)
{
    HAL_RTC_GetTime(&hrtc, (RTC_TimeTypeDef *)&time->Hours,   RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, (RTC_DateTypeDef *)&time->WeekDay, RTC_FORMAT_BIN);
}

void RTC_SetTime(aTime* time)
{
    HAL_RTC_SetTime(&hrtc, (RTC_TimeTypeDef *)&time->Hours,   RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, (RTC_DateTypeDef *)&time->WeekDay, RTC_FORMAT_BIN);
}

/****************************************************************************
**Description:							F1系列设定闹钟
**Input parameters:	
**Remark:				
*****************************************************************************/

void SetAlarm(void)
{
		if(sAlarm_flag == 1)    //收到闹钟标志位，说明可以开始来设置第二个闹钟
		{
				RTC_AlarmTypeDef sAlarm = {0};
				aTime tim = {0};
										
				// 获取当前时间
				RTC_GetTime(&tim);

				alarmTime.Hours = tim.Hours;
				alarmTime.Minutes = tim.Minutes;				 /* 设置下次闹钟提醒时间是当前时间的10秒分之后 */
				alarmTime.Seconds = tim.Seconds + 10;  
			

				if (sAlarm.AlarmTime.Seconds >= 60) 			//边界情况 · 防止溢出
				{
						alarmTime.Seconds -= 60;
						alarmTime.Minutes += 1;
				}
				
				if (sAlarm.AlarmTime.Minutes >= 60)         
				{
						alarmTime.Minutes -= 60;
						alarmTime.Hours += 1;
				}

				
				sAlarm.AlarmTime.Hours = alarmTime.Hours;
				sAlarm.AlarmTime.Minutes = alarmTime.Minutes;
				sAlarm.AlarmTime.Seconds = alarmTime.Seconds;	
				sAlarm.Alarm = RTC_ALARM_A;	
									
				if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
				{
							Error_Handler();
				}
				
				sAlarm_flag = 0;
				
				__HAL_RTC_ALARM_EXTI_ENABLE_IT();  // 使能EXTI中断
				__HAL_RTC_ALARM_EXTI_ENABLE_RISING_EDGE();  // 上升沿触
				
			}
}	


void DisplayCountdown(void)
{
    
    RTC_GetTime(&currentTime);
    
    currentSec = currentTime.Hours * 3600 + currentTime.Minutes * 60 + currentTime.Seconds;
    alarmSec = alarmTime.Hours * 3600 + alarmTime.Minutes * 60 + alarmTime.Seconds;
    
    if(alarmSec > currentSec) {
        remainingSec = alarmSec - currentSec;
    } else {
        remainingSec = 0;
    }
    
    remainingHour = remainingSec / 3600;
    remainingSec %= 3600;
    remainingMin = remainingSec / 60;
    remainingSec %= 60;
    
 
}	

