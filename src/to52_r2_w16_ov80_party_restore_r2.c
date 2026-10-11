#include "global.h"

typedef struct {
    u8 raw[0x400];
} Work;
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

s32 Party_GetCount(void *);
void *Party_GetMonByIndex(void *, s32);
u32 GetMonData(void *, u32, void *);
void SetMonData(void *, u32, const void *);
u32 ov80_02237ED8(void);
void ov80_02237ADC(u32, u32, void *, u32);
void *ov80_02237B58(u32, u32);
u32 ov80_02237D8C(u32);
u32 ov80_02237D88(Work *);
void FrontierTrainer_GenerateMons(void *, u16, u16, void *, void *, void *, void *, u8);
u32 CastleChallenge_GetField(void *, u32, u8, u32, u32);
void FrontierMon_GenerateList(void *, void *, void *, u32, u32, u32, u32, u32);
void *AllocMonZeroed(u32);
void FrontierMon_ToPokemon(void *, void *, u32);
void ov80_02237E18(void *, void *, void *);
void Heap_Free(void *);
void ov80_02232838(Work *);
void ov80_022329B4(Work *);
void ov80_022329B4(Work *p) {
    int count = (u8)Party_GetCount(PTR(p, 0x28));
    int i;
    u8 v8;
    u16 v16;
    u32 v32;
    for (i = 0; i < count; i++) {
        void *mon = Party_GetMonByIndex(PTR(p, 0x28), i);
        v16 = CastleChallenge_GetField(PTR(p, 8), 2, (u8)i, 0, 0);
        SetMonData(mon, 0xa3, &v16);
        v8 = CastleChallenge_GetField(PTR(p, 8), 3, (u8)i, 0, 0);
        SetMonData(mon, 0x3a, &v8);
        v8 = CastleChallenge_GetField(PTR(p, 8), 3, (u8)i, 1, 0);
        SetMonData(mon, 0x3b, &v8);
        v8 = CastleChallenge_GetField(PTR(p, 8), 3, (u8)i, 2, 0);
        SetMonData(mon, 0x3c, &v8);
        v8 = CastleChallenge_GetField(PTR(p, 8), 3, (u8)i, 3, 0);
        SetMonData(mon, 0x3d, &v8);
        v32 = CastleChallenge_GetField(PTR(p, 8), 4, (u8)i, 0, 0);
        SetMonData(mon, 0xa0, &v32);
        v16 = CastleChallenge_GetField(PTR(p, 8), 5, (u8)i, 0, 0);
        SetMonData(mon, 6, &v16);
    }
}
