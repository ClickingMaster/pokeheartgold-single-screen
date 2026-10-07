#pragma force_active on
#include "global.h"
#include "heap.h"
#include "player_data.h"

typedef struct Ov45Callbacks {
    void (*func0)(void);
    void (*func1)(void);
    void (*func2)(void);
    void (*func3)(void);
    void (*func4)(void);
} Ov45Callbacks;

typedef struct Ov45Work {
    u32 unk0;
    void *unk4;
    u8 pad8[0xE0];
    PlayerProfile *profiles[4];
    u8 padF8[0x10];
    u8 unk108[184];
    u8 unk1C0[40];
    u8 unk1E8[20];
    u8 unk1FC[16];
    u8 unk20C[416];
    u8 unk3AC[56];
    u8 unk3E4[216];
    u8 unk4BC[76];
    u8 unk508[32];
    enum HeapID heapID;
    u8 unk52C[4];
} Ov45Work;

extern u8 ov45_02254AC4[];

void ov45_0222B2B4(void);
void ov45_0222B470(void);
void ov45_0222B530(void);
void ov45_0222B5A0(void);
void ov45_0222B75C(void);
void ov45_0222E5D4(enum HeapID heapID, u32 a1, u32 a2, Ov45Callbacks *callbacks, void *arg);
void *ov45_0222D860(enum HeapID heapID);
void ov45_0222B8A0(void *a0, u32 a1, enum HeapID heapID);
void ov45_0222BD40(void *a0);
void ov45_0222BD30(void *a0);
void ov45_0222EE20(void *a0, u32 a1, void *a2);
void ov45_0222BC3C(void *a0);
void ov45_0222C388(void *a0);
void ov45_0222C8AC(void *a0);
void ov45_0222C978(void *a0, enum HeapID heapID);
void ov45_0222CB44(void *a0, u32 a1);
void ov45_0222BCC8(void *a0, enum HeapID heapID);
Ov45Work *ov45_02229FF4(u32 a0, enum HeapID heapID);

// @fn ov45_02229FF4
Ov45Work *ov45_02229FF4(u32 a0, enum HeapID heapID) {
    Ov45Callbacks callbacks;
    int i;
    Ov45Work *work = Heap_Alloc(heapID, sizeof(Ov45Work));
    memset(work, 0, sizeof(Ov45Work));
    work->unk0 = a0;
    work->heapID = heapID;
    callbacks.func0 = ov45_0222B2B4;
    callbacks.func1 = ov45_0222B470;
    callbacks.func2 = ov45_0222B530;
    callbacks.func3 = ov45_0222B5A0;
    callbacks.func4 = ov45_0222B75C;
    ov45_0222E5D4(heapID, work->unk0, 0x94, &callbacks, work);
    work->unk4 = ov45_0222D860(heapID);
    for (i = 0; i < 4; i++) {
        work->profiles[i] = PlayerProfile_New(heapID);
    }
    ov45_0222B8A0(work->unk108, work->unk0, heapID);
    ov45_0222BD40(work->unk1C0);
    ov45_0222BD30(work->unk1E8);
    ov45_0222EE20(ov45_02254AC4, 8, work);
    ov45_0222BC3C(work->unk1FC);
    ov45_0222C388(work->unk20C);
    ov45_0222C8AC(work->unk3AC);
    ov45_0222C978(work->unk3E4, heapID);
    ov45_0222CB44(work->unk4BC, work->unk0);
    ov45_0222BCC8(work->unk508, heapID);
    return work;
}
