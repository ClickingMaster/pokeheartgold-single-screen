#include "global.h"

typedef struct Work {
    u8 d[0xe00];
} Work;

#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))

typedef struct Entry14 {
    u32 unk00;
    u32 unk04;
    u8 pad08;
    u8 unk09;
    u16 unk0A;
    u32 unk0C;
} Entry14;

typedef struct SortEntry {
    u32 words[8];
} SortEntry;

typedef struct FieldDataBits {
    u8 pad00[0x1d0];
    u16 flag : 1;
    u16 bit1 : 1;
    u16 value : 2;
    u16 rest : 12;
    u16 points;
} FieldDataBits;

typedef struct ParticipantPacked {
    u16 value0;
    u16 value1;
    u32 id;
    u8 pad08[8];
    u8 flag;
    u8 value2;
    u8 pad12[0x16];
} ParticipantPacked;

typedef struct SavedParticipantPacked {
    u32 value0 : 9;
    u32 value1 : 5;
    u32 value2 : 2;
    u32 flag : 1;
    u32 pad : 15;
    u32 id;
    u32 unused;
} SavedParticipantPacked;

extern void *PokeathlonCourse_GetDataCopyArea(Work *);
extern u32 ov96_021E5F24(Work *);
extern void GF_AssertFail(void);
extern u32 PokeathlonCourse_GetUnkConstant4(Work *);
extern u8 *ov96_021E8A20(void *ptr);
extern void *ov96_021E9A14(void);
extern void *PokeathlonCourse_GetSystem(Work *);
extern BOOL ov96_021E87B4(u32, void *, void *, void *);
extern void *ov96_021E9370(void *, u32);
extern void *ov96_021E94EC(void *, u32);
extern BOOL ov96_021E7D18(int, int, u32);
extern const u8 _0221A7D8[];
extern void *Save_Pokeathlon_Get(void *);
extern void *PokeathlonCourse_GetFieldData(Work *);
extern void *Save_VarsFlags_Get(void *);
extern void ov96_021E7FA8(Work *);
extern BOOL Save_VarsFlags_CheckFlagInArray(void *, u32);
extern BOOL ov96_021E8084(Work *);
extern void Save_VarsFlags_SetFlagInArray(void *, u32);
extern void PokeathlonSave_AddAthletePoints(void *, u32);
extern void *PokeathlonSave_GetRecordsLink2(void *);
extern void ov96_021E7A2C(Work *, void *);
extern void *PokeathlonSave_GetRecordsSolo(void *);
extern void *PokeathlonSave_GetUnkAEC(void *);
extern void *PokeathlonSave_dummy2(void *);
extern void ov96_021E786C(Work *, void *);
extern BOOL ov96_021E8060(void *);
extern void *PokeathlonSave_GetRecordsSolo2(void *);
extern void ov96_021E7938(Work *, void *);
extern void *PokeathlonSave_GetAgainUnkB00(void *);
extern void ov96_021E7D6C(Work *, void *);
extern BOOL ov96_021E80C4(Work *);
extern void *PokeathlonCourse_GetParticipantUnk04(Work *, u32);
extern void ov96_021E7C94(Work *, void *, SortEntry *);
extern void ov96_021E7CC8(Work *, void *, SortEntry *);
extern void *PokeathlonCourse_GetPlayerProfileFromData(Work *);
extern u32 PlayerProfile_GetTrainerID(void *);
extern const u16 *PlayerProfile_GetNamePtr(void *);
extern u8 PlayerProfile_GetLanguage(void *);

BOOL ov96_021E75E4(Work *w);
void ov96_021E7658(Work *w, u8 index);
void ov96_021E7718(Work *w);
void ov96_021E786C(Work *volatile w, void *volatile records);
void ov96_021E7938(Work *volatile w, void *volatile records);
void ov96_021E7BA8(Work *w, u16 *records, u16 *out);
void ov96_021E7C04(u32 mode, SortEntry *entry, SortEntry *entries);

void ov96_021E786C(Work *w, void *records) {
    u8 sport = U8_AT((void *)U32_AT(w, 0x1f8), 0xc);
    u8 *record = (u8 *)records + sport * 0x2c;
    u16 score = U16_AT(w, 0x8fe);
    int i;
    ParticipantPacked *src;
    SavedParticipantPacked *dst;
    u16 *srcValue;
    u16 *dstValue;

    if (score <= U16_AT(record, 6)) {
        return;
    }
    U16_AT(record, 6) = score;
    if (U16_AT(record, 6) > 999) {
        U16_AT(record, 6) = 999;
    }
    srcValue = (u16 *)w;
    i = 0;
    dstValue = (u16 *)record;
    while (i < 3) {
        *dstValue = U16_AT(srcValue, 0x8b4);
        if (*dstValue > 999) {
            *dstValue = 999;
        }
        i++;
        srcValue++;
        dstValue++;
    }
    src = PokeathlonCourse_GetParticipantUnk04(w, ov96_021E5F24(w));
    dst = (SavedParticipantPacked *)record;
    i = 0;
    dst = (SavedParticipantPacked *)((u8 *)dst + 8);
    while (i < 3) {
        dst->value0 = src->value0;
        dst->value1 = src->value1;
        dst->value2 = src->value2;
        dst->flag = src->flag;
        U32_AT(record, 0xc) = src->id;
        src++;
        dst++;
        record += 0xc;
        i++;
    }
}
