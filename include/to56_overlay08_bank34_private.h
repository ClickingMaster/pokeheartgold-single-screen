#ifndef TO56_R3_W12_PRIVATE_H
#define TO56_R3_W12_PRIVATE_H
#include "global.h"
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
extern void ManagedSprite_SetDrawFlag(void *, BOOL);
extern void ov08_02223F74(void *, int, int);
extern void ov08_02223E3C(void *, u32, u32);
extern void ov08_02223E74(void *, u32, u32, u32);
extern u32 ov08_02223CD4(void *, u32);
extern const s32 ov08_02225D74[][2];
extern const u32 ov08_02225CF8[];
extern void *BattleDpadCursor_GetBattleCursor(void *);
extern void BattleDpadCursor_SetBoxes(void *, u32);
extern void BattleDpadCursor_SetPosition(void *, u32);
extern void ov08_02223FD8(u8 *);
extern void ov08_0222400C(u8 *);
extern void ov08_02224064(u8 *);
void ov08_02223F94(u8 *, u32);
void ov08_022240A8(u8 *);
void ov08_02224108(u8 *);
extern void *BattleSystem_GetSpriteSystem(void *);
extern void BattleCursor_LoadResources(void *, void *, void *, u32, u32, u32, u32, u32);
extern void *BattleCursor_New(void *, void *, u32, u32, u32, u32, u32, u32, u32);
extern void BattleCursor_Delete(void *);
extern void BattleCursor_FreeResources(void *, u32, u32, u32, u32);
extern void BattleDpadCursor_SetBattleCursor(void *, void *);
void ov08_02224134(u8 *, u32);
#endif
