#include "to57_r3_06_overlay07_ed_private.h"

#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))

void ov07_0221ED14(void *task, Ov07EdWork *work) {
    if (!ov07_02234BC0[work->callback](task, work)) {
        ((u8 *)*(void **)((u8 *)work + 0x48))[0x17C] = 0;
        Heap_Free(work);
        SysTask_Destroy(task);
    }
}

void ov07_0221ED44(Ov07EdOwner *owner) {
    Ov07EdWork *work = ov07_0221DF1C(owner);
    u32 word;

    work->kind = ((s32)ov07_0221C4A8(owner, 4));
    U32_AT(owner, 0x18) += 4;
    work->value = *(u32 *)U32_AT(owner, 0x18);
    U32_AT(owner, 0x18) += 4;
    word = *(u32 *)U32_AT(owner, 0x18);
    U32_AT(owner, 0x18) += 4;
    work->callback = (u16)word;
    work->param = (word & 0xFFFF0000) >> 16;
    SysTask_CreateOnMainQueue((void (*)(void *, void *))ov07_0221ED14, work, 0x44C);
}

void ov07_0221ED94(void *owner) {
    Ov07EdWork *work = ov07_0221DF1C(owner);
    u32 word;
    s16 kind;
    s16 amount;

    work->kind = ((s32)ov07_0221C4A8(owner, 4));
    U32_AT(owner, 0x18) += 4;
    work->value = *(u32 *)U32_AT(owner, 0x18);
    U32_AT(owner, 0x18) += 4;
    word = *(u32 *)U32_AT(owner, 0x18);
    U32_AT(owner, 0x18) += 4;
    work->callback = (u16)word;
    work->param = (word & 0xFFFF0000) >> 16;
    word = *(u32 *)U32_AT(owner, 0x18);
    U32_AT(owner, 0x18) += 4;
    kind = (s16)word;
    amount = (s16)((word & 0xFFFF0000) >> 16);
    switch (kind) {
    case 1:
        work->x += amount;
        break;
    case 2:
        work->x -= amount;
        break;
    case 3:
        work->y += amount;
        break;
    case 4:
        work->y -= amount;
        break;
    case 5:
        work->x += amount;
        work->y += amount;
        break;
    case 6:
        work->x -= amount;
        work->y -= amount;
        break;
    default:
        if (ov07_0223789C != NULL) {
            GF_AssertFail();
        }
        break;
    }
    SysTask_CreateOnMainQueue((void (*)(void *, void *))ov07_0221ED14, work, 0x44C);
}

void ov07_0221EE74(Ov07EdOwner *owner) {
    u32 kind;
    s16 value;
    s16 *transform;

    U32_AT(owner, 0x18) += 4;
    kind = *(u32 *)U32_AT(owner, 0x18);
    U32_AT(owner, 0x18) += 4;
    value = *(u32 *)U32_AT(owner, 0x18);
    U32_AT(owner, 0x18) += 4;
    transform = ov07_0221DD8C(&owner->transform, 1);
    switch (kind) {
    case 0:
        transform[4] = value;
        break;
    case 1:
        transform[5] = value;
        break;
    case 2:
        transform[2] = value;
        break;
    case 3:
        transform[2] = value;
        break;
    }
}

void ov07_0221EEC0(void *owner) {
    Ov07EdWork *work = ov07_0221DF1C(owner);
    u32 word;

    work->kind = ((s32)ov07_0221C4A8(owner, 4));
    U32_AT(owner, 0x18) += 4;
    work->value = *(u32 *)U32_AT(owner, 0x18);
    U32_AT(owner, 0x18) += 4;
    word = *(u32 *)U32_AT(owner, 0x18);
    U32_AT(owner, 0x18) += 4;
    work->callback = (u16)word + 3;
    work->param = (word & 0xFFFF0000) >> 16;
    SysTask_CreateOnMainQueue((void (*)(void *, void *))ov07_0221ED14, work, 0x44C);
}
