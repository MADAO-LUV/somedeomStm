#include "fun.h"
uint16_t status1,last_status1;
uint16_t status2,last_status2;
uint16_t status3,last_status3;
uint16_t status4,last_status4;
// LED ��0 ��ʼ��


/*
����:Ӳ��ƽ̨

Ƕ��ʽ����ʵѵƽ̨

���Ҫ��

ͨ����������LD״̬������B1���£�LD1ָʾ��״̬��ת������B2���£�LD2ָʾ��״̬��ת������B3���£�LD3ָʾ�Ʒ�ת������B4����ʱ��Ϩ������ָʾ�ơ�

��ע

�ϵ�ָʾ�ƾ�����Ϩ��״̬

*/


void LED_Show(uint8_t led)
{
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_8 << (led - 1));
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);	
}


void key_scan()
{
	status1 = HAL_GPIO_ReadPin(B1_GPIO_Port,B1_Pin);
	status2 = HAL_GPIO_ReadPin(B2_GPIO_Port,B2_Pin);
	status3 = HAL_GPIO_ReadPin(B3_GPIO_Port,B3_Pin);
	status4 = HAL_GPIO_ReadPin(B4_GPIO_Port,B4_Pin);
	
	if(status1 == 0 && last_status1 == 1)
	{
		LED_Show(1);
	}
	
	if(status2 == 0 && last_status2 == 1)
	{
		LED_Show(2);
	}
	
	if(status3 == 0 && last_status3 == 1)
	{
		LED_Show(3);
	}
	
	if(status4 == 0 && last_status4 == 1)
	{
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);	
	}
	
	last_status1 = status1;
	last_status2 = status2;
	last_status3 = status3;
	last_status4 = status4;
}