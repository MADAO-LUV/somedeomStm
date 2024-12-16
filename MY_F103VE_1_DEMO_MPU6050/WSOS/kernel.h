


#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "stm32f1xx_hal.h"


#define  TaskLen 20        //  ����������



#define  WS_OK  0     
#define  WS_ERR 1


typedef enum 
{ 
  WS_Status_OK       = 0x00U,
  WS_Status_ERROR    = 0x01U,
  WS_Status_BUSY     = 0x02U,
	WS_Status_EMPTY    = 0x03U,
  WS_Status_TIMEOUT  = 0x04U
} WS_StatusTypeDef;



//       �رա���  CPU�ж�
#define  EN_INTERRUPT  __enable_irq();
#define  DI_INTERRUPT  __disable_irq();

#define  WS_OS_EN_INTERRUPT EN_INTERRUPT
#define  WS_OS_DI_INTERRUPT DI_INTERRUPT

//       ������ƿ�
struct wstask ;
typedef struct wstask  Task;
typedef Task   *WspTsak;
struct wstask 
{
	 short  int   ID;
	 int          Time   ;
	 int         (*fun)(void *) ;
	 void         *parameter    ;
	 WspTsak  next;
	 	
#if  WS_OS_MSG_EN > 0
    Message msg;
#endif	
};




void AddRunTask(WspTsak ptk)  ; //  ��Ӵ�ִ�е�����
void AddOnTask(WspTsak ptk)   ;      //  ������񵽶�ʱ��
void AddHangTask(WspTsak ptk) ;    //  ������񵽹����б�   


extern void InitTask(void) ;   //  ��ʼ�����������
extern void OnTask(void)   ;  //  ������ȹ�����
extern void RunTask(void)  ;  //  ����ִ����



//   ����������,��ĳ��������Ϊ���񣬿�����ɶ�ʱ���õȹ���
//   ������ID������ID�������ظ�����ཫ����ʧ��
//         fun: ��������
//         time: ��������ʱtime * 10 ms ��ִ�У������0������ʱ��ִ�У���StopTask����
extern char CreateTask(char id,int (*fun)(void *),int time);  //  ��������

//   ������ʱ����,��ʱtime * 10 ms ��ִ�иú���
//         fun: ��������
//         time: ��������ʱtime * 10 ms ��ִ��
extern char CreateTimer(int (*fun)(void *),int time);         //  ������ʱ������

//   ��ĳ������ʼִ��
//         id: ����id
//         time: ��������ʱtime * 10 ms ��ִ�У����Ϊ0������ִ��
extern char StartTask(char id,int time);  //  ������������
//   ֹͣһ������
extern char StopTask(char id)   ;         //  ��ִ���������
extern char DeleteTask(char id) ;         //  ɾ������


//   WSOS������
#define WS_OS_Init_Task  InitTask
#define WS_OS_On_Task    OnTask
#define WS_OS_Run_Task   RunTask

#define WS_OS_Create_Task   CreateTask
#define WS_OS_Create_Timer  CreateTimer


#define WS_OS_Start_Task   StartTask
#define WS_OS_Stop_Task    StopTask
#define WS_OS_Delete_Task  DeleteTask

//   д���������
char WS_OS_Write_Task_Parameter(short int id , void *parameter, int delayTime );




//char setTaskMsg(char taskid,Message msg);
#if WS_MSG_EN > 0



#endif


#endif
