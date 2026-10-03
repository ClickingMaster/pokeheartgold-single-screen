#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
#include <string.h>
// clang-format on
typedef struct{int state;u8 p[0x4ac];int socket;} Work;
typedef struct{u8 a,b;} Two;
typedef struct{u8 a,b,c,d;} Four;
extern void ov00_022145F0(Work *,int,int,int,int);extern int ov00_02214558(Work *,void *,int);extern int ov00_021FF970(int,void *,int,int);
int ov00_02215CDC(Work *,u32,u16,void *,int);int ov00_02215E04(Work *,u32,u16,u32);
int ov00_02215CDC(Work *w,u32 address,u16 port,void *data,int length){u8 packet[16];u16 len;int value;if(w->state==1)ov00_022145F0(w,0,0,2,0);if(w->state==1)return 3;value=(u16)(length+9);len=((value>>8)&255)|((value<<8)&0xff00);memcpy(packet,&len,2);packet[2]=2;memcpy(packet+3,&address,4);memcpy(packet+7,&port,2);{int result=ov00_02214558(w,packet,9);if(result)return result;}return ov00_021FF970(w->socket,data,length,0)<0?3:0;}
int ov00_02215E04(Work *w,u32 address,u16 port,u32 payload){u8 packet[10];payload=((payload>>24)&255)|((payload>>8)&0xff00)|((payload<<8)&0xff0000)|((payload<<24)&0xff000000);packet[0]=253;packet[1]=252;packet[2]=30;packet[3]=102;packet[4]=106;packet[5]=178;memcpy(packet+6,&payload,4);return ov00_02215CDC(w,address,port,packet,10);}
