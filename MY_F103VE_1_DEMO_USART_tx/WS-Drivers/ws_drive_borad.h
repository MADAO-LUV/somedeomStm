#ifndef WS_DRIVE_BORAD
#define WS_DRIVE_BORAD

#include "main.h" //ʹ��ϵͳ��Ķ���

#define DEBUG_EN 1

void WS_Debug(char *p,...);  // printf

//ע��һ������  ������A
int WS_OS_Task_KeyBorad(void *date);
int WS_OS_Task_B(void *date);

char GetKeyDFallingEdge(unsigned char io);    // char : 0-255
//���� WSOS�е�WS_OS_Create_Timer ����һ����ʱ���������صƹصĹر�ʱ��
int WS_OS_DEBUG_LED_CLOSED(void *date);


void WS_OS_DEBUG_LED_OPEN(int time);


#endif