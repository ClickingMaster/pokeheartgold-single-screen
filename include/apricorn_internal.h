#ifndef POKEHEARTGOLD_APRICORN_INTERNAL_H
#define POKEHEARTGOLD_APRICORN_INTERNAL_H

#include "global.h"

#include "heap.h"
#include "msgdata.h"
#include "player_data.h"
#include "string_util.h"
#include "unk_02031B0C.h"

struct SaveApricornBox {
    u8 apricorns[7];
    u8 kurtQuantity;
    u8 kurtApricorn;
    u8 unk_09;
    u16 unk_0A;
    u8 unk_0C[8];
    u32 unk_14;
    u32 unk_18;
    union {
        u32 unk_1C;
        struct {
            u32 mixProgress : 30;
            u32 mixState : 2;
        };
    };
    u8 unk_20[3][0x20];
};

typedef struct ApricornRecord {
    u32 trainerId;
    u8 gender;
    u8 language;
    u8 version;
    u8 padding;
    u16 name[8];
    UnkStruct_02031CEC data;
} ApricornRecord;

typedef struct ApricornMix {
    u8 padding[3];
    u8 values[5];
} ApricornMix;

int ApricornBox_CountQueuedApricorns(SaveApricornBox *);
int ApricornBox_TakeApricorn(SaveApricornBox *, int, u32);
void ClearApricornJuice(void *);
BOOL ApricornBox_GetJuice(SaveApricornBox *, int, UnkStruct_02031CEC *);
BOOL ApricornJuice_IsValid(UnkStruct_02031CEC *);
int ApricornJuice_GetFlavorTotal(UnkStruct_02031CEC *);
void ApricornJuice_RankFlavors(void *, void *);
void ApricornBox_SetMixState(SaveApricornBox *, u32);
void sub_02032340(SaveApricornBox *, u8);
void ApricornBox_MixQueuedApricorns(SaveApricornBox *);
int sub_02032004(void *);
u8 GetJuiceFlavorLevel(int, u8);

#endif
