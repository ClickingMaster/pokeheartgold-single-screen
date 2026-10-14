#ifndef POKEHEARTGOLD_APRICORN_INTERNAL_H
#define POKEHEARTGOLD_APRICORN_INTERNAL_H

#include "global.h"

#include "heap.h"
#include "msgdata.h"
#include "player_data.h"
#include "string_util.h"
#include "apricorn_box.h"

struct SaveApricornBox {
    u8 apricorns[7];
    u8 kurtQuantity;
    u8 kurtApricorn;
    u8 juiceServings;
    u16 queuedApricorns;
    u8 juice[8];
    u32 mixStartSteps;
    u32 juiceAgingSteps;
    union {
        u32 mixWord;
        struct {
            u32 mixProgress : 30;
            u32 mixState : 2;
        };
    };
    u8 juiceRecords[3][0x20];
};

typedef struct ApricornRecord {
    u32 trainerId;
    u8 gender;
    u8 language;
    u8 version;
    u8 padding;
    u16 name[8];
    ApricornJuice data;
} ApricornRecord;

typedef struct ApricornMix {
    u8 padding[3];
    u8 values[5];
} ApricornMix;

int ApricornBox_CountQueuedApricorns(SaveApricornBox *);
int ApricornBox_TakeApricorn(SaveApricornBox *, int, u32);
void ClearApricornJuice(void *);
BOOL ApricornBox_GetJuice(SaveApricornBox *, int, ApricornJuice *);
BOOL ApricornJuice_IsValid(ApricornJuice *);
int ApricornJuice_GetFlavorTotal(ApricornJuice *);
void ApricornJuice_RankFlavors(void *, void *);
void ApricornBox_SetMixState(SaveApricornBox *, u32);
void ApricornBox_AddJuiceMildness(SaveApricornBox *, u8);
void ApricornBox_MixQueuedApricorns(SaveApricornBox *);
int ApricornJuice_GetFlavorNameIndex(void *);
u8 GetJuiceFlavorLevel(int, u8);

#endif
