#include "headfile.h"
char text[20];
uint32_t fre,Input_capture;
uint32_t free,Input_capturee;
void lcd_show()
{
	sprintf(text,"      text      ");
	LCD_DisplayStringLine(Line0,(uint8_t *)text);
	sprintf(text,"   fre1:   %d   ",fre);
	LCD_DisplayStringLine(Line3,(uint8_t *)text);
	sprintf(text,"   fre2:   %d   ",free);
	LCD_DisplayStringLine(Line5,(uint8_t *)text);
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)  //��д�жϻص�����
{
	if(htim->Instance == TIM16)   //�Ƚ��ж�ʱ����Դ�ж�
	{
		Input_capture = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);   // ����ͨ��ֵ
		TIM16->CNT = 0;
		fre = (80000000) / (80 * Input_capture);
		
	}
	
	if(htim->Instance == TIM2)
	{
		Input_capturee = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		TIM2->CNT = 0;
		free = (80000000) / (800 * Input_capturee);
	}
	
}






