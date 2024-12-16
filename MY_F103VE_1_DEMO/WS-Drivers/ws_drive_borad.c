
#include "ws_drive_borad.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

extern UART_HandleTypeDef huart1; //对串口1进行外部声明


// 格式化可变变量字符串缓存
char formatBuf[128];
// 格式化输出
// 利用系统提供的功能函数实现
void WS_Debug(char *p,...)
{
	/*   格式化数据  */
	va_list ap;
	va_start(ap,p);
	vsprintf(formatBuf,p,ap);
	va_end(ap);
	

// 编译控制
#ifdef DEBUG_EN
	//  信息输出接口
	HAL_UART_Transmit_IT(&huart1,(uint8_t *)formatBuf,strlen(formatBuf));
// HAL_UART_Transmi(&huart1,(uint8_t *)formatBuf,strlen(formatBuf),1);
// 对于没有使用中断的串口发送，是需要加入一个阻塞的 
#endif
}


/*
读取 方法:
// 使用(uint8_t) 就是取GPIOX->IDR的低8位 
//  0xfffe ---> 0x1111 1111 1111 1101 为1 的地方是屏蔽 为 0的地方保留下来读取
	 key = GetKeyDFallingEdge((uint8_t)(GPIOA->IDR | 0xfffe)); // sw1
*/

//   获取按键 下降沿函数，调用无要求： 1~10 间隔使用
char GetKeyDFallingEdge(unsigned char io)    // char : 0-255
{
	char key = 0;   //局部变量
	static char countFlag = 0;  //静态变量
	if(io != 0xff)  //说明有按键按下
	{
		if(countFlag < 3)   countFlag++; // 1,2,3
		
		if(countFlag == 2)  //连续进入两次 实现去抖
		{
			switch(io)
			{
				case 0xfe: key=1;break;  // 1111 1110,sw1 
				case 0xfd: key=2;break;  // 1111 1101 ,sw2 
				case 0xfb: key=3;break;  // 1111 1011 ,sw3 
				case 0xf7: key=4;break;  // 1111 0111, sw4 
				case 0xef: key=5;break;  // 1110 1111, sw5
			}
		}
		
	}
	else
	{
		countFlag = 0;
	}
	return key;  //返回键值
}





// 注意: 回调函数是再中断程序内执行，不能在这个函数里面执行耗时操作
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t tick = 0;	 //必须是静态变量
	
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
 //本人的板子就只有两个按键 所以只设置两个sw
	switch(GPIO_Pin)
	{
		case sw1_Pin:
					if(HAL_GPIO_ReadPin(sw1_GPIO_Port,sw1_Pin) == GPIO_PIN_RESET)
					{
						tick = HAL_GetTick();  //获取初始值
						//WS_Debug("SW1 DOWN\r\n");
					}
					else
					{
						WS_Debug("SW1 Down time %d ms\r\n",HAL_GetTick() - tick);
						//WS_Debug("SW1 UP\r\n");
					}
					break;
		
		case sw2_Pin:
					if(HAL_GPIO_ReadPin(sw2_GPIO_Port,sw2_Pin) == GPIO_PIN_RESET)
					{	
						tick = HAL_GetTick();  //获取初始值
						//WS_Debug("SW2 DOWN\r\n");
					}
					else
					{
						WS_Debug("SW2 Down time %d ms\r\n",HAL_GetTick() - tick);
					}
					break;
					
	//  case sw3_pin:
	}
	
}


