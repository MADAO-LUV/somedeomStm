
#include "ws_drive_dwt.h"

uint32_t  dwtCountBuf[10] = {0};


//   ���������ֵ
uint32_t  getDWTCountDt(uint32_t startTick , uint32_t  stopTick )
{
	   uint32_t tick;
     if(stopTick < startTick)
			{
				 tick = (0xffffffff - startTick) + stopTick  + 1 ;
			}
			else
			{
				 tick = stopTick - startTick;
			}
		 return tick;
}


/******************************************************************************
*�������ƣ� uint32_t WS_DWT_Get_Tick_Dt(uint8_t chx , uint8_t stu)
*���ܣ�     ���õ��õļ������ϵͳʱ�Ӽ�����72MƵ�ʶ�Ӧ1�����72000000������
              ʾ����
              WS_DWT_Get_Tick_Dt(0,0);
              WS_Delay_MS(10);
              time = WS_DWT_Get_Tick_Dt(0,2);
              time �����������ε��õ�ϵͳʱ�Ӽ�����
							����WS_Delay_MS(10)����ִ���������ϵͳʱ�Ӽ��� 
 �βΣ�    mode:    0 �� �趨������㣬����0 ��
                   1 �� �����յ㣬������㵽�յ�ò���ʱ��,���ʱ�䲻�޸�
                   2 �� �������/�ص㣬ÿ�ε��ò����ϴε��õ����ε��õ�ʱ��
           chx:    ����ͨ����0~9���� ֧�ֶ�·ͬʱ����
*����:		  ��
*******************************************************************************/

uint32_t WS_DWT_Get_Tick_Dt(uint8_t chx , uint8_t stu)
{
   uint32_t  tick  = 0 ; 
   static    uint8_t  initFlag = 0;
	
	 //  �״ε��ã� ��ɳ�ʼ��
   if(initFlag == 0)
	 {
	    initFlag = 1;
		  //  DWT ������ʼ��
			CoreDebug->DEMCR |= 1<<24 ;  // ʹ��DWT����
			DWT->CYCCNT       = 0     ;  // ����CYCCNT
			DWT->CTRL        |= 1<<0  ;  // ʹ�ܼ���
	 }	
	 
	 if(chx >= 10)  return 0;  //  ����ͨ������ֱ�ӷ���0
	 
	 switch(stu)
	 {
		 // �趨���
		 case 0:  
							dwtCountBuf[chx]  = DWT->CYCCNT ;  // �洢��ʼֵ
			        break;
		 
		 //  �趨�ص㣬���ز���ʱ��
		 case 1:
			        tick  = getDWTCountDt(dwtCountBuf[chx],DWT->CYCCNT);
		          break;
		 //  ��������
		 case 2:    
			        tick  = getDWTCountDt(dwtCountBuf[chx],DWT->CYCCNT);
		          dwtCountBuf[chx]  = DWT->CYCCNT ;  
			        break;
	 }
	 return tick;
}
