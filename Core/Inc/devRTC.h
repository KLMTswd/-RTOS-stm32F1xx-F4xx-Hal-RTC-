#ifndef __DEVRTC_H__
#define __DEVRTC_H__

#include "main.h"
#include "rtc.h"

/*我们自己定义的闹钟时、分、秒结构体*/
typedef struct
{
  uint8_t Hours;          
  uint8_t Minutes;          
  uint8_t Seconds;          
  uint8_t TimeFormat;      
  uint32_t SubSeconds;      
  uint32_t SecondFraction;  
  uint32_t DayLightSaving; 
  uint32_t StoreOperation; 
	uint8_t WeekDay; 
  uint8_t Month;   
  uint8_t Date;    
  uint8_t Year; 
}	aTime;

extern uint32_t currentSec, alarmSec, remainingSec;
extern uint8_t remainingMin, remainingHour;
extern volatile uint8_t AlarmTrigger;

void dev_rtc_alarm(void);
void SetAlarm(void);
void DisplayCountdown(void);

#endif
