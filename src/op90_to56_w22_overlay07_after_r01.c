#pragma force_active on
#include "global.h"

typedef struct ManagedSprite ManagedSprite;
typedef struct SpriteSystem SpriteSystem;
typedef struct Pokepic Pokepic;

void *BattleAnimUtil_AllocWork(void *, u32);
void BattleAnimUtil_InitCommonWork(void *, void *);
void *BattleAnimSystem_CreateTaskDefaultPriority(void *, void *, void *);
void BattleAnimSystem_DestroyTask(void *, void *);
void BattleAnimSystem_GetSpriteTemplate(void *, void *);
void ov07_0222A8D8(ManagedSprite *);
void ManagedSprite_SetDrawFlag(ManagedSprite *, BOOL);
void ManagedSprite_SetFlipMode(ManagedSprite *, int);
void ManagedSprite_OffsetPositionXY(ManagedSprite *, int, int);
void Sprite_DeleteAndFreeResources(ManagedSprite *);
void SpriteSystem_DrawSprites(SpriteSystem *);
void Heap_Free(void *);
ManagedSprite *SpriteSystem_NewSprite(void *, void *, void *);

void BattleAnimUtil_FreeWork(void *);
BOOL ov07_0222260C(void *);
void ov07_02222644(void *, s32 *, s32 *);
void ov07_022226FC(ManagedSprite *, s32, s32, u32, u32);
void ManagedSprite_SetAffineScale(ManagedSprite *, float, float);
void ManagedSprite_TickFrame(ManagedSprite *);
void ManagedSprite_SetAffineOverwriteMode(ManagedSprite *, int);
void ManagedSprite_SetPriority(ManagedSprite *, int);
void ManagedSprite_SetDrawPriority(ManagedSprite *, int);
void ov07_02222590(void *, int, int, int, int, int, int);
u32 BattleAnimSystem_GetDefender(void *);
Pokepic *BattleAnimSystem_GetBattlerPokepic(void *, u32);
u32 ov07_0221FAA0(void *, u32);
ManagedSprite *BattleAnimSystem_GetBattlerSprite(void *, int);
int ov07_0221FAE8(void *);
u32 BattleAnimSystem_GetBattlerType(void *, u32);
u32 Pokepic_GetAttr(Pokepic *, int);
void ov07_02222268(void *, s32, s32, s32, s32, s32);
BOOL ov07_022222B4(void *);
void Pokepic_SetAttr(Pokepic *, int, int);
void BattleAnimSystem_BuildBattlerPicParams(void *, void *, int);
void *BattleAnimSystem_GetHeapID(void *);
void BattleAnimUtil_RestorePic(void *, void *);
void BattleAnimUtil_LoadMonIntoPicOppositeFacing(void *, void *);
u32 BattleAnimSystem_GetAttacker(void *);
void BattleAnimUtil_GetBattlerPosition(void *, u32, s16 *);
u32 BattleAnimUtil_GetBattlerSide(void *, u32);

extern u32 BattleAnimSystem_GetArg(void *p, int i);
void BattleAnimUtil_CollectPicsByFlags(void *, void *, s16 *, void *);
void *Sprite_GetPaletteProxy(void *);
u32 ObjPlttTransfer_GetPaletteVramOffset(void *, int);
BOOL ov07_02222EE8(void *);
void ov07_02222EF8(void *);
void *ov07_02222F10(void *, void *, int, int, int, int, int, int, int, int, int);

typedef struct B4Work {
    u8 state;
    u8 pad01[3];
    void *owner;
    u8 pad08[4];
    SpriteSystem *spriteSystem;
    u8 pad10[8];
    void *effectManager;
    ManagedSprite *sprite1;
    ManagedSprite *sprite2;
    ManagedSprite *sprite3;
    s32 timer;
    float scaleX;
    float scaleY;
    u8 blendA;
    u8 blendB;
    s8 direction;
    u8 pad37;
    void *effect1;
    void *effect2;
} B4Work;

