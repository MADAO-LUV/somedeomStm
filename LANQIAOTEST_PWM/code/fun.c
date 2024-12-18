#include "headfile.h"
char text[20];
uint32_t fre,Input_capture;

void lcd_show()
{
	sprintf(text,"      text      ");
	LCD_DisplayStringLine(Line0,(uint8_t *)text);
	sprintf(text,"   fre:   %d   ",fre);
	LCD_DisplayStringLine(Line3,(uint8_t *)text);
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)  //��д�жϻص�����
{
	if(htim->Instance == TIM17)   //�Ƚ��ж�ʱ����Դ�ж�
	{
		Input_capture = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);   // ����ͨ��ֵ
		TIM17->CNT = 0;
		fre = (8000000) / (80 * Input_capture);
		
	}
	
}






