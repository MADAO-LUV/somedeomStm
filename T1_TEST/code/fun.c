#include "fun.h"
uint16_t status1,last_status1;
uint16_t status2,last_status2;
uint16_t status3,last_status3;
uint16_t status4,last_status4;
// LED 从0 开始数


/*
任务:硬件平台

嵌入式竞赛实训平台

设计要求

通过按键控制LD状态，按键B1按下，LD1指示灯状态翻转，按键B2按下，LD2指示灯状态翻转，按键B3按下，LD3指示灯翻转，按键B4按下时，熄灭所有指示灯。

备注

上电指示灯均处于熄灭状态

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