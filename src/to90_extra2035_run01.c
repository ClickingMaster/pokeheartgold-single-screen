#pragma force_active on
#include "global.h"
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define C(p,o) (*(s8*)((u8*)(p)+(o)))
extern const u8 ov71_0224BEC0[],ov71_0224BE84[],ov71_0224BEA4[];
extern void GfGfx_SetBanks(const void*),SetBothScreensModesAndDisable(const void*),InitBgFromTemplate(void*,int,const void*,int);
extern int TradeSequence_GetTimeOfDay(void*);
extern void GfGfxLoader_LoadCharData(int,int,void*,int,int,int,int,int),GfGfxLoader_LoadScrnData(int,int,void*,int,int,int,int,int);
extern void GfGfxLoader_GXLoadPalWithSrcOffset(int,int,int,int,int,int,int),BgSetPosTextAndCommit(void*,int,int,int),GfGfx_EngineATogglePlanes(int,int),GfGfx_EngineBTogglePlanes(int,int),WirelessStatusIcon_Create(void),sub_0203A948(int,int);
void ov71_0224A278(void*);
#pragma force_active on
void ov71_0224A278(void*w){int screen,ch,pal,offset;
 GfGfx_SetBanks(ov71_0224BEC0);*(volatile u16*)0x04000304|=0x8000;SetBothScreensModesAndDisable(ov71_0224BE84);
 InitBgFromTemplate(P(w,0x24),3,ov71_0224BEA4,0);InitBgFromTemplate(P(w,0x24),7,ov71_0224BEA4,0);
 switch(TradeSequence_GetTimeOfDay(P(w,0))){default:case 0:screen=15;ch=16;pal=17;offset=0;break;case 1:screen=15;ch=16;pal=17;offset=32;break;case 2:screen=15;ch=16;pal=17;offset=64;break;case 3:screen=24;ch=25;pal=26;offset=0;break;}
 GfGfxLoader_LoadCharData(89,ch,P(w,0x24),3,0,0,1,57);GfGfxLoader_LoadCharData(89,ch,P(w,0x24),7,0,0,1,57);
 GfGfxLoader_LoadScrnData(89,screen,P(w,0x24),3,0,0,1,57);GfGfxLoader_LoadScrnData(89,screen,P(w,0x24),7,0,0,1,57);
 GfGfxLoader_GXLoadPalWithSrcOffset(89,pal,0,offset,0,32,57);GfGfxLoader_GXLoadPalWithSrcOffset(89,pal,4,offset,0,32,57);
 BgSetPosTextAndCommit(P(w,0x24),3,3,0);BgSetPosTextAndCommit(P(w,0x24),7,3,256);
 GfGfx_EngineATogglePlanes(16,1);GfGfx_EngineBTogglePlanes(16,1);WirelessStatusIcon_Create();sub_0203A948(1,56);
}
