#pragma force_active on
#include "global.h"
#include "heap.h"
typedef struct SysTask SysTask;
typedef struct Ov90Params {
    u8 data[16];
} Ov90Params;
typedef struct Ov90Profile {
    u8 data[24];
} Ov90Profile;
typedef struct Ov90Main {
    u8 pad0[2];
    u16 heapId;
    u8 pad4;
    u8 x5;
    u8 pad6;
    u8 mode;
    u8 pad8[4];
    union {
        Ov90Params params;
        struct {
            u32 x0;
            u8 pad4[5];
            u8 x9;
            u8 xA;
        } p;
    } u;
    Ov90Profile profile;
    u8 x34[0x18];
    u8 x4C[0xC];
    u8 x58[0x10];
    u8 x68[0x38];
    u8 xA0[0x12C];
    u8 x1CC[0x1C];
    u8 x1E8[0x644 - 0x1E8];
    SysTask *task;
    SysTask *vtask;
    u8 pad64C[0x664 - 0x64C];
} Ov90Main;
extern const u8 ov90_0225C2F4[];
extern const u8 ov90_0225C264[];
extern u8 ov90_0225C43C[];
void ov90_0225C15C(Ov90Main *);
void GfGfx_SetBanks(const void *);
void ov90_0225A204(void *, void *);
void GF_CreateVramTransferManager(int, enum HeapID);
void ov90_02258FF0(void *, const void *, void *, u32, enum HeapID);
void ov90_022590CC(void *, int, int, int, enum HeapID);
void ov90_02258DD0(void *, int, enum HeapID);
void ov90_02259184(void *, enum HeapID);
void ov90_0225A850(Ov90Main *, enum HeapID);
int ov90_0225888C(void *, u8);
int ov90_022588A4(void *, int);
void ov90_0225938C(void *, void *, u32, int, enum HeapID);
void ov90_0225B340(void *, void *, u8, enum HeapID);
u8 ov90_0225A258(void *, void *);
void ov90_0225A980(SysTask *, void *);
void ov90_0225AE4C(SysTask *, void *);
void ov90_0225B230(SysTask *, void *);
SysTask *SysTask_CreateOnMainQueue(void (*)(SysTask *, void *), void *, u32);
SysTask *SysTask_CreateOnVWaitQueue(void (*)(SysTask *, void *), void *, u32);
void ov00_021E69A8(enum HeapID);
Ov90Main *ov90_0225A6B4(Ov90Params *params, Ov90Profile *profile, u8 mode, enum HeapID heapId);
// @fn ov90_0225A6B4
Ov90Main *ov90_0225A6B4(Ov90Params *params, Ov90Profile *profile, u8 mode, enum HeapID heapId) {
    Ov90Main *w = Heap_Alloc(heapId, sizeof(Ov90Main));
    memset(w, 0, sizeof(Ov90Main));
    w->profile = *profile;
    w->u.params = *params;
    w->mode = mode;
    ov90_0225C15C(w);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    reg_GX_DISPCNT &= ~0xE000;
    reg_GXS_DB_DISPCNT &= ~0xE000;
    GfGfx_SetBanks(ov90_0225C2F4);
    w->heapId = heapId;
    ov90_0225A204(w->x34, &w->u);
    GF_CreateVramTransferManager(0x10, heapId);
    ov90_02258FF0(w->x4C, ov90_0225C264, ov90_0225C43C, 6, heapId);
    ov90_022590CC(w->xA0, 0x20, 2, 2, heapId);
    ov90_02258DD0(w->x1CC, 2, heapId);
    ov90_02259184(w->x58, heapId);
    ov90_0225A850(w, heapId);
    ov90_0225938C(w->x68, w->x4C, w->u.p.x0, ov90_022588A4(&w->u, ov90_0225888C(&w->u, w->u.p.x9)), heapId);
    ov90_0225B340(w->x1E8, w->x4C, w->u.p.xA, heapId);
    w->x5 = ov90_0225A258(&w->u, w->x34);
    switch (w->mode) {
    case 0:
    case 1:
        w->task = SysTask_CreateOnMainQueue(ov90_0225A980, w, 0);
        break;
    case 2:
        w->task = SysTask_CreateOnMainQueue(ov90_0225AE4C, w, 0);
        break;
    default:
        GF_ASSERT(FALSE);
        w->task = SysTask_CreateOnMainQueue(ov90_0225A980, w, 0);
        break;
    }
    w->vtask = SysTask_CreateOnVWaitQueue(ov90_0225B230, w, 0);
    if (w->u.p.xA) {
        ov00_021E69A8(heapId);
    }
    return w;
}
