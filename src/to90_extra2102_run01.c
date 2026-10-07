#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,n) (*(void**)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define SW(p,n) (*(s32*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define B(p,n) (*(u8*)((u8*)(p)+(n)))
void PlaySE(u16);
#include <string.h>
extern u8 _021D4124[],_0210F8FC[];void sub_02032858(int);void sub_02032844(int);int sub_02033264(void);u32 sub_0203993C(void);int WM_Disconnect(void*,u16);int sub_02032A40(void);void sub_02032934(void*);
void sub_02032934(void*event){u16 mask=1<<H(event,16);if(H(event,2)){sub_02032858(H(event,2));sub_02032844(9);return;}switch(H(event,8)){case 2:B(P(_021D4124,4),0x1345)++;break;case 7:if(B(P(_021D4124,4),0x1343)==1||B(P(_021D4124,4),0x1342)==1||sub_02033264()>=B(P(_021D4124,4),0x1334)||B(event,20)!=sub_0203993C()||memcmp(_0210F8FC,(u8*)event+21,3)!=0){int result=WM_Disconnect(0,H(event,16));if(result!=2){sub_02032858(result);sub_02032844(9);}}else{H(P(_021D4124,4),0x132e)|=mask;if(P(P(_021D4124,4),0x1328))((void(*)(u16))P(P(_021D4124,4),0x1328))(H(event,16));}break;case 9:H(P(_021D4124,4),0x132e)&=~mask;if(P(P(_021D4124,4),0x1324))((void(*)(u16))P(P(_021D4124,4),0x1324))(H(event,16));break;case 0:if(!sub_02032A40())sub_02032844(9);break;case 26:break;}}
