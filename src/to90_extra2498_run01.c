#pragma force_active on
#include "global.h"
#pragma force_active on
void BeginNormalPaletteFade(int,int,int,int,int,int,u32);BOOL IsPaletteFadeFinished(void);
#define P(p,n) (*(void**)((u8*)(p)+(n)))
#define W(p,n) (*(u32*)((u8*)(p)+(n)))
#define H(p,n) (*(u16*)((u8*)(p)+(n)))
#define SH(p,n) (*(s16*)((u8*)(p)+(n)))
#define B(p,n) (*(u8*)((u8*)(p)+(n)))
#define SB(p,n) (*(s8*)((u8*)(p)+(n)))
void WifiConnection_StartVoiceChat(u32);void WifiConnection_StopVoiceChat(void);void sub_0203A948(int,u32);void WirelessStatusIcon_Destroy(void);void Comm_SendCommand(int,int,int);
void ov90_0225927C(void*,void*);void ov90_02259464(void*,void*,int,int);int ov90_02259538(void*,int);void ov90_02259554(void*,int);
void YesNoPrompt_InitFromTemplate(void*,void*);int YesNoPrompt_HandleInput(void*);void YesNoPrompt_Reset(void*);

typedef struct {u8 state,flag;u16 timer;void*prompt;u8 template[22];u8 ready,cancel;} Work;
int ov90_0225B38C(Work*,void*,void*,void*,u32);
int ov90_0225B38C(Work*s,void*a,void*b,void*c,volatile u32 heap){switch(s->state){case 0:{u32 localheap=heap;BeginNormalPaletteFade(0,0,1,0,6,1,localheap);sub_0203A948(0,heap);if(s->flag)WifiConnection_StartVoiceChat(localheap);s->state++;}break;case 1:if(IsPaletteFadeFinished())s->state++;break;case 2:ov90_0225927C(b,c);ov90_02259464(a,b,3,1);s->state++;break;case 3:if(ov90_02259538(a,1)==1)s->state++;break;case 4:YesNoPrompt_InitFromTemplate(s->prompt,s->template);s->state++;break;case 5:{int result=YesNoPrompt_HandleInput(s->prompt);if((u32)result-1u<=1u){if(result==1)Comm_SendCommand(22,0,0);else if(result==2)Comm_SendCommand(23,0,0);ov90_02259464(a,b,15,1);ov90_02259554(a,1);YesNoPrompt_Reset(s->prompt);s->state++;}}break;case 6:if(s->ready){if(s->cancel)s->state=10;else s->state++;}break;case 7:ov90_02259464(a,b,6,1);s->state++;break;case 8:if(ov90_02259538(a,1)==1){s->state++;s->timer=102;}break;case 9:s->timer--;if(s->timer==0)s->state++;break;case 10:BeginNormalPaletteFade(4,0,0,0,6,1,heap);s->state++;break;case 11:if(IsPaletteFadeFinished()){WirelessStatusIcon_Destroy();if(s->flag)WifiConnection_StopVoiceChat();s->state++;}break;case 12:return 1;}return 0;}
