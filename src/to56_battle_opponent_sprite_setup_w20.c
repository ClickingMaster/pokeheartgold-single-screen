#include "to56_battle_opponent_sprite_setup_w20_private.h"

typedef struct W20Input {
    u8 command;
    u8 gender : 2;
    u8 shiny : 1;
    u8 form : 5;
    u16 species;
    u32 personality;
    u32 personality2;
    u32 value0C;
    u32 value10;
    u8 pad14[0x18];
    u32 value2C;
} W20Input;

typedef struct W20InputF30 {
    u8 command;
    u8 flags;
    u16 species;
    u32 personality;
    u16 values08[4];
    u8 values10[4];
    u8 values14[4];
    u8 values18[4];
    u32 values1C[4];
    u8 pad2C[0x0C];
} W20InputF30;
typedef struct W20Work {
    BattleSystem *battleSystem;
    OpponentData *opponentData;
    Pokepic *pokepic;
    u8 pad0C[0x1C];
    u16 values28[4];
    u8 values30[4];
    u8 values34[4];
    u8 values38[4];
    u32 values3C[4];
    u8 pad4C[0x1C];
    u8 command;
    u8 battlerId;
    u8 battlerType;
    u8 unk6B;
    u8 form;
    u8 pad6D;
    u16 species;
    u32 personality;
    u32 value74;
} W20Work;

extern void ov12_0225C9BC(SysTask *, void *);
extern void ov12_0225CC58(SysTask *, void *);

void ov12_02259F30(BattleSystem *, OpponentData *, W20InputF30 *);

/* definition moved: ov12_02259F30 */

typedef struct W20Work9C {
    BattleSystem *battleSystem;
    OpponentData *opponentData;
    u8 pad08[0x0C];
    PokepicTemplate pokepicTemplate;
    u8 pad24[0x5C];
    u8 command;
    u8 battlerId;
    u8 battlerType;
    u8 state;
    u8 facing;
    u8 height;
    u16 species;
    u32 personality;
    u8 value8C;
    u8 nature;
    u16 value8E;
    s8 value90;
    s8 value91;
    u8 shiny;
    u8 value93;
    u16 value94;
    u8 value96;
    u8 form;
    u32 value98;
} W20Work9C;

extern void ov12_02261284(OpponentData *);
extern void sub_02005B58(u8);
extern void ov12_0225B960(SysTask *, void *);
extern void ov12_0225BE38(SysTask *, void *);

void ov12_02259BA8(BattleSystem *, OpponentData *, W20Input *);

/* definition moved: ov12_02259BA8 */

typedef struct W20InputD48 {
    u8 command;
    u8 gender : 2;
    u8 shiny : 1;
    u8 form : 5;
    u16 species;
    u32 personality;
    u32 personality2;
    u32 value0C;
    u32 value10;
    u32 value14;
    u8 pad18[0x34];
    u32 value4C;
    u16 values50[4];
    u8 values58[4];
    u8 values5C[4];
    u8 values60[4];
    u32 values64[4];
} W20InputD48;

typedef struct W20WorkD48 {
    BattleSystem *battleSystem;
    OpponentData *opponentData;
    u8 pad08[0x0C];
    PokepicTemplate pokepicTemplate;
    u8 pad24[0x1C];
    u16 values40[4];
    u8 values48[4];
    u8 values4C[4];
    u8 values50[4];
    u32 values54[4];
    u8 pad64[0x1C];
    u8 command;
    u8 battlerId;
    u8 battlerType;
    u8 state;
    u8 facing;
    u8 height;
    u16 species;
    u32 personality;
    u8 value8C;
    u8 nature;
    u16 value8E;
    s8 value90;
    s8 value91;
    u8 shiny;
    u8 value93;
    u16 value94;
    u8 value96;
    u8 form;
    u32 value98;
} W20WorkD48;

extern void ov12_0225C18C(SysTask *, void *);
extern void ov12_0225C6C8(SysTask *, void *);

void ov12_02259D48(BattleSystem *, OpponentData *, W20InputD48 *);

/* definition moved: ov12_02259D48 */

typedef struct W20Work30 {
    BattleSystem *battleSystem;
    OpponentData *opponentData;
    Pokepic *pokepic;
    UnkBattleSystemSub17C *spriteInfo;
    u8 command;
    u8 battlerId;
    u8 state;
    u8 facing;
    s16 x;
    u16 species;
    u32 personality2;
    u32 battlerType;
    u8 pad20[4];
    u32 nature;
    u32 shiny;
    u8 form;
    u8 pad2D[3];
} W20Work30;

extern const s16 ov07_022377DC[][2];
extern const s16 ov07_022377F4[][3];
extern Pokepic *ov12_022612A4(BattleSystem *, PokepicManager *, PokepicTemplate *, s32, s32, s32, s32, s32, s32, s32, s32, PokepicAnimScript *, PokepicCallback);
extern void ov12_0225B7B8(SysTask *, void *);
extern void ov12_0225B494(SysTask *, void *);

