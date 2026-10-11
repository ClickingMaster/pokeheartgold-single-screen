#pragma force_active on
#ifndef PRIVATE_OVERLAY_80_02236450_H
#define PRIVATE_OVERLAY_80_02236450_H
#include "global.h"

/* 0x10-byte NARC record read by FrontierData_ReadMember */
typedef struct FrontierMonSrc {
    /* 0x00 */ u16 species;
    /* 0x02 */ u16 moves[4];
    /* 0x0a */ u8 evFlags;
    /* 0x0b */ u8 nature;
    /* 0x0c */ u16 item;
    /* 0x0e */ u16 unk_0e;
} FrontierMonSrc;

/* 0x38-byte built mon entry */
typedef struct FrontierMon {
    /* 0x00 */ u16 species : 11;
    /* 0x00 */ u16 form : 5;
    /* 0x02 */ u16 item;
    /* 0x04 */ u16 moves[4];
    /* 0x0c */ u32 unk_0c;
    /* 0x10 */ u32 personality;
    /* 0x14 */ u32 iv0 : 5;
    /* 0x14 */ u32 iv1 : 5;
    /* 0x14 */ u32 iv2 : 5;
    /* 0x14 */ u32 iv3 : 5;
    /* 0x14 */ u32 iv4 : 5;
    /* 0x14 */ u32 iv5 : 5;
    /* 0x14 */ u32 pad_14 : 2;
    /* 0x18 */ u8 evs[6];
    /* 0x1e */ u8 unk_1e;
    /* 0x1f */ u8 language;
    /* 0x20 */ u8 ability;
    /* 0x21 */ u8 unk_21;
    /* 0x22 */ u16 name[11];
} FrontierMon;

typedef struct Ov80Flags {
    u8 unk_0 : 3;
    u8 unk_3 : 1;
    u8 unk_4 : 1;
    u8 unk_5 : 3;
} Ov80Flags;

typedef struct Ov80C050 {
    u16 unk_00;
    u8 unk_02[4];
} Ov80C050;

extern const u8 ov80_0223C048[];
extern const u8 ov80_0223C050[];
extern const u8 ov80_0223C07C[];
extern const u8 ov80_0223C0AC[];
extern const u8 gGameLanguage;

void GF_AssertFail(void);
void *Heap_Free(void *);
void MI_CpuFill8(void *, u8, u32);
void MI_CpuCopy8(const void *, void *, u32);
void *CommPlayerInfo_GetProfile(int);
int PlayerProfile_GetVersion(void *);
void *Save_BattleRoomDownload_Get(void *);
int BattleRoomDownload_HasTrainers(void);
void BattleRoomDownload_GetTrainer(void *, void *, int);
u16 FrontierFieldSystem_0204B510(void *);
int GetNatureFromPersonality(u32);
int CalcShininessByOtIdAndPersonality(u32, u32);
u8 MaskOfFlagNo(int);
int GetMonBaseStat(int, int);
void GetSpeciesNameIntoArray(int, int, u16 *);
u8 GetFrontierTrainerIVs(u32);
void *AllocMonZeroed(int);
void *BattleSetup_New(int, int);
void BattleSetup_AddMonToParty(void *, void *, int);
void BattleSetup_SetAllySideBattlersToPlayer(void *);
void *SaveArray_Party_Get(void *);
void *Party_GetMonByIndex(void *, int);
void Party_InitWithMaxSize(void *, int);
BOOL Party_AddMon(void *, void *);
void CopyPokemonToPokemon(void *, void *);
u32 GetMonData(void *, int, void *);
void SetMonData(void *, int, void *);
int GetMonExpBySpeciesAndLevel(int, int);
void CalcMonLevelAndStats(void *);
void sub_02051D18(void *, int, void *, int, int, int);
int _s32_div_f(int, int);

void FrontierData_ReadMember(void *, int, int);
void *FrontierTrainer_Load(void *, int, int, int);
void FrontierMon_ToPokemon(void *, void *, int);
void FrontierTrainer_SetBattleTrainer(void *, u8 *, int, int, int);

u32 ov80_02236450(void *, void *, int, u32, u32, u32, u32, int);

void ov80_02236698(void *, void *, int);
void ov80_022366D4(u8 *, int);
u32 ov80_02236734(void *, FrontierMon *, u16, u32, u32, u8, u8, u32, int);
void *ov80_0223690C(void *, void *);
void ov80_02236A34(void *, u8 *, int, int, int);
u32 ov80_02236A88(u32);
void ov80_02236ABC(void *, u16);
BOOL ov80_02236AD8(void *, const u16 *);
u32 ov80_02236AF0(u32);
u32 ov80_02236B04(u32);
u32 ov80_02236B18(u32);
BOOL ov80_02236B30(u32);


