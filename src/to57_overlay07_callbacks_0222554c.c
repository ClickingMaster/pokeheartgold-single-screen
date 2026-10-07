#include "global.h"

typedef struct Work2554C {
    u8 pad00[4];
    void *ptr04;
    u8 pad08[0x14];
    u8 slot1C[4];
    s16 x;
    s16 y;
    u8 pad24[0x0C];
    u8 movement[0x24];
    s16 unk54;
    s16 unk56;
    s16 unk58;
    s16 unk5A;
} Work2554C;

extern Work2554C *ov07_022324D8(void *, int);
extern void ov07_02231FE4(void *, Work2554C *);

extern u32 ov07_0221C4A8(void *p, int i);
extern void *ov07_0221C468(void *);
extern void *ov07_0221C470(void *);
extern void *ov07_0223197C(void *, void *);
extern void GF_AssertFail(void);
extern void ov07_02231A50(void *, void *, void *);
extern void ov07_02232020(void *, int, void *, void *);
extern int ov07_0223192C(void *, void *);
extern void ov07_02222268(void *, int, int, int, int, int);
extern void ov07_0221C410(void *, void (*)(void *, void *), void *);
extern void ov07_02225504(void *, void *);
extern int ov07_02221F80(void *, void *, int);
extern int ov07_0221FAA0(void *, void *);
extern void Pokepic_SetAttr(void *, int, int);

void ov07_0222554C(void *owner);
void ov07_02225640(void *owner);
void ov07_02225768(void *owner);

void ov07_0222554C(void *owner) {
    u32 slots[2];
    int kind;
    Work2554C *work;
    int param;

    work = ov07_022324D8(owner, 0x5C);
    ov07_02231FE4(owner, work);
    kind = ((s32)ov07_0221C4A8(owner, 0));
    param = ((s32)ov07_0221C4A8(owner, 1));
    switch (kind) {
    case 2:
        *(void **)&slots[0] = ov07_0221C468(owner);
        break;
    case 4:
        *(void **)&slots[0] = ov07_0223197C(owner, ov07_0221C468(owner));
        break;
    case 8:
        *(void **)&slots[0] = ov07_0221C470(owner);
        break;
    case 16:
        *(void **)&slots[0] = ov07_0223197C(owner, ov07_0221C470(owner));
        break;
    default:
        GF_AssertFail();
        break;
    }
    ov07_02231A50(owner, *(void **)&slots[0], &work->unk54);
    ov07_02232020(owner, kind, &work->slot1C, &slots[1]);
    work->unk58 = 0;
    work->unk5A = 0;
    work->unk58 = ov07_0223192C(work->ptr04, *(void **)&slots[0]) == 3 ? -80 : 336;
    ov07_02222268(work->movement, work->x, work->unk58, work->y, work->y, (u16)param);
    ov07_0221C410(work->ptr04, ov07_02225504, work);
}

void ov07_02225640(void *owner) {
    u32 slots[3];
    Work2554C *work;
    int kind;
    int param;

    work = ov07_022324D8(owner, 0x5C);
    ov07_02231FE4(owner, work);
    slots[1] = ((s32)ov07_0221C4A8(owner, 0));
    kind = ((s32)ov07_0221C4A8(owner, 1));
    param = ((s32)ov07_0221C4A8(owner, 2));
    switch (kind) {
    case 2:
        *(void **)&slots[0] = ov07_0221C468(owner);
        break;
    case 4:
        *(void **)&slots[0] = ov07_0223197C(owner, ov07_0221C468(owner));
        break;
    case 8:
        *(void **)&slots[0] = ov07_0221C470(owner);
        break;
    case 16:
        *(void **)&slots[0] = ov07_0223197C(owner, ov07_0221C470(owner));
        break;
    default:
        GF_AssertFail();
        break;
    }
    ov07_02231A50(owner, *(void **)&slots[0], &work->unk54);
    ov07_02232020(owner, kind, &work->slot1C, &slots[2]);
    work->unk58 = 0;
    work->unk5A = 0;
    work->unk58 = ov07_0223192C(work->ptr04, *(void **)&slots[0]) == 3 ? -80 : 336;
    if (slots[1] == 0) {
        ov07_02222268(work->movement, work->x, work->unk58, work->y, work->y, (u16)param);
    } else {
        int value = ov07_02221F80(owner, *(void **)&slots[0], 0);
        ov07_02222268(work->movement, work->unk58, value, work->y, work->y, (u16)param);
    }
    ov07_0221C410(work->ptr04, ov07_02225504, work);
}

void ov07_02225768(void *owner) {
    typedef struct Slot {
        s16 data[4];
        void *pic;
        u8 pad0C[8];
    } Slot;
    int count;
    s16 a[2];
    Slot b;
    int kind = ((s32)ov07_0221C4A8(owner, 0));
    void *selected;
    switch (kind) {
    case 2:
        selected = ov07_0221C468(owner);
        break;
    case 4:
        selected = ov07_0223197C(owner, ov07_0221C468(owner));
        break;
    case 8:
        selected = ov07_0221C470(owner);
        break;
    case 16:
        selected = ov07_0223197C(owner, ov07_0221C470(owner));
        break;
    default:
        GF_AssertFail();
        break;
    }
    int value;

    ov07_02231A50(owner, selected, a);
    ov07_02232020(owner, kind, &b, &count);
    value = (s16)ov07_0221FAA0(owner, selected);
    Pokepic_SetAttr(b.pic, 0, a[0]);
    Pokepic_SetAttr(b.pic, 1, a[1] + value);
}