typedef struct W20AnimBlock {
    u8 heightOffset;
    s8 yOffset;
    s8 xOffset;
    PokepicAnimScript scripts[10];
} W20AnimBlock;

void ov12_02259968(BattleSystem *, OpponentData *, W20Input *);

/* definition moved: ov12_02259968 */

void ov12_02259968(BattleSystem *battleSystem, OpponentData *opponentData, W20Input *input) {
    PokepicTemplate pokepicTemplate;
    W20AnimBlock anim;
    PokepicManager *pokepicManager;
    W20Work30 *work;
    int height;
    int tableIndex;
    int x;

    pokepicManager = BattleSystem_GetPokepicManager(battleSystem);
    BattleSystem_GetBattleType(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(W20Work30));
    work->state = 0;
    if (opponentData->battlerType & 1) {
        work->facing = 2;
        work->spriteInfo = ov12_0223A8F4(battleSystem, 1);
        ManagedSprite_SetPositionXY(work->spriteInfo->unk0,
            ov07_022377F4[opponentData->battlerType & 1][0],
            0x58);
    } else {
        work->facing = 0;
        work->spriteInfo = ov12_0223A8F4(battleSystem, 0);
        ManagedSprite_SetPositionXY(work->spriteInfo->unk0,
            ov07_022377F4[opponentData->battlerType & 1][0],
            0x88);
    }

    GetMonSpriteCharAndPlttNarcIdsEx(&pokepicTemplate, input->species, input->gender, work->facing, input->shiny != 0, input->form, input->personality);
    height = GetMonPicHeightBySpeciesGenderForm(input->species, input->gender, work->facing, input->form, input->personality);
    ((void (*)(NARC *, s8 *, u16))sub_020729D8)((NARC *)opponentData->unk1A4,
        &anim.xOffset,
        input->species);
    ((void (*)(NARC *, s8 *, u16))sub_020729FC)((NARC *)opponentData->unk1A4,
        &anim.yOffset,
        input->species);
    ((void (*)(NARC *, u8 *, u16))sub_02072A20)((NARC *)opponentData->unk1A4,
        &anim.heightOffset,
        input->species);
    NARC_ReadPokepicAnimScript((NARC *)opponentData->unk1A4, anim.scripts, input->species, opponentData->battlerType);

    tableIndex = opponentData->battlerType;
    *(Pokepic *volatile *)((u8 *)opponentData + 0x20) = ov12_022612A4(
        battleSystem, pokepicManager, &pokepicTemplate, ov07_022377F4[tableIndex][0], ov07_022377F4[tableIndex][1], ov07_022377F4[tableIndex][2], height, anim.xOffset, anim.yOffset, anim.heightOffset, opponentData->unk194, anim.scripts, NULL);
    work->pokepic = *(Pokepic *volatile *)((u8 *)opponentData + 0x20);

    if (work->facing == 2) {
        Pokepic_StartPaletteFade(work->pokepic, 8, 8, 0, 0);
    }
    if (work->facing == 2 && (BattleSystem_GetBattleSpecial(battleSystem) & 0x40)) {
        x = Pokepic_GetAttr(work->pokepic, 1);
        Pokepic_SetAttr(work->pokepic, 0x2E, 0);
        Pokepic_SetAttr(work->pokepic, 0, 0xC0);
        Pokepic_SetAttr(work->pokepic, 1, x - 0x88);
        work->x = x;
    } else {
        work->x = ov07_022377DC[opponentData->battlerType][0];
    }

    work->battleSystem = battleSystem;
    work->opponentData = opponentData;
    work->command = input->command;
    work->battlerId = opponentData->unk194;
    work->species = input->species;
    work->form = input->form;
    work->personality2 = input->personality2;
    work->battlerType = opponentData->battlerType;
    work->nature = GetNatureFromPersonality(input->personality);
    work->shiny = input->shiny;
    if (work->facing == 2 && (BattleSystem_GetBattleSpecial(battleSystem) & 0x40)) {
        SysTask_CreateOnMainQueue(ov12_0225B7B8, work, 0);
    } else {
        SysTask_CreateOnMainQueue(ov12_0225B494, work, 0);
    }
    sub_02005B58(1);
}

