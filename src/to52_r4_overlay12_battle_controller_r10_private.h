#ifndef TO52_R4_W05_PRIVATE_H
#define TO52_R4_W05_PRIVATE_H
#include "global.h"
#define U8(p,o) (*(u8 *)((u8 *)(p)+(o)))
#define U16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
void *BattleSystem_GetPartyMon(void *, int, int);
u32 GetMonData(void *, int, void *);
u32 GetMonExpBySpeciesAndLevel(u32, u32);
void ov12_02262240(void *, int, int, void *, int);
void BattleController_EmitHealthbarUpdate(void *, void *, int);
void ov12_02263564(void *, void *, int, u32);
void BattleController_EmitPlayFaintAnimation(void *, void *, int);
#endif
