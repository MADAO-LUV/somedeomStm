
#include "ws_drive_borad.h"
#include "ws_drive_dwt.h"
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
	HAL_UART_Transmit_DMA(&huart1,(uint8_t *)formatBuf,strlen(formatBuf));
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




// 计算计数差值  只是用来计算  不能超过最大测量时间 59秒
uint32_t getDWTCountDx(uint32_t startTick, uint32_t stopTick)
{
	uint32_t tick;
	if(stopTick < startTick)
	{
		tick = (0xffffffff - startTick) + stopTick + 1;
	}
	else
	{
		tick = stopTick - startTick;
	}
	
	return tick;
}



//注册一个任务  任务函数A
int WS_OS_Task_KeyBorad(void *date)
{
	static int key = 0;
	key = GetKeyDFallingEdge(GPIOA->IDR | 0xE0);  //这里是
	if (key > 0)
	{
		WS_Debug("key %d\r\n",key);
		WS_OS_DEBUG_LED_OPEN(1000);
	}
	
	return 10; // 10ms
	
	
}

//根据 WSOS中的WS_OS_Create_Timer 创建一个定时器，来调控灯关的关闭时间
int WS_OS_DEBUG_LED_CLOSED(void *date)
{
	HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port,LED_DEBUG_Pin,GPIO_PIN_SET);
	
	return 0; //不进行延时
}


void WS_OS_DEBUG_LED_OPEN(int time)
{
	HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port,LED_DEBUG_Pin,GPIO_PIN_RESET);
	WS_OS_Create_Timer(WS_OS_DEBUG_LED_CLOSED,time);
	
}



// 任务函数 B
int WS_OS_Task_B(void *date)
{
	static int num = 0;
	WS_Debug("Task B output: %d\r\n",num ++);
	return 500;   //返回值 决定下一次的什么时候执行
	
}

/*
回调函数 里面使用的是 DWT外设 只有Cortex-3 以及以上才有这个外设
优点: 测量十分精确
缺点: 不能产生中断

这里的 第一次测量是错误值，但是之后的测量是正确的。
上一次的结束值 为 下一次的起始值
*/

// 注意: 回调函数是再中断程序内执行，不能在这个函数里面执行耗时操作


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t startTick = 0,stopTick,tick;	 //必须是静态变量
	// 一定是静态变量 来记录初值
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
 //本人的板子就只有两个按键 所以只设置两个sw

	
}

