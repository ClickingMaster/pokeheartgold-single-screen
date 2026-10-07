#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
void Main_SetVBlankIntrCB(void(*)(void*),void*);void HBlankInterruptDisable(void);void GfGfx_DisableEngineAPlanes(void);void GfGfx_DisableEngineBPlanes(void);void Heap_Create(int,int,u32);void *OverlayManager_CreateAndGetData(void*,u32,int);void*OverlayManager_GetArgs(void*);void*BgConfig_Alloc(int);void*String_New(u32,int);void*NewMsgDataFromNarc(int,int,int,int);
int IsPaletteFadeFinished(void);void ov71_022473E4(void);int Sound_SetSceneAndPlayBGM(u8,u16,int);void*MessageFormat_New(int);void BufferBoxMonNickname(void*,int,void*);void BufferPlayersName(void*,int,void*);int ov71_02246B28(void*);void OamManager_Create(int,int,int,int,int,int,int,int,int);void*G2dRenderer_Init(int,void*,int);void G2dRenderer_SetSubSurfaceCoords(void*,s32,s32);u32 GetBoxMonData(void*,int,void*);void*SysTask_CreateOnPrintQueue(void(*)(void*,void*),void*,int);void ov71_02246C48(void*,void*);
int TradeSequence_Init(void*manager,void*state);
int TradeSequence_Init(void*manager,void*state){u8*w;
 if(IsPaletteFadeFinished()){
 Heap_Create(3,56,3<<15);Heap_Create(3,57,3<<15);ov71_022473E4();Sound_SetSceneAndPlayBGM(3,0x3f4,1);w=OverlayManager_CreateAndGetData(manager,0x158,56);
 if(w){P(w,0)=OverlayManager_GetArgs(manager);U(w,4)=0;P(w,8)=BgConfig_Alloc(56);P(w,0x14)=String_New(400,56);P(w,0x10)=NewMsgDataFromNarc(0,27,179,56);P(w,0xc)=MessageFormat_New(56);
 switch(U(P(w,0),0x10)){
 case 1:BufferBoxMonNickname(P(w,0xc),0,P(P(w,0),0));BufferBoxMonNickname(P(w,0xc),1,P(P(w,0),4));BufferPlayersName(P(w,0xc),2,P(P(w,0),8));U(w,0x148)=ov71_02246B28(P(P(w,0),4));break;
 case 2:BufferBoxMonNickname(P(w,0xc),0,P(P(w,0),0));break;
 case 4:BufferBoxMonNickname(P(w,0xc),1,P(P(w,0),4));U(w,0x148)=ov71_02246B28(P(P(w,0),4));break;
 }
 NNS_G2dInitOamManagerModule();OamManager_Create(0,128,0,32,1,127,0,32,56);P(w,0x18)=G2dRenderer_Init(64,w+0x1c,56);G2dRenderer_SetSubSurfaceCoords(w+0x1c,0,0xe8000);
 H(w,0x14c)=GetBoxMonData(P(P(w,0),0),5,NULL);H(w,0x150)=GetBoxMonData(P(P(w,0),4),5,NULL);H(w,0x14e)=GetBoxMonData(P(P(w,0),0),0x70,NULL);H(w,0x152)=GetBoxMonData(P(P(w,0),4),0x70,NULL);U(w,0x154)=0;
 Main_SetVBlankIntrCB(NULL,NULL);HBlankInterruptDisable();GfGfx_DisableEngineAPlanes();GfGfx_DisableEngineBPlanes();*(volatile u32*)0x04000000&=0xffffe0ff;*(volatile u32*)0x04001000&=0xffffe0ff;P(w,0x144)=SysTask_CreateOnPrintQueue(ov71_02246C48,w,1);U(w,0x154)=0;
 }return 1;}else return 0;
}
