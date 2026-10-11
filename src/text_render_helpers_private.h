#ifndef PRIVATE_H
#define PRIVATE_H
#include "global.h"
extern void ScreenWindow_QueueSetVisible(u32,int); extern void ScreenWindow_QueueSetInside(u32,int,int,int,int); extern void ScreenWindow_QueueSetRect(u32,int,int,int,int,int,int); extern void ScreenWindow_QueueSetOutside(u32,int,int,int);
extern void ScreenWindow_SetInside(u32,int,int,int); extern void ScreenWindow_SetOutside(u32,int,int); extern void ScreenWindow_SetRect(u32,u32,u32,u32,int,int);
void sub_02010F34(int,u32,int); void sub_02010F84(u32,u32,u32,u32,int,int,int,int,int,int);
#endif
extern int ScreenWindow_GetInside(int,int); extern int ScreenWindow_GetOutside(int); void sub_02010FEC(u32,int,int,int);