int ov07_02222004(void *, int);
int ov07_0221FB78(void *, int);
int BattleAnimUtil_GetBattlerType(void *, int);
int ov07_0221BFC0(void *);
void BattleAnimUtil_GetBattlerTypePosition(int, int, s16 *);
void BattleAnimUtil_SetBlendWithBattleBg(void *, int, int);
int BattleAnimSystem_GetBattlerSpecies(void *, int);
int BattleAnimSystem_GetBattlerGender(void *, int);
int BattleAnimSystem_GetBattlerForm(void *, int);
u32 BattleAnimSystem_GetBattlerPersonality(void *, int);
u8 GetMonPicHeightBySpeciesGenderForm(u16 species, u8 gender, u8 whichFacing, u8 form, u32 pid);
void ManagedSprite_GetPositionXY(ManagedSprite *, s16 *, s16 *);
void ManagedSprite_SetPositionXY(ManagedSprite *, s16, s16);
void ov07_0222B4E0(void *task, B4Work *work);
void ov07_0222B640(void *owner);

// @fn ov07_0222B640
void ov07_0222B640(void *owner) {
    s16 curY, curX;
    s16 origX, origY;
    s16 base[2];
    B4Work *work;
    int offset;
    u32 paletteOffset;
    u8 battler;
    u8 type;
    int facing;
    int species;
    int gender;
    int form;
    int height;
    void *task;

    work = BattleAnimUtil_AllocWork(owner, sizeof(B4Work));
    BattleAnimUtil_InitCommonWork(owner, work);
    work->sprite1 = BattleAnimSystem_GetBattlerSprite(work->owner, 0);
    work->sprite2 = BattleAnimSystem_GetBattlerSprite(work->owner, 1);
    work->sprite3 = BattleAnimSystem_GetBattlerSprite(work->owner, 2);
    offset = 0;
    work->timer = offset;
    offset = -32;
    offset = (s16)ov07_02222004(owner, BattleAnimSystem_GetAttacker(owner)) * offset;
    work->scaleX = 1.0f;
    work->scaleY = 1.0f;
    if (ov07_0221FB78(owner, 1) == 1) {
        work->direction = -1;
    } else {
        work->direction = 1;
    }
    ManagedSprite_GetPositionXY(work->sprite2, &origX, &origY);
    ManagedSprite_SetPositionXY(work->sprite1, origX + offset, origY);
    ManagedSprite_SetAffineOverwriteMode(work->sprite1, 2);
    ManagedSprite_SetAffineScale(work->sprite1, work->scaleX * (float)work->direction, work->scaleY);

    paletteOffset = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(*(void **)work->sprite1), 1);
    work->effect1 = ov07_02222F10(work->effectManager, BattleAnimSystem_GetHeapID(owner), 2, (u16)(paletteOffset << 4), 16, 0, 1, 0, 15, 0xFFFF, 1100);
    paletteOffset = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(*(void **)work->sprite3), 1);
    work->effect2 = ov07_02222F10(work->effectManager, BattleAnimSystem_GetHeapID(owner), 2, (u16)(paletteOffset << 4), 16, 0, 1, 0, 15, 0xFFFF, 1100);

    battler = BattleAnimSystem_GetAttacker(work->owner);
    type = BattleAnimUtil_GetBattlerType(work->owner, battler);
    BattleAnimUtil_GetBattlerTypePosition(type, ov07_0221BFC0(work->owner), base);
    ManagedSprite_GetPositionXY(work->sprite1, &curX, &curY);
    if (BattleAnimUtil_GetBattlerSide(work->owner, battler) == 3) {
        facing = 0;
        ManagedSprite_SetPriority(work->sprite1, 1);
        ManagedSprite_SetDrawPriority(work->sprite1, facing);
    } else {
        ManagedSprite_SetPriority(work->sprite1, 2);
        ManagedSprite_SetDrawPriority(work->sprite1, 0);
        facing = 2;
    }
    battler = BattleAnimSystem_GetDefender(work->owner);
    species = BattleAnimSystem_GetBattlerSpecies(work->owner, battler);
    gender = BattleAnimSystem_GetBattlerGender(work->owner, battler);
    form = BattleAnimSystem_GetBattlerForm(work->owner, battler);
    height = GetMonPicHeightBySpeciesGenderForm(species, gender, facing, form, BattleAnimSystem_GetBattlerPersonality(work->owner, battler));
    ManagedSprite_SetPositionXY(work->sprite1, curX, base[1] + height);
    ManagedSprite_SetDrawFlag(work->sprite1, TRUE);
    work->blendA = 0;
    work->blendB = 15;
    BattleAnimUtil_SetBlendWithBattleBg(work->owner, -1, -1);
    *(vu16 *)0x04000052 = work->blendA | (work->blendB << 8);
    task = BattleAnimSystem_CreateTaskDefaultPriority(work->owner, ov07_0222B4E0, work);
    ov07_0222B4E0(task, work);
}
