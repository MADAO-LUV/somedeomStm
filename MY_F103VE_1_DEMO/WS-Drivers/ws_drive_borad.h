#ifndef WS_DRIVE_BORAD
#define WS_DRIVE_BORAD

#include "main.h" //使用系统里的东西

#define DEBUG_EN 1

void WS_Debug(char *p,...);  // printf

char GetKeyDFallingEdge(unsigned char io);    // char : 0-255
#endif