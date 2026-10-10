#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
void*OverlayManager_GetData(void*);void*OverlayManager_GetArgs(void*);int ov93_0225C730(void*);void*OverlayManager_New(void*,void*,int);int OverlayManager_Run(void*);void OverlayManager_Delete(void*);void WifiConnection_StopVoiceChat(void);void sub_020398D4(int,int);void sub_02037AC0(int);int sub_02037B38(int);int sub_02037454(void);int sub_020347A0(void);extern u8 ov93_02262A08[],_022629F8[];
int ov93_0225C574(void*manager,int*state);
int ov93_0225C574(void*manager,int*state){u8*w=OverlayManager_GetData(manager);u8*args=OverlayManager_GetArgs(manager);if(ov93_0225C730(w)==1)return 1;
 switch(*state){
 case 0:B(w,0x31)=0;P(w,0x28)=OverlayManager_New(ov93_02262A08,w,117);(*state)++;break;
 case 1:if(OverlayManager_Run(P(w,0x28))==1){OverlayManager_Delete(P(w,0x28));P(w,0x28)=NULL;(*state)++;}break;
 case 2:B(w,0x31)=1;P(w,0x28)=OverlayManager_New(_022629F8,w,117);(*state)++;break;
 case 3:if(OverlayManager_Run(P(w,0x28))==1){OverlayManager_Delete(P(w,0x28));P(w,0x28)=NULL;(*state)++;}break;
 case 4:if(B(args,0x38))WifiConnection_StopVoiceChat();B(w,0x31)=2;P(w,0x28)=OverlayManager_New(ov93_02262A08,w,117);(*state)++;break;
 case 5:if(OverlayManager_Run(P(w,0x28))==1){OverlayManager_Delete(P(w,0x28));P(w,0x28)=NULL;if(U(w,0x38)==1)*state=0;else (*state)++;}break;
 case 6:sub_020398D4(0,1);sub_02037AC0(222);(*state)++;break;
 case 7:if(sub_02037B38(222)!=1){int n=sub_02037454();if(n>=sub_020347A0())break;}(*state)++;break;
 case 8:
 default:return 1;
 }return 0;
}
