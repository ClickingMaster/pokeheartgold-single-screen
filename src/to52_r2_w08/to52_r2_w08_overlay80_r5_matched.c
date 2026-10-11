#include "private.h"

void ov80_02232F08(void *work) {
    void *obj;
    int value;
    int i = U8(work, 0x11);
    obj = ov80_02237B58(U8(work, 0x10), 1);
    value = ov80_02237D8C(U8(work, 0x10));
    FrontierTrainer_GenerateMons(obj,
        ((W08Work *)work)->values[i],
        ((W08Work *)work)->values[i + 7],
        (u8 *)work + 0x26c,
        (u8 *)work + 0x288,
        (u8 *)work + 0x274,
        (u8 *)work + 0x278,
        (u8)value);
    ov80_02237E30(work);
}

void ov80_02232F60(void *ctx) {
    u8 *p = ctx;
    s32 start = Comm_GetLocalNetId() == 0 ? 0 : 2;
    s32 count = ov80_02237B24(p[0x10], 0);
    s32 end;
    s32 i;
    Party_GetCount(*(void **)(p + 0x28));
    i = start;
    end = count + start;
    for (; i < end; i++) {
        void *mon = Party_GetMonByIndex(*(void **)(p + 0x28), i);
        u8 *dst = p + (i - start) * 8;
        *(u16 *)(dst + 0x394) = GetMonData(mon, 0x3A, NULL);
        *(u16 *)(dst + 0x396) = GetMonData(mon, 0x3B, NULL);
        *(u16 *)(dst + 0x398) = GetMonData(mon, 0x3C, NULL);
        *(u16 *)(dst + 0x39A) = GetMonData(mon, 0x3D, NULL);
    }
    ov80_02237D9C(*(void **)(p + 0x28));
    for (i = 0; i < 4; i++) {
        p[0x370 + i] = 0;
        p[0x374 + i] = 0;
        p[0x378 + i] = 0;
        p[0x37C + i] = 0;
    }
}
