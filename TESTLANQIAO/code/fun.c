#include "headfile.h"

 uint8_t B1_state,B2_state,B3_state,B4_state;    //״̬��¼
 uint8_t B1_last_state,B2_last_state,B3_last_state,B4_last_state;   //��һ�ε�״̬
 char text[20];  //�ַ�����
int count;
void LED_Show(uint8_t led,uint8_t mode)
{
   
   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET); 
	
	if(mode)
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8 << (led - 1),GPIO_PIN_RESET);//����8λ��PC8-PC15
	else
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8 << (led - 1),GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}
 

void key_scan()
{
	B1_state = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
	B2_state = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
	B3_state = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
	B4_state = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
	
	
	
	if(B1_state == 0 && B1_last_state == 1)   //�����������
	{
		LED_Show(1,1);
	}
	
	if(B2_state == 0 && B2_last_state == 1)   //�����������
	{
		LED_Show(4,1);
	}
	
	if(B3_state == 0 && B3_last_state == 1)   //�����������
	{
		LED_Show(8,1);
	}
	
	if(B4_state == 0 && B4_last_state == 1)   //�����������
	{
		LED_Show(2,1);
	}
	
	
	B1_last_state = B1_state;
	B2_last_state = B2_state;
	B3_last_state = B3_state;
	B4_last_state = B4_state;
}



void lcd_show()
{
	sprintf(text,"      text      ");
	LCD_DisplayStringLine(Line0,(uint8_t *)text);
	sprintf(text,"     count:   %d  ",count++);
	LCD_DisplayStringLine(Line3,(uint8_t *)text);
}









