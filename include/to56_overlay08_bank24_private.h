#ifndef TO56_R2_W12_PRIVATE_H
#define TO56_R2_W12_PRIVATE_H
#include "global.h"

#include "bg_window.h"
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
extern void PaletteData_BeginPaletteFade(void *, int, u16, int, int, int, u16);
extern u32 PaletteData_GetSelectedBuffersBitmask(void *);
extern void *NewMsgDataFromNarc(u32, u32, u32, u32);
extern void *MessagePrinter_New(u32, u32, u32, u32);
extern void *MessageFormat_New(u32);
extern void *String_New(u32, u32);
int ov08_02222E04(u8 *);
void ov08_022230CC(void *);
extern void ov08_02223F34(u8 *);
extern void ov08_02223464(u8 *);
extern void ov08_02223228(u8 *);
extern void DestroySysTaskAndEnvironment(void *);
extern void *BattleSystem_GetBagCursor(void *);
extern void BagCursor_Battle_PocketSetPosition(void *, u32, u32, u32);
extern void BagCursor_Battle_SetPocket(void *, u32);
extern u8 BattleDpadCursor_IsActive(void *);
extern void BattleDpadCursor_Free(void *);
extern void FontID_Release(u32);
int ov08_02222E2C(void *, u8 *);
extern const GraphicsModes ov08_02225AE8;
extern const BgTemplate ov08_02225AF8;
extern const BgTemplate ov08_02225B14;
extern const BgTemplate ov08_02225B30;
void ov08_02223000(u8 *);
void ov08_022231E8(u8 *);
#endif
