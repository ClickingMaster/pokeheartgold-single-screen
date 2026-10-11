#ifndef SOL37_R19_OV96_R92_PRIVATE_H
#define SOL37_R19_OV96_R92_PRIVATE_H

#include "global.h"

#pragma require_prototypes off

#define U8_AT(p, o) (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct R92Bits { u32 value : 8; } R92Bits;

typedef struct R92Rect {
    void *unk0;
    s16 x;
    s16 y;
    s32 unk8;
    s32 unkC;
    s16 value;
    s16 unk12;
} R92Rect;

typedef struct R92Pair {
    u16 x;
    u16 y;
} R92Pair;

typedef struct R92Node {
    struct R92Node *prev;
    struct R92Node *next;
    R92Rect *rect;
} R92Node;

void *PokeathlonCourse_GetHeapAllocPtr4(void *);
u32 PokeathlonCourse_GetDataCopyArea(void *);
u32 PokeathlonCourse_GetParticipantCount(void *);
void PokeathlonCourse_SetField5E0_AtIndex(void *, u8, u16);
int ov96_021E5F24(void *);
void PokeathlonCourse_SetStateField07(void *, u8);
void *SpriteManager_GetSpriteList(void *);
u16 PokeathlonBlockSmashBlocks_GetScore(void *);
u8 PokeathlonPennantCaptureCpu_SortCells(R92Rect **, R92Rect **);
extern const R92Pair ov96_0221CC00;

#endif