void ov12_02259BA8(BattleSystem *battleSystem, OpponentData *opponentData, W20Input *input) {
    W20Work9C *work;
    u32 battleType;

    BattleSystem_GetBattleType(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(W20Work9C));
    if (opponentData->battlerType & 1) {
        work->facing = 2;
    } else {
        work->facing = 0;
    }
    GetMonSpriteCharAndPlttNarcIdsEx(&work->pokepicTemplate, input->species, input->gender, work->facing, input->shiny != 0, input->form, input->personality);
    work->height = GetMonPicHeightBySpeciesGenderForm(input->species, input->gender, work->facing, input->form, input->personality);
    ((void (*)(NARC *, s8 *, u16))sub_020729D8)((NARC *)opponentData->unk1A4, &work->value90, input->species);
    ((void (*)(NARC *, s8 *, u16))sub_020729FC)((NARC *)opponentData->unk1A4, &work->value91, input->species);
    ((void (*)(NARC *, u8 *, u16))sub_02072A20)((NARC *)opponentData->unk1A4, &work->value93, input->species);
    ov12_02261284(opponentData);

    work->battleSystem = battleSystem;
    work->opponentData = opponentData;
    work->state = 0;
    work->value96 = 0;
    work->command = input->command;
    work->battlerId = opponentData->unk194;
    work->species = input->species;
    work->form = input->form;
    work->battlerType = opponentData->battlerType;
    work->personality = input->personality2;
    work->value8C = input->value0C;
    work->nature = GetNatureFromPersonality(input->personality);
    work->value8E = input->value10;
    work->shiny = input->shiny;
    work->value94 = 0;
    sub_02005B58(1);

    battleType = BattleSystem_GetBattleType(battleSystem);
    if (ov12_0223C140(battleSystem, opponentData->unk194) != 0xFF) {
        if ((battleType & 2) && !(battleType & 8) && opponentData->battlerType > 3) {
            SysTask_CreateOnMainQueue(ov12_0225B960, work, 0);
            return;
        }
        SysTask_CreateOnMainQueue(ov12_0225BE38, work, 0);
        return;
    }
    SysTask_CreateOnMainQueue(ov12_0225B960, work, 0);
}

void ov12_02259D48(BattleSystem *battleSystem, OpponentData *opponentData, W20InputD48 *input) {
    int i;
    W20WorkD48 *work;

    BattleSystem_GetBattleType(battleSystem);
    work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(W20WorkD48));
    work->state = 0;
    if (opponentData->battlerType & 1) {
        work->facing = 2;
    } else {
        work->facing = 0;
    }
    GetMonSpriteCharAndPlttNarcIdsEx(&work->pokepicTemplate, input->species, input->gender, work->facing, input->shiny != 0, input->form, input->personality);
    work->height = GetMonPicHeightBySpeciesGenderForm(input->species, input->gender, work->facing, input->form, input->personality);
    ((void (*)(NARC *, s8 *, u16))sub_020729D8)((NARC *)opponentData->unk1A4, &work->value90, input->species);
    ((void (*)(NARC *, s8 *, u16))sub_020729FC)((NARC *)opponentData->unk1A4, &work->value91, input->species);
    ((void (*)(NARC *, u8 *, u16))sub_02072A20)((NARC *)opponentData->unk1A4, &work->value93, input->species);
    ov12_02261284(opponentData);

    work->battleSystem = battleSystem;
    work->opponentData = opponentData;
    work->command = input->command;
    work->battlerId = opponentData->unk194;
    work->species = input->species;
    work->form = input->form;
    work->battlerType = opponentData->battlerType;
    work->personality = input->personality2;
    work->value8C = input->value0C;
    work->nature = GetNatureFromPersonality(input->personality);
    work->value8E = input->value10;
    work->shiny = input->shiny;
    work->value94 = input->value14;
    work->value96 = 0;
    work->value98 = input->value4C;

    for (i = 0; i < 4; i++) {
        work->values40[i] = input->values50[i];
        work->values48[i] = input->values58[i];
        work->values4C[i] = input->values5C[i];
        work->values50[i] = input->values60[i];
        work->values54[i] = input->values64[i];
    }

    i = ov12_0223C140(battleSystem, opponentData->unk194);
    if (i == 0xFF || i != work->value8C) {
        SysTask_CreateOnMainQueue(ov12_0225C18C, work, 0);
    } else {
        SysTask_CreateOnMainQueue(ov12_0225C6C8, work, 0);
    }
}

void ov12_02259F30(BattleSystem *battleSystem, OpponentData *opponentData, W20InputF30 *input) {
    int i;
    W20Work *work = Heap_Alloc(HEAP_ID_BATTLE, sizeof(W20Work));

    work->battleSystem = battleSystem;
    work->opponentData = opponentData;
    work->pokepic = opponentData->pokepic;
    work->command = input->command;
    work->battlerId = opponentData->unk194;
    work->battlerType = opponentData->battlerType;
    work->unk6B = 0;
    work->form = *(u8 *)((u8 *)input + 1);
    work->species = input->species;
    work->personality = input->personality;
    work->value74 = *(u32 *)((u8 *)input + 0x2C);

    for (i = 0; i < 4; i++) {
        work->values28[i] = input->values08[i];
        work->values30[i] = input->values10[i];
        work->values34[i] = input->values14[i];
        work->values38[i] = input->values18[i];
        work->values3C[i] = input->values1C[i];
    }

    i = ov12_0223C140(battleSystem, opponentData->unk194);
    if (i == 0xFF || i != work->value74) {
        SysTask_CreateOnMainQueue(ov12_0225C9BC, work, 0);
    } else {
        SysTask_CreateOnMainQueue(ov12_0225CC58, work, 0);
    }
}
