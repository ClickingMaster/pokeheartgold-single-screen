#pragma force_active on
#include "global.h"
#include "gf_3d_vramman.h"
#pragma force_active on
extern const u16 ov71_0224BE94[];
void GfGfx_EngineATogglePlanes(int,int);
void *ov71_0224744C(int,int,int,int),*ov71_02247610(void *,int,int,int,int,int,int,int);
void ov71_0224780C(void *,void *),ov71_0224781C(void *,void *),ov71_0224783C(void *,int),ov71_022477EC(void *,void *),ov71_0224A3F0(u8 *);
#define R16(a) (*(volatile u16 *)(a))
#define R32(a) (*(volatile u32 *)(a))
#define P(o) (*(void **)(w+(o)))
void ov71_0224A3F0(u8 *w){u16 v[3];VecFx32 p;NNS_G3dInit();G3X_InitMtxStack();R16(0x04000060)&=~0x3002;R16(0x04000060)=(R16(0x04000060)&~0x3000)|16;R16(0x04000060)&=0xcffb;R16(0x04000060)=(R16(0x04000060)&~0x3000)|8;R16(0x04000060)=(R16(0x04000060)&~0x3000)|32;G3X_SetEdgeColorTable(ov71_0224BE94);G3X_SetClearColor(0x43ff,0,0x7fff,63,FALSE);R32(0x04000540)=2;R32(0x04000580)=0xbfff0000;GF_3DVramMan_InitFrameTexVramManager(1,1);GF_3DVramMan_InitFramePlttVramManager(16384,1);GfGfx_EngineATogglePlanes(1,1);R16(0x04000008)=(R16(0x04000008)&~3)|0;P(40)=ov71_0224744C(1,0,0,0);P(44)=ov71_02247610(P(40),0,89,27,0,0,0,0);v[0]=6371;v[1]=0;v[2]=0;ov71_0224780C(P(40),v);v[0]=4432;ov71_0224781C(P(40),v);ov71_0224783C(P(40),2002);ov71_022477EC(P(40),&p);}
