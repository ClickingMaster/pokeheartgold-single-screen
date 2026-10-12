#include "to50_network_state_init_internal.h"

void sub_02034B0C(int value, int mode) {
    if (_021D4134.work == NULL) {
        void *allocation;
        u8 *work;
        u8 *raw;
        u32 remainder;
        u32 adjustment;

        allocation = Heap_Alloc(HEAP_ID_15, 0xD98);
        _021D4134.work = allocation;
        MI_CpuFill8(allocation, 0, 0xD98);

        allocation = Heap_Alloc(HEAP_ID_15, Wireless_GetWorkSize());
        *(void **)(_021D4134.work + 0xD64) = allocation;
        MI_CpuFill8(*(void **)(_021D4134.work + 0xD64), 0, Wireless_GetWorkSize());

        allocation = Heap_Alloc(HEAP_ID_15, LinkBattleRuleset_sizeof());
        *(void **)(_021D4134.work + 0xD7C) = allocation;
        MI_CpuFill8(*(void **)(_021D4134.work + 0xD7C), 0, LinkBattleRuleset_sizeof());

        allocation = Heap_Alloc(HEAP_ID_15, 0x90);
        *(void **)(_021D4134.work + 0xD84) = allocation;
        work = _021D4134.work;
        raw = *(u8 **)(work + 0xD84);
        remainder = (u32)raw & 0x1F;
        adjustment = 0x20 - remainder;
        *(u8 **)(work + 0xD88) = (u8 *)(adjustment + (u32)raw);
        *(u32 *)(_021D4134.work + 0xD80) = 0x333;
        *(int *)(_021D4134.work + 0xD78) = value;
        MailMsg_Init((MailMessage *)(_021D4134.work + 0xD68));
        sub_02034DF0(mode);
        _021D4134.localId = WM_GetNextTgid();
    }
}
