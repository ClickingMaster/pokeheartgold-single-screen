#ifndef TO56_W10_PRIVATE_H
#define TO56_W10_PRIVATE_H
#include "global.h"

typedef struct FadeWork {
    void *owner;
    void *palette;
    int state;
    int count;
    int unk10;
    int unk14;
    int unk18;
    int delay;
} FadeWork;

typedef struct ScaleWork {
    void *owner;
    void *spriteSystem;
    int state;
    s16 x;
    s16 y;
    void *sprite;
    u8 anim[0x24];
    int a;
    int b;
    int c;
    int d;
    int count;
    int mode;
    int flag;
} ScaleWork;

typedef struct MoveWork {
    void *owner;
    int state;
    s16 x;
    s16 y;
    void *pic;
    u8 anim[0x24];
    int a;
    int b;
    int c;
    int d;
    int e;
    int delay;
    int packed;
    int hold;
} MoveWork;

void GF_AssertFail(void);
void Heap_Free(void *);
void *BattleAnimUtil_AllocWork(void *, int);
void *BattleAnimSystem_GetPaletteData(void *);

extern u32 BattleAnimSystem_GetArg(void *p, int i);
int BattleAnimSystem_GetBattleBgPaletteMask(void *);
void *BattleAnimSystem_CreateTaskDefaultPriority(void *, void *, void *);
void BattleAnimSystem_DestroyTask(void *, void *);
void *BattleAnimSystem_GetBattlerSpriteManager(void *);
u16 BattleAnimSystem_GetAttacker(void *);
u16 BattleAnimSystem_GetDefender(void *);
void *BattleAnimSystem_GetBattlerPokepic(void *, int);
int ov07_0221FAA0(void *, int);
int BattleAnimSystem_IsDoubleBattle(void *);
int BattleAnimSystem_GetBattlerType(void *, int);
int BattleAnimUtil_GetPartnerBattler(void *, int);
int BattleAnimSystem_GetBattlerSprite(void *, int);
int ov07_0221FB78(void *, int);
void BattleAnimUtil_SetBlendWithBattleBg(void *, int, int);
int PaletteData_BeginPaletteFade(void *, int, u16, s8, u8, u8, u16);
void Pokepic_StartPaletteFade(void *, int, int, int, int);
int Pokepic_ResumePaletteFade(void *);
int Pokepic_GetAttr(void *, int);
void ManagedSprite_SetDrawPriority(void *, int);
void ManagedSprite_SetPriority(void *, int);
void ManagedSprite_SetOamMode(void *, int);
void ManagedSprite_SetAffineOverwriteMode(void *, int);
void ManagedSprite_SetAffineScale(void *, u32, u32);
void SpriteSystem_DrawSprites(void *);
u32 _fsub(u32, u32);
void ov07_02222508(void *, s16, s16, s16, int);
int ov07_02222558(void *);
void ov07_02222644(void *, u32 *, u32 *);
void ov07_022226FC(void *, s16, s16, int, int);
void ov07_02222590(void *, s16, s16, s16, s16, s16, int);
int ov07_02222768(void *, void *);
int ov07_02222788(void *, void *);
void ov07_022226C4(void *, s16, s16, int, int);
void ov07_02223224(void *);
void BattleAnimUtil_CollectPicsByFlags(void *, int, void *, void *);
void ov07_02223E94(void *, void *);
void ov07_02223EB4(void *);
void ov07_02224008(void *);
void ov07_02224348(void *);
void ov07_022245BC(void *);
void ov07_02223F5C(void *, FadeWork *);
void ov07_022241D8(void *, ScaleWork *);
void ov07_0222446C(void *, MoveWork *);
#endif
