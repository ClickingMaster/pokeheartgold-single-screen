#ifndef TO50_OVERLAY65_R14_R2_PRIVATE_H
#define TO50_OVERLAY65_R14_R2_PRIVATE_H
#include "global.h"

#include "overlay_manager.h"
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
extern int sub_0203769C(void);
extern void *sub_02034818(int);
extern void *PlayerProfile_GetPlayerName_NewString(void *, int);
extern void *NewString_ReadMsgData(void *, int);
extern void ov65_0221FB4C(void *, void *, int, int, int, int);
extern void String_Delete(void *);
extern void *NARC_New(int, int);
extern void NARC_Delete(void *);
extern void ov65_0221CE98(void *);
extern void ov65_0221CEB8(void *);
extern void ov65_0221D280(void *, void *);
extern u32 Options_GetFrame(void *);
extern void LoadUserFrameGfx2(void *, int, int, int, int, int);
extern void LoadUserFrameGfx1(void *, int, int, int, int, int);
extern void ov65_0221FB90(void *, int, int, void *, void *);
extern void ToggleBgLayer(int, int);
extern void GfGfx_EngineBTogglePlanes(int, int);
extern void GfGfx_EngineATogglePlanes(int, int);
extern void ov65_0221D3B8(void);
extern void ov65_0221D3E8(void *, void *);
extern void ov65_0221D930(void *);
extern void ov65_0221C46C(void *, int, void *);
extern void Sprite_SetDrawFlag(void *, int);
extern void Main_SetVBlankIntrCB(void (*)(void *), void *);
extern void ov65_0221CE1C(void *);
extern void ov65_0221C3DC(void *, void *, int);
extern void ov65_0221C414(void *, void *, void *, void *, int);
extern int Party_GetCount(void *);
extern void ov65_0221D25C(void *, int, int);
extern BOOL ov65_0221C5E0(void *);
extern int ov65_0221CC0C(void *);
extern void BeginNormalPaletteFade(int, int, int, int, int, int, int);
extern BOOL IsPaletteFadeFinished(void);
extern void ov65_0221CD0C(void *);
extern void ov65_0221CCB0(void *);
extern void ov65_0221D1C8(void *);
extern void ov65_0221E06C(void *, int);
extern void ov65_0221CB5C(void *);
extern void ov65_0221DD34(int, void *, int);
extern void sub_0203A880(void);
extern void ov65_0221F714(void *);
extern void SpriteList_RenderAndAnimateSprites(void *);
extern void sub_020399FC(int, void *);
extern int _s32_div_f(int, int);
void ov65_0221BFEC(void *);
void ov65_0221C08C(void *);
void ov65_0221C1C4(void *);
BOOL WirelessTradeSelectMon_Main(OverlayManager *, int *);
#endif
