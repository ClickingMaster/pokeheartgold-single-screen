#ifndef POKEHEARTGOLD_APRICORN_INTERNAL_H
#define POKEHEARTGOLD_APRICORN_INTERNAL_H

#include "global.h"

#include "apricorn_box.h"
#include "heap.h"
#include "msgdata.h"
#include "player_data.h"
#include "string_util.h"

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

struct SaveApricornBox {
    u8 apricorns[7];
    u8 kurtQuantity;
    u8 kurtApricorn;
    u8 juiceServings;
    u16 queuedApricorns;
    union {
        ApricornJuice juice;
        ApricornMix juiceMix;
    };
    u32 mixStartSteps;
    u32 juiceAgingSteps;
    union {
        u32 mixWord;
        struct {
            u32 mixProgress : 30;
            u32 mixState : 2;
        };
    };
    ApricornRecord juiceRecords[3];
};

int ApricornBox_CountQueuedApricorns(SaveApricornBox *);
BOOL ApricornBox_GetJuice(SaveApricornBox *, int, ApricornJuice *);
int ApricornJuice_GetFlavorTotal(ApricornJuice *);
void ApricornJuice_RankFlavors(void *, void *);
void ApricornBox_SetMixState(SaveApricornBox *, u32);
int ApricornJuice_GetFlavorNameIndex(void *);

#endif
