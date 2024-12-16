#include "headfile.h"

uint8_t B1_state;
uint8_t B2_state;
uint8_t B3_state;
uint8_t B4_state;    //状态记录
uint8_t B1_last_state = 1;
uint8_t B2_last_state = 1;
uint8_t B3_last_state = 1;
uint8_t B4_last_state = 1;   //上一次的状态
char text[20];  //字符缓冲
int count;
uint16_t led_mode;
uint16_t lcd_highshow;  // 高亮显示 原理就是修改对应行的 背景色

void LED_Show(uint8_t led,uint8_t mode)
{
   
   HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET); 
	
	if(mode)
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8 << (led - 1),GPIO_PIN_RESET);//左移8位到PC8-PC15
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
	
	
	
	if(B1_state == 0 && B1_last_state == 1)   //如果按键按下
	{
		TIM3->CNT = 0;   //直接对寄存器进行操作
	}
	else if(B1_state == 0 && B1_last_state == 0)    //按键长按
	{
		if(TIM3->CNT >= 10000)
		{
			count++;
		}
	}
	else if(B1_state == 1 && B1_last_state == 0) //按键b1松开
	{
		if(TIM3->CNT < 10000)
		{
			count += 2;
		}
	}
	
	
	
	
	
	
	if(B2_state == 0 && B2_last_state == 1)   //如果按键按下
	{
		LED_Show(4,1);
	}
	
	
	
	if(B3_state == 0 && B3_last_state == 1)   //如果按键按下
	{
		LED_Show(8,1);
	}
	
	if(B4_state == 0 && B4_last_state == 1)   //如果按键按下
	{
		lcd_highshow++;
		lcd_highshow %= 3; //限定到3行内
	}
	
	
	B1_last_state = B1_state;
	B2_last_state = B2_state;
	B3_last_state = B3_state;
	B4_last_state = B4_state;
}



void lcd_show()
{
	if(lcd_highshow == 0)
	{
		sprintf(text,"      text      ");
		LCD_SetBackColor(Yellow);
		LCD_DisplayStringLine(Line0,(uint8_t *)text);
		sprintf(text,"     count:   %d  ",count);
		LCD_SetBackColor(Black);
		LCD_DisplayStringLine(Line3,(uint8_t *)text);
		sprintf(text,"   I  love    ");
		LCD_DisplayStringLine(Line4,(uint8_t *)text);
		sprintf(text,"   bibi   ");
		LCD_DisplayStringLine(Line5,(uint8_t *)text);
	}
	
	else if(lcd_highshow == 1)
	{
		sprintf(text,"      text      ");
		LCD_DisplayStringLine(Line0,(uint8_t *)text);
		sprintf(text,"     count:   %d  ",count);
		LCD_SetBackColor(Yellow);
		LCD_DisplayStringLine(Line3,(uint8_t *)text);
		sprintf(text,"   I  love    ");
		LCD_SetBackColor(Black);
		LCD_DisplayStringLine(Line4,(uint8_t *)text);
		sprintf(text,"   bibi   ");
		LCD_DisplayStringLine(Line5,(uint8_t *)text);
	}
	else if(lcd_highshow == 2)
	{
		sprintf(text,"      text      ");
		LCD_DisplayStringLine(Line0,(uint8_t *)text);
		sprintf(text,"     count:   %d  ",count);
		LCD_DisplayStringLine(Line3,(uint8_t *)text);
		sprintf(text,"   I  love    ");
		LCD_DisplayStringLine(Line4,(uint8_t *)text);
		LCD_SetBackColor(Yellow);
		sprintf(text,"   bibi   ");
		LCD_DisplayStringLine(Line5,(uint8_t *)text);
		LCD_SetBackColor(Black);
	}
	LED_Show(1,led_mode);
}




void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM2)   //判断中断资源
  {
	led_mode++;
	 led_mode = led_mode % 2;

  }
}








