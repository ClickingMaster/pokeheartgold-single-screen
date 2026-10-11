#ifndef TO53_W02_PRIVATE_H
#define TO53_W02_PRIVATE_H
#include "global.h"
#include "heap.h"
typedef struct BattleSystem BattleSystem;

void PlaySE(u16);
void StopSE(u16, int);
void GF_AssertFail(void);
void BattleAnimSystem_InitBlend(void);
void BattleAnimSystem_ConvertPan(void *, s8);
void SetAllSEPan(void);
void *BattleAnimBattlerSprites_New(BattleSystem *, enum HeapID, u8);
void *NARC_AllocAndReadWholeMember(u32, u32, u32);
void ov06_0221BAD8(void);
void ParticleSystem_CreateEmitterWithParam(void *, void *, void *);
void ov07_022207D0(void *, int, int);
void ov07_02221EEC(void *, void *);
void BattleAnimSystem_ReadScriptArgs(void *, void *, int);

void BattleAnimScriptCmd_WaitForBackgroundLoaded(void *);
void BattleAnimScriptCmd_WaitForBackgroundSwitch(void *);
void BattleAnimScriptCmd_Nop37(void);
void BattleAnimScriptCmd_Nop38(void);
void BattleAnimScriptCmd_Nop39(void);
void BattleAnimScriptCmd_Nop42(void);
void BattleAnimScriptCmd_Nop43(void);
void BattleAnimScriptCmd_Nop28(void);
void BattleAnimScriptCmd_ResetBlend(void);
void BattleAnimScriptCmd_Nop27(void);
void BattleAnimScriptCmd_Invalid55(void);
void BattleAnimScriptCmd_Nop54(void);
u32 BattleAnimSystem_GetScriptCmd(u32);
u8 ov07_0221F8C4(void *);
u32 BattleAnimSystem_GetPaletteData(void *);
struct Outer;
u32 ov07_0221FA80(struct Outer *, int);
u32 ov07_0221FA90(struct Outer *, int);
u32 ov07_0221FAA0(struct Outer *, int);
BOOL BattleAnimSystem_IsDoubleBattle(void *);
BOOL ov07_0221FAE8(void *);
u32 BattleAnimSystem_GetBgLayer(void *, int);
u32 ov07_0221FAF8(void *, int);
BOOL ov07_0221FB78(void);
u32 BattleAnimSystem_GetBackgroundFile(int, int);
void *BattleAnimBattlerSprites_NewAll(BattleSystem *, enum HeapID);
u8 ov07_0221FE70(void *);
void BattleAnimEmitterCb_None(void);
void *BattleAnimSystem_ReadNarcMember(u32, u32, u32);
void BattleAnimSystem_FreeParticleSystem(void);
void ov07_0221FF74(void);
void ov07_02220800(void *);
void ov07_0222080C(void *);
void ov07_02220818(void *);
void ov07_02220824(void *);
void ov07_02220830(void *);
void ov07_0222083C(void *);
void ov07_02220848(void *);
void ov07_02220854(void *);
void ov07_02220860(void *);
void ov07_0222086C(void *);
void ov07_02220878(void *);
void ov07_022217A4(void *, void *);
BOOL ov07_022217B0(void);
BOOL ov07_02221898(void);
BOOL ov07_0222189C(void);
BOOL ov07_02221AD4(void);
BOOL ov07_02221AD8(void);
BOOL ov07_02221ADC(void);
BOOL ov07_02221AE0(void);
void ov07_02221C74(int, void *, void *);
void ov07_02221CE0(void *, void *);

#endif
