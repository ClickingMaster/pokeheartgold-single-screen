#pragma force_active on
#include "global.h"
#include "system.h"
#pragma force_active on
void PlaySE(u16);
#define P(p,n) (*(void**)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define SH(p,n) (*(s16*)((u8*)(p)+(n)))
#define B(p,n) (*(u8*)((u8*)(p)+(n)))
#define SB(p,n) (*(s8*)((u8*)(p)+(n)))
u16 TMHMGetMove(u16);int MoveIsHM(u16);void BufferMoveName(void*,int,u16);void *NewString_ReadMsgData(void*,int);void StringExpandPlaceholders(void*,void*,void*);void String_Delete(void*);void ReadMsgDataIntoString(void*,int,void*);u8 ov15_021FEF48(void*,int);int TextPrinterCheckActive(u8);void FillWindowPixelBuffer(void*,u8);void ov15_021FF004(void*);int YesNoPrompt_HandleInput(void*);void ov15_021FF058(void*);void sub_020880CC(int,int);void ov15_021FED3C(void*);void ClearFrameAndWindow2(void*,int);void ClearWindowTilemapAndScheduleTransfer(void*);void ScheduleWindowCopyToVram(void*);void*ov15_021FA074(void*);void ov15_02200140(void*,void*,void*,int);void ov15_021FE868(void*);void ov15_02200294(void*);void ov15_021FB518(void*);void ov15_021FD788(void*,int);int ov15_021FB830(void*);
int ov15_021FB830(void*s){switch(B(s,0x67b)){case 0:{u16 move=TMHMGetMove(H(P(s,0x234),0x66));BufferMoveName(P(s,0x2f4),0,move);if(MoveIsHM(move)==1)ReadMsgDataIntoString(P(s,0x2f0),60,P(s,0x5e4));else ReadMsgDataIntoString(P(s,0x2f0),59,P(s,0x5e4));B(s,0x616)=ov15_021FEF48(s,0);B(s,0x67b)=1;}break;case 1:if(!TextPrinterCheckActive(B(s,0x616))){if((gSystem.newKeys&3)||gSystem.touchNew){void*t=NewString_ReadMsgData(P(s,0x2f0),61);FillWindowPixelBuffer((u8*)s+0x34,15);StringExpandPlaceholders(P(s,0x2f4),P(s,0x5e4),t);String_Delete(t);B(s,0x616)=ov15_021FEF48(s,0);B(s,0x67b)=2;}}break;case 2:if(!TextPrinterCheckActive(B(s,0x616))){ov15_021FF004(s);B(s,0x67b)=3;}break;case 3:{int answer=YesNoPrompt_HandleInput(P(s,0x804));switch(answer){case 1:{ov15_021FF058(s);sub_020880CC(1,6);H(P(s,0x234),0x68)=0;return 37;}case 2:{void*party;void*arg;ov15_021FF058(s);ov15_021FED3C(s);ClearFrameAndWindow2((u8*)s+0x34,1);ClearWindowTilemapAndScheduleTransfer((u8*)s+0x34);ScheduleWindowCopyToVram((u8*)s+4);party=P(s,0x234);arg=ov15_021FA074(s);ov15_02200140(s,(u8*)party+4+12*B(party,0x64),arg,0);ov15_021FE868(s);ov15_02200294(s);ov15_021FB518(s);ov15_021FD788(s,1);return 1;}default:break;}}break;}return 13;}
