#ifndef TO52_R6_W01_PRIVATE_H
#define TO52_R6_W01_PRIVATE_H

#include "global.h"

typedef int (*MATHQSortComparator)(const void *, const void *);

typedef struct Ov96RankEntryR38 {
    u32 count : 16;
    u32 group : 8;
    u32 sourceIndex : 4;
    u32 inputRank : 4;
} Ov96RankEntryR38;

typedef struct Ov96CounterDisplayR38 {
    void *sprites[4];
    u32 limit;
    u32 current;
    u32 active;
} Ov96CounterDisplayR38;

u16 ov96_021ECC4C(void *source, u8 index);
void MATH_QSort(void *base, u32 count, u32 width, MATHQSortComparator compare, void *context);
BOOL IsSEPlaying(u32 seqNo);
void PlaySE(u32 seqNo);
u32 PokeathlonUtil_GetDecimalDigit(u32 value, u32 digit);
void ManagedSprite_SetAnim(void *sprite, u32 animation);
void ManagedSprite_OffsetPositionXY(void *sprite, s16 x, s16 y);

int ov96_021ECC58(const void *a, const void *b);
void ov96_021ECC7C(void *source, Ov96RankEntryR38 *values, const u8 *ranks);
void ov96_021ECD24(Ov96CounterDisplayR38 *display);

#endif
