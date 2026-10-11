#ifndef TO91_TEX_CALLBACK_41566_PRIVATE_H
#define TO91_TEX_CALLBACK_41566_PRIVATE_H
#include "global.h"
#include <string.h>
typedef void (*To91TexCalc41566)(void *,void *);
typedef void (*To91TexSend41566)(void *);
extern To91TexSend41566 gSendTexSRTFuncs[];
extern To91TexCalc41566 gTexMtxCalcFuncsXsi[];
void texmtxCalc_flagR___xsi(void *,void *);
void NNSi_G3dSendTexSRTXsi(void *);
#define To91TexWord41566(p,offset) (*(const u32*)((const u8*)(p)+(offset)))
#define To91TexSignedWord41566(p,offset) ((s32)To91TexWord41566(p,offset))
#define To91TexHalf41566(p,offset) (*(const u16*)((const u8*)(p)+(offset)))
#define To91TexSignedHalf41566(p,offset) ((s16)To91TexHalf41566(p,offset))
#define To91TexStoreWord41566(p,offset,value) (*(u32*)((u8*)(p)+(offset))=(u32)(value))
#define To91TexStoreHalf41566(p,offset,value) (*(u16*)((u8*)(p)+(offset))=(u16)(value))
#endif
