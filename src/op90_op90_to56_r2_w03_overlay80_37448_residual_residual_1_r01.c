#pragma force_active on

#include "global.h"

typedef struct Ov80Mon {
    /* 0x00 */ u8 _p0[0xC];
    /* 0x0C */ u32 otId;
    /* 0x10 */ u32 personality;
    /* 0x14 */ u8 _p14[0x24];
} Ov80Mon; /* size 0x38 */

typedef struct Ov80Battle {
    /* 0x000 */ u8 _p0[4];
    /* 0x004 */ u8 mode;
    /* 0x005 */ u8 slot;
    /* 0x006 */ u8 _p6;
    /* 0x007 */ u8 unk7;
    /* 0x008 */ u8 _p8[0x10];
    /* 0x018 */ u16 species[0x124];
    /* 0x260 */ u8 order[8];
    /* 0x268 */ u16 arr268[0x14];
    /* 0x290 */ Ov80Mon mons[4];
    /* 0x370 */ u8 _p370[0x385];
    /* 0x6F5 */ u8 f6F5;
    /* 0x6F6 */ u8 _p6F6[6];
    /* 0x6FC */ void *save;
    /* 0x700 */ u8 _p700[4];
    /* 0x704 */ u8 f704[4][9];
    /* 0x728 */ u8 _p728[0xD84 - 0x728];
    /* 0xD84 */ u16 fD84;
} Ov80Battle;

typedef struct TrHouseWork {
    /* 0x00 */ u32 unk0;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ u8 pad8[0x30 - 0x08];
} TrHouseWork;

typedef struct Ov80Battler {
    /* 0x00 */ u32 unk0;
    /* 0x04 */ u8 _p4[0x2C];
    /* 0x30 */ void *profile;
} Ov80Battler; /* size 0x34 */

typedef struct Ov80Setup {
    /* 0x000 */ u8 _p0[4];
    /* 0x004 */ void *party0;
    /* 0x008 */ void *party1;
    /* 0x00C */ u8 _pC[4];
    /* 0x010 */ void *party2;
    /* 0x014 */ u8 _p14[0x20];
    /* 0x034 */ Ov80Battler battlers[4];
    /* 0x104 */ u8 _p104[0x48];
    /* 0x14C */ u32 unk14C;
    /* 0x150 */ u32 unk150;
} Ov80Setup;

typedef struct Ov80Arg {
    /* 0x00 */ u8 _p0[8];
    /* 0x08 */ u32 f8;
    /* 0x0C */ u32 fC;
    /* 0x10 */ u8 _p10[8];
    /* 0x18 */ u32 f18;
    /* 0x1C */ u32 f1C;
} Ov80Arg;

extern const u16 ov80_0223C5A8[];
extern const u16 ov80_0223C5B4[];
extern const u8 ov80_0223C5B8[];
extern const u16 ov80_0223C5E0[];
extern const u16 ov80_0223C608[];
extern const u16 ov80_0223C698[];
extern const u16 ov80_0223C738[];
extern const u16 ov80_0223C990[];
extern const u16 ov80_0223CD4A[];
extern const u8 ov80_0223D4C0[];

u16 LCRandom(void);
void *SaveArray_Party_Get(void *saveData);
void *Party_GetMonByIndex(void *party, int slot);
u32 GetMonData(void *mon, int field, void *dest);
void HealParty(void *party);
void *AllocMonZeroed(u32 heapId);
void CopyPokemonToPokemon(void *src, void *dst);
void UpdateMonAbility(void *mon);
void Heap_Free(void *ptr);
void Party_InitWithMaxSize(void *party, int maxSize);
Ov80Setup *BattleSetup_New(u32 heapId, u32 flags);
void BattleSetup_AddMonToParty(Ov80Setup *setup, void *mon, int side);
void BattleSetup_SetAllySideBattlersToPlayer(Ov80Setup *setup);
void PlayerProfile_Copy(void *dst, void *src);
u8 CalcShininessByOtIdAndPersonality(u32 otId, u32 personality);
fx32 FX_Sqrt(fx32 x);
u32 HallTypeRanks_Get(u32 a, void *b);
void *CommPlayerInfo_GetProfile(int a);
int Comm_GetLocalNetId(void);
void sub_02051D18(Ov80Setup *setup, int a, u32 b, u32 c, u32 d, u32 e);
void *FrontierTrainer_Load(TrHouseWork *dst, u32 species, u32 heapId, u32 d);
void FrontierMon_ToPokemon(Ov80Mon *a, void *b, u8 c);
void FrontierTrainer_SetBattleTrainer(Ov80Setup *a, TrHouseWork *b, int c, int d, u32 e);
void FrontierMon_GenerateWithRandomOtId(Ov80Mon *a, u16 b, int c, u8 d, int e, u32 f, u32 g);

