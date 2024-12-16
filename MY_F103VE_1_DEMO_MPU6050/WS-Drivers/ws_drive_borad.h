#ifndef WS_DRIVE_BORAD
#define WS_DRIVE_BORAD

#include "main.h" //使用系统里的东西

#define DEBUG_EN 1

void WS_Debug(char *p,...);  // printf

//注册一个任务  任务函数A
int WS_OS_Task_KeyBorad(void *date);
int WS_OS_Task_B(void *date);

char GetKeyDFallingEdge(unsigned char io);    // char : 0-255
//根据 WSOS中的WS_OS_Create_Timer 创建一个定时器，来调控灯关的关闭时间
int WS_OS_DEBUG_LED_CLOSED(void *date);


void WS_OS_DEBUG_LED_OPEN(int time);


#endif