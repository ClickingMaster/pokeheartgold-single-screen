#ifndef TO50_OV65_R14_R1_PRIVATE_H
#define TO50_OV65_R14_R1_PRIVATE_H
#include "global.h"

#include "overlay_manager.h"
#include "system.h"
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
extern void GfGfx_DisableEngineAPlanes(void);
extern void GfGfx_DisableEngineBPlanes(void);
extern void FontID_Alloc(u32, u32);
extern void *NARC_New(u32, u32);
extern void NARC_Delete(void *);
extern void *BgConfig_Alloc(u32);
extern void *MessageFormat_New_Custom(u32, u32, u32);
extern void *MessageFormat_New(u32);
extern void *NewMsgDataFromNarc(u32, u32, u32, u32);
extern void ov65_0221D0EC(void *, OverlayManager *);
extern void ov65_0221CE98(void);
extern void ov65_0221CEB8(void *);
extern void BeginNormalPaletteFade(u32, u32, u32, u32, u32, u32, u32);
extern void ov65_0221D280(void *, void *);
extern void ov65_0221CE1C(void *);
extern void ov65_0221D3B8(void);
extern void ov65_0221D3E8(void *, void *);
extern void ov65_0221D930(void *);
extern void ov65_0221F8D0(void *, void *, void *);
extern void sub_0208F7E0(void *, void *);
extern void ov65_0221FB90(void *, u32, u32, void *, void *);
extern void sub_0208F658(void *);
extern void ToggleBgLayer(u32, u32);
extern void GfGfx_EngineATogglePlanes(u32, u32);
extern void sub_020399FC(u32, void *);
extern void WirelessStatusIcon_Create(void);
extern u32 IsNighttime(void);
extern void sub_02055198(u32, u32);
extern void sub_02005448(u32);
BOOL WirelessTradeSelectMon_Init(OverlayManager *, int *);
#endif