/* Generated integration prototypes. */
u32 ov80_02236450(void *work, void *record, int trainerId, u32 index, u32 a4, u32 a5, u32 a6, int heapId);
void ov80_02236698(void *a0, void *dst, int index);
void ov80_022366D4(u8 *dst, int index);
u32 ov80_02236734(void *work, FrontierMon *dst, u16 trainerMonId, u32 otId, u32 personality, u8 ivs, u8 index, u32 useFixedItem, int heapId);
void *ov80_0223690C(void *work, void *a1);
void ov80_02236A34(void *bs, u8 *mons, int count, int idx, int heapId);
u32 ov80_02236A88(u32 mode);
void ov80_02236ABC(void *work, u16 value);
BOOL ov80_02236AD8(void *work, const u16 *arg);
u32 ov80_02236AF0(u32 mode);
u32 ov80_02236B04(u32 mode);
u32 ov80_02236B18(u32 mode);
BOOL ov80_02236B30(u32 mode);
#endif



typedef struct FrontierSelectionOut {
    u32 otId;
    u16 selected[2];
    u32 personalities[2];
} FrontierSelectionOut;

typedef struct FrontierTrainerMons {
    u16 unk0;
    u16 count;
    u16 ids[1];
} FrontierTrainerMons;

u32 ov80_022364A4(u8 *work, FrontierTrainerMons *raw, u16 trainerId, FrontierMon *mons, u8 count, u16 *speciesExclude, u16 *itemExclude, FrontierSelectionOut *out, int heapId);
// @fn ov80_022364A4
u32 ov80_022364A4(u8 *work, FrontierTrainerMons *raw, u16 trainerId, FrontierMon *mons, u8 count, u16 *speciesExclude, u16 *itemExclude, FrontierSelectionOut *out, int heapId) {
    int i;
    int j;
    int n;
    u8 ivs;
    u32 otId;
    u32 id;
    int tries;
    u32 fallback;
    u32 *sel;
    u32 *cursor;
    u32 selected[4];
    u32 personalities[4];
    FrontierMonSrc previous;
    FrontierMonSrc candidate;

    fallback = FALSE;
    GF_ASSERT(count <= 4);
    n = count;
    i = tries = 0;
    if (n != 0) {
    cursor = selected;
    do {
        id = raw->ids[(u8)(FrontierFieldSystem_0204B510(work) % raw->count)];
        FrontierData_ReadMember(&candidate, id, ov80_02236AF0(work[0xf]));
        for (j = 0; j < i; j++) {
            FrontierData_ReadMember(&previous, selected[j], ov80_02236AF0(work[0xf]));
            if (previous.species == candidate.species) {
                break;
            }
        }
        if (j != i) {
            continue;
        }
        if (speciesExclude != NULL) {
            for (j = 0; j < n; j++) {
                if (candidate.species == speciesExclude[j]) {
                    break;
                }
            }
            if (j != n) {
                continue;
            }
        }
        if (tries < 50) {
            for (j = 0; j < i; j++) {
                FrontierData_ReadMember(&previous, selected[j], ov80_02236AF0(work[0xf]));
                if (previous.item != 0 && previous.item == candidate.item) {
                    break;
                }
            }
            if (j != i) {
                tries++;
                continue;
            }
            if (itemExclude != NULL) {
                for (j = 0; j < n; j++) {
                    if (candidate.item == itemExclude[j] && itemExclude[j] != 0) {
                        break;
                    }
                }
                if (j != n) {
                    tries++;
                    continue;
                }
            }
        }
        i++;
        *cursor++ = id;
    } while (i != n);
    }
    ivs = GetFrontierTrainerIVs(trainerId);
    otId = FrontierFieldSystem_0204B510(work) | (FrontierFieldSystem_0204B510(work) << 16);
    if (tries >= 50) {
        fallback = TRUE;
    }
    j = 0;
    if (i > 0) {
        sel = selected;
        do {
            personalities[j] = ov80_02236734(work, &mons[j], *sel, otId, 0, ivs, j, fallback, heapId);
            sel++;
            j++;
        } while (j < i);
    }
    if (out == NULL) {
        return fallback;
    }
    out->otId = otId;
    for (j = 0; j < 2; j++) {
        out->selected[j] = selected[j];
        out->personalities[j] = personalities[j];
    }
    return fallback;
}
