#include "headfile.h"
char text[20];
RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef sDate = {0};


void lcd_show()
{
	HAL_RTC_GetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&sDate,RTC_FORMAT_BIN);
	sprintf(text,"      text      ");
	LCD_DisplayStringLine(Line0,(uint8_t *)text);
	sprintf(text,"Time %2d:%2d:%2d   ",sTime.Hours,sTime.Minutes,sTime.Seconds);
	LCD_DisplayStringLine(Line3,(uint8_t *)text);
	sprintf(text,"    %d-%d-%d-%d   ",sDate.Year,sDate.Month,sDate.Date,sDate.WeekDay);
	LCD_DisplayStringLine(Line5,(uint8_t *)text);
}












