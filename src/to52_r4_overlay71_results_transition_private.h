#ifndef W12_OV71_R30_PRIVATE_H
#define W12_OV71_R30_PRIVATE_H
#include "global.h"
typedef u8 Ov71R30;
#define U32_(p,o) (*(u32 *)((p)+(o)))
#define S32_(p,o) (*(s32 *)((p)+(o)))
#define PTR_(p,o) (*(void **)((p)+(o)))
BOOL ov71_02249D80(Ov71R30 *, u32 *);
void ov71_02249E24(void *, Ov71R30 *);
void ov71_02249E6C(Ov71R30 *);
extern BOOL IsPaletteFadeFinished(void);
extern void PlaySE(u32);
extern void BeginNormalPaletteFade(u32,u32,u32,u32,u32,u32,u32);
extern void *SysTask_CreateOnVBlankQueue(void (*)(void *, Ov71R30 *), void *, u32);
extern void SysTask_Destroy(void *);
extern void ov71_0224A0B8(void *, s32, s32);
extern void GfGfx_SetBanks(const void *);
extern void SetBothScreensModesAndDisable(const void *);
extern void InitBgFromTemplate(void *, u32, const void *, u32);
extern void GfGfxLoader_LoadCharData(u32,u32,void *,u32,u32,u32,u32,u32);
extern void GfGfxLoader_LoadScrnData(u32,u32,void *,u32,u32,u32,u32,u32);
extern void GfGfxLoader_GXLoadPal(u32,u32,u32,u32,u32,u32);
extern void BG_FillCharDataRange(void *,u32,u32,u32,u32);
extern void ov71_02247124(void *,u32,u32,u32,u32);
extern void FillBgTilemapRect(void *,u32,u32,u32,u32,u32,u32,u32);
extern void ov71_02247230(void *,u32,u32,u32,u32,u32);
extern void BgCommitTilemapBufferToVram(void *,u32);
extern void GfGfx_EngineATogglePlanes(u32,u32);
extern void GfGfx_EngineBTogglePlanes(u32,u32);
extern u8 ov71_0224BDF0[];
extern u8 ov71_0224BE00[];
extern u8 ov71_0224BE1C[];
extern u8 ov71_0224BE38[];
extern u8 ov71_0224BE54[];
#endif
