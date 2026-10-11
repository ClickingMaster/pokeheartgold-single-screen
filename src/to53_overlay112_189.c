#include "global.h"
#pragma require_prototypes off

extern void ManagedSprite_SetDrawFlag(void *, int);
extern void ManagedSprite_SetAnim(void *, int);
extern void ov112_021F1624(void *, int, int);
extern void ov112_021F1EFC(void *, int);
extern void ov112_021F1F3C(void *, int);
extern void ov112_021F2204(void *, int, int);
extern void ov112_021F22D0(void *, int, int, int);
extern void ov112_021F235C(void *);
extern void ov112_021F238C(void *);
extern void ov112_021F24D8(void *);
extern void ov112_021F24F8(void *);
extern int TrainerHouseSet_CheckHasData(void *);
extern int TrainerHouseTrainer_Compare(void *, void *);
extern void *Save_VarsFlags_Get(void *);
extern int Save_VarsFlags_CheckFlagInArray(void *, u16);
extern void Save_VarsFlags_SetFlagInArray(void *, u16);
extern void Save_VarsFlags_ClearFlagInArray(void *, u16);
extern const u16 sTrainerHouseDailyFlags[];

typedef struct Flag148 {
    u8 pad[0x148];
    u32 active : 1;
} Flag148;

typedef struct Flag14C {
    u8 pad[0x14c];
    u32 active : 1;
} Flag14C;


void ov112_021F27B8(void *p) {
    ov112_021F2204(p, 0, 1);
    ov112_021F2204(p, 1, 0);
    ov112_021F2348(p);
    ov112_021F1624(p, 0x2c, 0);
}
