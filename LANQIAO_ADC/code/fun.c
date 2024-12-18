#include "headfile.h"
char text[20];
double Get_volte(ADC_HandleTypeDef *hadc);


void lcd_show()
{
	sprintf(text,"      text      ");
	LCD_DisplayStringLine(Line0,(uint8_t *)text);
	sprintf(text,"   fre1:   %.2f   ",Get_volte(&hadc1));
	LCD_DisplayStringLine(Line3,(uint8_t *)text);
	sprintf(text,"   fre2:   %.2f   ",Get_volte(&hadc2));
	LCD_DisplayStringLine(Line5,(uint8_t *)text);
}

/*
��ѹֵ : 0 ~ 3.3v
ģ��ֵ��ΧΪ : 0 ~ 4096
ģ��ֵ ת��Ϊ ʵ�ʵ�ѹֵ ��ʽ:

 3.3v * (mon_volte / 4096)
*/
double Get_volte(ADC_HandleTypeDef *hadc)
{
	uint32_t temp;
	HAL_ADC_Start(hadc);    //���� adcͨ����ȡ adc ģ��ֵ
	temp = HAL_ADC_GetValue(hadc);
	return 3.3 * temp / 4096;
}










