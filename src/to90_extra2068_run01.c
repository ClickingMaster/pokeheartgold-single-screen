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
u16 TMHMGetMove(u16);int MoveIsHM(u16);void BufferMoveName(void*,int,u16);void *NewString_ReadMsgData(void*,int);void StringExpandPlaceholders(void*,void*,void*);void String_Delete(void*);void ReadMsgDataIntoString(void*,int,void*);u8 BagApp_PrintMessage(void*,int);int TextPrinterCheckActive(u8);void FillWindowPixelBuffer(void*,u8);void BagApp_CreateYesNoPrompt(void*);int YesNoPrompt_HandleInput(void*);void BagApp_DestroyYesNoPrompt(void*);void sub_020880CC(int,int);void BagApp_CloseItemMenuWindows(void*);void ClearFrameAndWindow2(void*,int);void ClearWindowTilemapAndScheduleTransfer(void*);void ScheduleWindowCopyToVram(void*);void*BagApp_GetPageItemCount(void*);void BagApp_DrawItemIcons(void*,void*,void*,int);void BagApp_ClearMoveInfoWindow(void*);void BagApp_HideItemMenuSprites(void*);void BagApp_RedrawPocket(void*);void BagApp_ShowCancelButton(void*,int);int BagApp_UseTMHMTask(void*);
int BagApp_UseTMHMTask(void*s){switch(B(s,0x67b)){case 0:{u16 move=TMHMGetMove(H(P(s,0x234),0x66));BufferMoveName(P(s,0x2f4),0,move);if(MoveIsHM(move)==1)ReadMsgDataIntoString(P(s,0x2f0),60,P(s,0x5e4));else ReadMsgDataIntoString(P(s,0x2f0),59,P(s,0x5e4));B(s,0x616)=BagApp_PrintMessage(s,0);B(s,0x67b)=1;}break;case 1:if(!TextPrinterCheckActive(B(s,0x616))){if((gSystem.newKeys&3)||gSystem.touchNew){void*t=NewString_ReadMsgData(P(s,0x2f0),61);FillWindowPixelBuffer((u8*)s+0x34,15);StringExpandPlaceholders(P(s,0x2f4),P(s,0x5e4),t);String_Delete(t);B(s,0x616)=BagApp_PrintMessage(s,0);B(s,0x67b)=2;}}break;case 2:if(!TextPrinterCheckActive(B(s,0x616))){BagApp_CreateYesNoPrompt(s);B(s,0x67b)=3;}break;case 3:{int answer=YesNoPrompt_HandleInput(P(s,0x804));switch(answer){case 1:{BagApp_DestroyYesNoPrompt(s);sub_020880CC(1,6);H(P(s,0x234),0x68)=0;return 37;}case 2:{void*party;void*arg;BagApp_DestroyYesNoPrompt(s);BagApp_CloseItemMenuWindows(s);ClearFrameAndWindow2((u8*)s+0x34,1);ClearWindowTilemapAndScheduleTransfer((u8*)s+0x34);ScheduleWindowCopyToVram((u8*)s+4);party=P(s,0x234);arg=BagApp_GetPageItemCount(s);BagApp_DrawItemIcons(s,(u8*)party+4+12*B(party,0x64),arg,0);BagApp_ClearMoveInfoWindow(s);BagApp_HideItemMenuSprites(s);BagApp_RedrawPocket(s);BagApp_ShowCancelButton(s,1);return 1;}default:break;}}break;}return 13;}
