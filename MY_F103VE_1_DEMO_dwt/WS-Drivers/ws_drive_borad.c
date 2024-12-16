
#include "ws_drive_borad.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

extern UART_HandleTypeDef huart1; //�Դ���1�����ⲿ����


// ��ʽ���ɱ�����ַ�������
char formatBuf[128];
// ��ʽ�����
// ����ϵͳ�ṩ�Ĺ��ܺ���ʵ��
void WS_Debug(char *p,...)
{
	/*   ��ʽ������  */
	va_list ap;
	va_start(ap,p);
	vsprintf(formatBuf,p,ap);
	va_end(ap);
	

// �������
#ifdef DEBUG_EN
	//  ��Ϣ����ӿ�
	HAL_UART_Transmit_IT(&huart1,(uint8_t *)formatBuf,strlen(formatBuf));
// HAL_UART_Transmi(&huart1,(uint8_t *)formatBuf,strlen(formatBuf),1);
// ����û��ʹ���жϵĴ��ڷ��ͣ�����Ҫ����һ�������� 
#endif
}


/*
��ȡ ����:
// ʹ��(uint8_t) ����ȡGPIOX->IDR�ĵ�8λ 
//  0xfffe ---> 0x1111 1111 1111 1101 Ϊ1 �ĵط������� Ϊ 0�ĵط�����������ȡ
	 key = GetKeyDFallingEdge((uint8_t)(GPIOA->IDR | 0xfffe)); // sw1
*/

//   ��ȡ���� �½��غ�����������Ҫ�� 1~10 ���ʹ��
char GetKeyDFallingEdge(unsigned char io)    // char : 0-255
{
	char key = 0;   //�ֲ�����
	static char countFlag = 0;  //��̬����
	if(io != 0xff)  //˵���а�������
	{
		if(countFlag < 3)   countFlag++; // 1,2,3
		
		if(countFlag == 2)  //������������ ʵ��ȥ��
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
	return key;  //���ؼ�ֵ
}




// ���������ֵ  ֻ����������  ���ܳ���������ʱ�� 59��
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




/*
�ص����� ����ʹ�õ��� DWT���� ֻ��Cortex-3 �Լ����ϲ����������
�ŵ�: ����ʮ�־�ȷ
ȱ��: ���ܲ����ж�

����� ��һ�β����Ǵ���ֵ������֮��Ĳ�������ȷ�ġ�
��һ�εĽ���ֵ Ϊ ��һ�ε���ʼֵ
*/

// ע��: �ص����������жϳ�����ִ�У������������������ִ�к�ʱ����
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t startTick = 0,stopTick,tick;	 //�����Ǿ�̬����
	// һ���Ǿ�̬���� ����¼��ֵ
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
 //���˵İ��Ӿ�ֻ���������� ����ֻ��������sw
	switch(GPIO_Pin)
	{
		case sw1_Pin:
					if(HAL_GPIO_ReadPin(sw1_GPIO_Port,sw1_Pin) == GPIO_PIN_RESET)
					{
						stopTick = DWT->CYCCNT;
						
						tick = getDWTCountDx(startTick,stopTick);
						if(tick > 720000) // 10ms
						  WS_Debug("SW1 down time is %.2fms\r\n",(float)tick/72000);
						
						startTick = stopTick;
					}
					break;
		
		case sw2_Pin:
					if(HAL_GPIO_ReadPin(sw2_GPIO_Port,sw2_Pin) == GPIO_PIN_RESET)
					{
						stopTick = DWT->CYCCNT;
						
						tick = getDWTCountDx(startTick,stopTick);
						if(tick > 72000) // 10ms
						  WS_Debug("SW2 down time is %.2fms\r\n",(float)tick/72000);
						
						startTick = stopTick;
					}	
					break;
					
	//  case sw3_pin:
	}
	
}


