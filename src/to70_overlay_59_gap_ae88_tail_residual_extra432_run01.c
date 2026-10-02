#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x5c];void *messages;u8 pad2[8];void *str;u8 pad3[0x1c4-0x6c];u8 window[16];u8 pad4[0x260-0x1d4];void *sprites[2];} State;
extern u32 ov59_0223C954[];
extern void ClearWindowTilemapAndScheduleTransfer(void *),Sprite_SetDrawFlag(void *,int),FillWindowPixelBuffer(void *,int),ReadMsgDataIntoString(void *,int,void *),AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,u32,void *),ScheduleWindowCopyToVram(void *),Sprite_SetAnimCtrlSeq(void *,int),Sprite_ResetAnimCtrlState(void *),ov59_0223B358(State *,int);
extern int ov59_0223B3B0(State *,int *);
void ov59_0223AEB0(State *,u32),ov59_0223AF7C(State *);
void ov59_0223AEB0(State *s,u32 state){int i;switch(state){case 3:ClearWindowTilemapAndScheduleTransfer(s->window);Sprite_SetDrawFlag(s->sprites[0],0);Sprite_SetDrawFlag(s->sprites[1],0);break;case 2:FillWindowPixelBuffer(s->window,0);for(i=0;i<2;i++){ReadMsgDataIntoString(s->messages,i+1,s->str);AddTextPrinterParameterizedWithColor(s->window,4,s->str,0,4+i*24,255,ov59_0223C954[i],0);Sprite_SetDrawFlag(s->sprites[i],1);}ScheduleWindowCopyToVram(s->window);break;case 0:case 1:Sprite_SetAnimCtrlSeq(s->sprites[state],state*2+1);Sprite_ResetAnimCtrlState(s->sprites[state]);break;}}
void ov59_0223AF7C(State *s){int flag=0;ov59_0223B3B0(s,&flag);if(flag==0)ov59_0223B358(s,flag);}
