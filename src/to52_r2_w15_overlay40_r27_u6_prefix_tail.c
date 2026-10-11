#include "to52_overlay40_r27_u6_prefix_private.h"

int ov40_02231C78(void *p) {
    void *w;
    void *e;
    int i;
    u8 *q;
    int t;
    int v;
    u8 *r;

    switch (WORD(p, 8)) {
    case 0:
        w = Heap_Alloc(0x6D, 0x10);
        MI_CpuFill8(w, 0, 0x10);
        PTR(p, 0x860) = w;
        i = 0;
        q = (u8 *)p;
        do {
            e = Heap_Alloc(0x6D, 0x34);
            memset(e, 0, 0x34);
            VsRecorder_GetSpritePos(PTR(q, 0x534), (s16 *)e, (s16 *)((u8 *)e + 2));
            PTR(e, 0x20) = PTR(q, 0x534);
            PTR(e, 0x24) = PTR(q, 0x548);
            PTR(e, 0x2C) = (u8 *)w + 4;
            PTR(e, 0x30) = (u8 *)w + 0xC;
            WORD(e, 0x28) = 0;
            HALF(e, 4) = SHALF(e, 0);
            if (S32(p, 0x6D8) - 1 == i) {
                PTR(e, 0x28) = PTR(p, 0x6F0);
                HALF(e, 4) = SHALF(e, 0);
                v = 0xD9;
            } else {
                HALF(e, 4) = SHALF(e, 0) + 4;
                t = S32(p, 0x6D8) - i;
                v = (5 - t) * 0x24 + 0x1D + t * 16;
            }
            HALF(e, 6) = v;
            BYTE(e, 0x1C) = 4;
            SysTask_CreateOnMainQueue(ov40_022318C8, e, 2 << 12);
            i++;
            q += 0x28;
        } while (i <= S32(p, 0x6D8) - 1);
        WORD(p, 8)
        ++;
        break;
    case 1:
        w = PTR(p, 0x860);
        if (WORD(w, 0xC) == 1) {
            i = 0;
            if (S32(p, 0x6E0) > 0) {
                q = (u8 *)p;
                do {
                    e = Heap_Alloc(0x6D, 0x34);
                    memset(e, 0, 0x34);
                    VsRecorder_GetSpritePos(PTR(q, 0x5FC), (s16 *)e, (s16 *)((u8 *)e + 2));
                    PTR(e, 0x20) = PTR(q, 0x5FC);
                    PTR(e, 0x24) = PTR(q, 0x610);
                    PTR(e, 0x2C) = (u8 *)w + 4;
                    PTR(e, 0x30) = (u8 *)w + 0xC;
                    WORD(e, 0x28) = 0;
                    HALF(e, 4) = SHALF(e, 0);
                    HALF(e, 6) = (5 - S32(p, 0x6E0)) * 16 + 0xCD;
                    if (SHALF(e, 6) >= 0xDD) {
                        HALF(e, 6) = 0xDD;
                    }
                    BYTE(e, 0x1C) = 8;
                    SysTask_CreateOnMainQueue(ov40_022318C8, e, 2 << 12);
                    i++;
                    q += 0x28;
                } while (i < S32(p, 0x6E0));
            }
            WORD(w, 0) = 0;
            WORD(p, 8)
            ++;
            --S32(p, 0x6D8);
            i = 0;
            if (S32(p, 0x6D8) > 0) {
                r = (u8 *)p;
                do {
                    if (i == S32(p, 0x6D8) - 1) {
                        ManagedSprite_SetPaletteOverrideOffset(PTR(r, 0x534), 1);
                    } else {
                        ManagedSprite_SetPaletteOverrideOffset(PTR(r, 0x534), 2);
                    }
                    i++;
                    r += 0x28;
                } while (i < S32(p, 0x6D8));
            }
        }
        break;
    case 2:
        w = PTR(p, 0x860);
        if (WORD(w, 0) != 0x10) {
            WORD(w, 0) = WORD(w, 0) + 2;
            PaletteData_BlendPalettes(PTR(p, 0x28), 2, 0xC, WORD(w, 0), WORD(p, 0x58));
        }
        if (WORD(w, 4) == 0) {
            WORD(p, 8)
            ++;
        }
        WORD(w, 4) = 0;
        break;
    default:
        VsRecorder_MenuRestore(p);
        Heap_Free(PTR(p, 0x860));
        VsRecorder_SetSubState(p, 5);
        break;
    }
    return 0;
}
