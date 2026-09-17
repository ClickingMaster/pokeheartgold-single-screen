#ifndef TO50_OV65_R12_PRIVATE_H
#define TO50_OV65_R12_PRIVATE_H
#include "global.h"
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define FN(p, o)  (*(void (**)(void *))((u8 *)(p) + (o)))
extern u32 TouchscreenListMenu_HandleInput(void *);
extern void FillBgTilemapRect(void *, u32, u32, u32, u32, u32, u32, u32);
extern u32 ov65_0221FB90(void *, u32, u32, u32, void *);
extern void ov65_0221E050(void *);
extern void ov65_0221F4B0(void *);
extern void ListMenuItems_Delete(void *);
extern void *AllocMonZeroed(u32);
extern void *Party_GetMonByIndex(void *, u32);
extern void CopyPokemonToPokemon(void *, void *);
extern u32 GetMonData(void *, u32, void *);
extern void SetMonData(void *, u32, void *);
extern void *Save_VarsFlags_Get(void *);
extern u32 Save_VarsFlags_GetVar404C(void *);
extern void Save_VarsFlags_SetVar404C(void *, u32);
extern u32 sub_0203769C(void);
extern void *sub_02034818(u32);
extern void MonSetTrainerMemo(void *, void *, u32, u32, u32);
extern void Pokemon_RemoveCapsule(void *);
extern void PlayerProfile_Copy(void *, void *);
extern u32 Party_HasMon(void *, u32);
extern void *Save_Chatot_Get(void *);
extern void Chatot_Invalidate(void *);
extern void UpdatePokedexWithReceivedSpecies(void *, void *);
extern void Party_ResetMonAprijuiceModifiers(void *, u32);
extern void GameStats_Inc(void *, u32);
extern void Heap_Free(void *);
int ov65_0221F4D4(void *);
void ov65_0221F5A0(void *, void *, u32, u32, void *);
#endif
