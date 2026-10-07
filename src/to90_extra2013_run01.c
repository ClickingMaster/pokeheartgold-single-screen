#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
extern void ov31_0225E4EC(int,void*,int,int),ov31_0225E4BC(int,void*,int,int),BufferPocketName(void*,int,int),StringExpandPlaceholders(void*,void*,void*),String_Delete(void*),FillWindowPixelBuffer(void*,int),DrawFrameAndWindow2(void*,int,int,int),ov31_0225E948(void*);extern void*NewString_ReadMsgData(void*,int);extern int GetItemAttr(int,int,int),Options_GetTextFrameDelay(void*),AddTextPrinterParameterized(void*,int,void*,int,int,int,void*);void ov31_0225E7D4(void*);
void ov31_0225E7D4(void*w){void*s;void*d=P(w,20);if(*(s16*)((u8*)d+0x286)>1)ov31_0225E4EC(B(d,0x283),P(w,0x154),H(d,0x284),0);else ov31_0225E4BC(B(d,0x283),P(w,0x154),H(d,0x284),0);switch(B(P(w,20),0x283)){case 0:s=NewString_ReadMsgData(P(w,0x158),15);BufferPocketName(P(w,0x154),1,GetItemAttr(H(P(w,20),0x284),5,11));break;case 3:if(H(P(w,20),0x284)>=485&&H(P(w,20),0x284)<=491)s=NewString_ReadMsgData(P(w,0x158),49);else{s=NewString_ReadMsgData(P(w,0x158),15);BufferPocketName(P(w,0x154),1,GetItemAttr(H(P(w,20),0x284),5,11));}break;case 4:s=NewString_ReadMsgData(P(w,0x158),49);break;case 1:s=NewString_ReadMsgData(P(w,0x158),21);break;default:s=NewString_ReadMsgData(P(w,0x158),23);break;}StringExpandPlaceholders(P(w,0x154),P(w,0x188),s);String_Delete(s);FillWindowPixelBuffer((u8*)w+0x44,15);DrawFrameAndWindow2((u8*)w+0x44,1,437,5);B(P(w,20),0x280)=AddTextPrinterParameterized((u8*)w+0x44,1,P(w,0x188),0,0,Options_GetTextFrameDelay(P(w,0x164)),ov31_0225E948);}