void ov80_022372D8(u32 a, int n, u32 c, u32 d, u16 *out);
void ov80_02237334(int a, int b, int c, u32 d, u8 e, u16 *f);
void ov80_02237448(int a, u8 b, int c, u8 d, u16 e, u16 *f, int g);

u8 ov80_02237820(Ov80Battle *p, u32 i);
u32 ov80_02237850(u32 a);
int ov80_0223787C(int a);
int ov80_02237888(int a);
void ov80_02237894(Ov80Mon *a, u32 b, u32 c, u16 *d, int e, u32 f, u32 g);
u16 ov80_022378F8(Ov80Battle *p, u32 unused);
u8 ov80_02237920(u32 a);
BOOL ov80_0223792C(int a);
u8 ov80_0223793C(Ov80Battle *p);
u8 ov80_0223796C(u32 a);
int ov80_02237980(Ov80Battle *p, u32 idx, int c);
u32 ov80_022379C0(u32 a);
u16 ov80_022379C8(Ov80Battle *p);
u32 ov80_02237A40(u32 a);


Ov80Setup *ov80_022375D0(Ov80Battle *p, Ov80Arg *q);
// @fn ov80_022375D0
Ov80Setup *ov80_022375D0(Ov80Battle *p, Ov80Arg *q) {
    u32 val;
    Ov80Setup *setup;
    void *mon;
    void *party;
    u8 slot2;
    int n1;
    int ai;
    TrHouseWork trainer;
    int i;
    s32 n2;
    slot2 = p->slot * 2;
    n1 = ov80_0223787C(p->mode);
    n2 = ov80_02237888(p->mode);
    party = SaveArray_Party_Get(p->save);

    HealParty(party);
    setup = BattleSetup_New(11, ov80_02237850(p->mode));
    sub_02051D18(setup, 0, q->f8, q->f18, q->fC, q->f1C);
    setup->unk14C = 22;
    setup->unk150 = 22;
    Party_InitWithMaxSize(setup->party0, n1);
    mon = AllocMonZeroed(11);
    for (i = 0; i < n1; i++) {
        CopyPokemonToPokemon(Party_GetMonByIndex(party, p->order[i]), mon);
        BattleSetup_AddMonToParty(setup, mon, 0);
    }
    Heap_Free(mon);
    BattleSetup_SetAllySideBattlersToPlayer(setup);
    Heap_Free(FrontierTrainer_Load(&trainer, p->species[slot2], 11, 0xCC));
    FrontierTrainer_SetBattleTrainer(setup, &trainer, n2, 1, 11);
    Party_InitWithMaxSize(setup->party1, n2);
    val = HallTypeRanks_Get(p->f6F5, p->f704[p->mode]);
    if (p->mode == 2) {
        val = 9;
    }
    ai = ov80_02237980(p, slot2, val);
    for (i = 0; i < 4; i++) {
        setup->battlers[i].unk0 = ai;
    }
    ov80_02237894((Ov80Mon *)((u8 *)p->mons + slot2 * sizeof(Ov80Mon)), val, p->species[slot2], &p->arr268[slot2], n2, 11, 0xCE);
    mon = AllocMonZeroed(11);
    for (i = 0; i < n2; i++) {
        while (ov80_02237820(p, slot2) != 0) {
        }
        FrontierMon_ToPokemon(&p->mons[slot2], mon, ov80_022378F8(p, val));
        UpdateMonAbility(mon);
        BattleSetup_AddMonToParty(setup, mon, 1);
    }
    Heap_Free(mon);
    switch (p->mode) {
    case 2:
    case 3:
        BattleSetup_SetAllySideBattlersToPlayer(setup);
        PlayerProfile_Copy(CommPlayerInfo_GetProfile(1 - Comm_GetLocalNetId()), setup->battlers[3].profile);
        Heap_Free(FrontierTrainer_Load(&trainer, p->species[slot2 + 1], 11, 0xCC));
        FrontierTrainer_SetBattleTrainer(setup, &trainer, n2, 3, 11);
        Party_InitWithMaxSize(setup->party2, n2);
        mon = AllocMonZeroed(11);
        while (ov80_02237820(p, slot2) != 0) {
        }
        FrontierMon_ToPokemon((Ov80Mon *)((u8 *)p + 0x290 + slot2 * sizeof(Ov80Mon)), mon, ov80_022378F8(p, val));
        UpdateMonAbility(mon);
        BattleSetup_AddMonToParty(setup, mon, 3);
        Heap_Free(mon);
        break;
    }
    return setup;
}
