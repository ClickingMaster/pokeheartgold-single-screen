#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "render_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "palette.h"
#include "unk_0200FA24.h"
#pragma force_active on
extern const u8 ov71_0224BCBC[],ov71_0224BC74[],ov71_0224BC84[],ov71_0224BCA0[];
typedef struct {u16 a,b,c;} Triple;
extern const Triple ov71_0224BC5C;
extern u32 TradeSequence_GetFrameType(void *);
extern void ov71_02247ED0(void *),ov71_02247FF8(void *);
extern void *ov71_0224744C(int,int,int,int);
extern void *ov71_02247F9C(void *);
extern void *ov71_02247610(void *,int,int,int,int,int,int,int);
extern void ov71_022476EC(void *,Triple *);
extern void WirelessStatusIcon_Create(void),sub_0203A948(int,int);
int ov71_02247A10(u8 *);
#define P(x) (*(void **)(p+(x)))
int ov71_02247A10(u8 *p){
 Triple vec;
 GfGfx_SetBanks((const GraphicsBanks *)ov71_0224BCBC);
 *(volatile u16 *)0x04000304 &=~0x8000;
 SetBothScreensModesAndDisable((const GraphicsModes *)ov71_0224BC74);
 InitBgFromTemplate(P(0x54),1,(const BgTemplate *)ov71_0224BC84,0);
 InitBgFromTemplate(P(0x54),2,(const BgTemplate *)ov71_0224BCA0,0);
 InitBgFromTemplate(P(0x54),6,(const BgTemplate *)ov71_0224BCA0,0);
 GfGfx_EngineATogglePlanes(1,1);
 *(volatile u16 *)0x04000008=(*(volatile u16 *)0x04000008 &~3)|1;
 BG_FillCharDataRange(P(0x54),(GFBgLayer)1,0,1,0);
 FillBgTilemapRect(P(0x54),1,0,0,0,32,32,0);
 LoadUserFrameGfx2(P(0x54),(GFBgLayer)1,0x6d,2,(u8)TradeSequence_GetFrameType(P(0)),(enum HeapID)0x39);
 AddWindowParameterized(P(0x54),(Window *)(p+0x58),1,2,19,27,4,1,1);
 FillWindowPixelBuffer((Window *)(p+0x58),15);
 GfGfxLoader_GXLoadPal((NarcId)16,8,(enum GFPalLoadLocation)0,(enum GFPalSlotOffset)32,32,(enum HeapID)0x39);
 GfGfxLoader_LoadCharData((NarcId)0x59,22,P(0x54),(GFBgLayer)2,0,0,1,(enum HeapID)0x39);
 GfGfxLoader_LoadScrnData((NarcId)0x59,21,P(0x54),(GFBgLayer)2,0,0,1,(enum HeapID)0x39);
 GfGfxLoader_GXLoadPal((NarcId)0x59,23,(enum GFPalLoadLocation)0,(enum GFPalSlotOffset)0,32,(enum HeapID)0x39);
 GfGfxLoader_LoadCharData((NarcId)0x59,22,P(0x54),(GFBgLayer)6,0,0,1,(enum HeapID)0x39);
 GfGfxLoader_LoadScrnData((NarcId)0x59,21,P(0x54),(GFBgLayer)6,0,0,1,(enum HeapID)0x39);
 GfGfxLoader_GXLoadPal((NarcId)0x59,23,(enum GFPalLoadLocation)4,(enum GFPalSlotOffset)0,32,(enum HeapID)0x39);
 BgCommitTilemapBufferToVram(P(0x54),1);
 ov71_02247ED0(p);
 P(0x70)=ov71_0224744C(1,0,0,0);P(0x10)=ov71_02247F9C(p);
 P(0x74)=ov71_02247610(P(0x70),0,0x59,27,0,0x1a800,0x73800,0);
 vec=ov71_0224BC5C;ov71_022476EC(P(0x74),&vec);
 GfGfx_EngineATogglePlanes(16,1);GfGfx_EngineBTogglePlanes(16,1);
 ov71_02247FF8(p);*(volatile u16 *)0x04000050=0;
 WirelessStatusIcon_Create();sub_0203A948(1,0x38);
 BeginNormalPaletteFade(0,1,1,0,16,1,(enum HeapID)0x39);return 1;
}
