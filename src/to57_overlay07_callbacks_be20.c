#include "global.h"

#include "filesystem.h"
#include "heap.h"

#pragma require_prototypes off

typedef struct Ov07CallbackOwner Ov07CallbackOwner;
typedef void (*Ov07Callback)(Ov07CallbackOwner *);

struct Ov07CallbackOwner {
    u8 pad00[0x10];
    u32 state;
    u8 pad14[4];
    void **dispatcher;
    u8 pad1C[0x71];
    u8 delay;
    u16 taskCount1;
    u16 taskCount2;
    u8 pad92[0x2A];
    Ov07Callback callback;
};

extern Ov07Callback ov07_0221F8B0(void *);
extern void GF_AssertFail(void);
extern void *SysTask_CreateOnMainQueue(void *, void *, u32);
extern void SysTask_Destroy(void *);

void ov07_0221BE44(Ov07CallbackOwner *owner);

void ov07_0221BE20(Ov07CallbackOwner *owner) {
    if (owner->delay == 0) {
        owner->callback = ov07_0221BE44;
    } else {
        owner->delay--;
    }
}

void ov07_0221BE44(Ov07CallbackOwner *owner) {
    do {
        Ov07Callback callback = ov07_0221F8B0(*owner->dispatcher);
        callback(owner);
    } while (owner->delay == 0 && owner->state == 1);
}

void ov07_0221BE68(int kind, Ov07CallbackOwner *owner, void *func, void *arg, u32 priority) {
    switch (kind) {
    case 1:
        owner->taskCount1++;
        break;
    case 2:
        owner->taskCount2++;
        break;
    default:
        GF_AssertFail();
        break;
    }
    SysTask_CreateOnMainQueue(func, arg, priority);
}

void ov07_0221BEA4(int kind, Ov07CallbackOwner *owner, void *task) {
    switch (kind) {
    case 1:
        owner->taskCount1--;
        break;
    case 2:
        owner->taskCount2--;
        break;
    default:
        GF_AssertFail();
        break;
    }
    SysTask_Destroy(task);
}

typedef struct Ov07Root {
    enum HeapID heapId;
    u8 pad04[4];
    void *unk08;
    u32 ready;
    u8 pad10[8];
    void *dispatcher;
    u8 pad1C[0xA4];
    void *subWork;
    u8 padC4[8];
    void *slots4[4];
    u8 padDC[0x88];
    void *slots5[5];
    void *unk178;
    u8 pad17C[0x34];
    NARC *narcs[6];
} Ov07Root;

Ov07Root *ov07_0221BEDC(enum HeapID heapId) {
    int i;
    Ov07Root *work = Heap_Alloc(heapId, sizeof(Ov07Root));
    if (work == NULL) {
        GF_ASSERT(work != NULL);
        return NULL;
    }

    memset(work, 0, sizeof(Ov07Root));
    work->heapId = heapId;
    work->unk08 = NULL;
    work->subWork = Heap_Alloc(work->heapId, 0x11C);
    work->narcs[0] = NARC_New((NarcId)7, heapId);
    work->narcs[1] = NARC_New((NarcId)8, heapId);
    work->narcs[2] = NARC_New((NarcId)0x16, heapId);
    work->narcs[3] = NARC_New((NarcId)0x17, heapId);
    work->narcs[4] = NARC_New((NarcId)0x18, heapId);
    work->narcs[5] = NARC_New((NarcId)0x19, heapId);
    if (work->subWork == NULL) {
        GF_ASSERT(work->subWork != NULL);
        return NULL;
    }
    memset(work->subWork, 0, 0x11C);
    work->ready = 0;
    work->dispatcher = NULL;
    for (i = 0; i < 4; i++) {
        work->slots4[i] = NULL;
    }
    for (i = 0; i < 5; i++) {
        work->slots5[i] = NULL;
    }
    work->unk178 = NULL;
    work->ready = 1;
    return work;
}
