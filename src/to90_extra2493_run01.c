#pragma force_active on
#include "global.h"
#pragma force_active on
#define HH(a) (*(volatile u16*)(a))
#define WW(a) (*(volatile u32*)(a))
void NNS_G3dInit(void);void GF_3DVramMan_InitFrameTexVramManager(int,int),GF_3DVramMan_InitFramePlttVramManager(int,int),GfGfx_EngineATogglePlanes(int,int);
void*TradeSequence_CreateModels(int,int,int,int);void*TradeSequence_LoadModel(void*,int,int,int,int,s32,int,int);void TradeSequence_SetCameraAngle(void*,const void*),TradeSequence_SetModelScale(void*,s32);
extern u16 ov71_0224BD0C[];extern u8 ov71_0224C0C4[],ov71_0224C0C8[];
void ov71_022488E4(u8*);
void ov71_022488E4(u8*w){NNS_G3dInit();G3X_InitMtxStack();HH(0x04000060)&=~0x3002;HH(0x04000060)=(HH(0x04000060)&~0x3000)|16;HH(0x04000060)&=0xcffb;HH(0x04000060)=(HH(0x04000060)&~0x3000)|8;HH(0x04000060)=(HH(0x04000060)&~0x3000)|32;G3X_SetEdgeColorTable(ov71_0224BD0C);G3X_SetClearColor(0x43ff,0,0x7fff,63,0);WW(0x04000540)=2;WW(0x04000580)=0xbfff0000;GF_3DVramMan_InitFrameTexVramManager(1,1);GF_3DVramMan_InitFramePlttVramManager(0x4000,1);GfGfx_EngineATogglePlanes(1,1);G2_SetBG0Priority(0);*(void**)(w+0xd4)=TradeSequence_CreateModels(1,0,0,0);*(void**)(w+0xd8)=TradeSequence_LoadModel(*(void**)(w+0xd4),0,89,27,0,(s32)0xfffc8800,0,0);*(u16*)(ov71_0224C0C4+4)=0xebe0;*(u16*)(ov71_0224C0C4+6)=0;*(u16*)(ov71_0224C0C4+8)=0;TradeSequence_SetCameraAngle(*(void**)(w+0xd4),ov71_0224C0C8);TradeSequence_SetModelScale(*(void**)(w+0xd8),0x1c00);}
