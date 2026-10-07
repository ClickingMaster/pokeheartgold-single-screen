#define ov81_0223FC74 ov81_0223FC74_header_decl
#include "to54_atomic_h05_private.h"
#undef ov81_0223FC74

extern void ClearWindowTilemapAndCopyToVram(void *);
extern void ClearWindowTilemapAndScheduleTransfer(void *);
extern void ClearFrameAndWindow2(void *, int);
extern void ov81_0224300C(void *);
extern void ov81_02242D74(void *);
extern void ov81_02241FC0(void *);
extern void ov81_02242170(void *);
extern void ov81_02242DE4(void *, int);
extern void ov81_02242D88(void *, int);
extern void ov81_02240728(void *, int);
extern void ov81_022406E0(void *, int);
extern void ov81_0223EBE4(void *);
extern void ov81_02241EDC(void *, int, int, int);
extern void Pokepic_Delete(void *);
extern BOOL ov81_02240564(void *);
extern void *ov81_02242EA4(void *);
extern void sub_0201980C(void *, int);
typedef struct {
    u8 pad[0x1AC];
    void *pics[1];
} PicWork;
typedef struct {
    u8 pad[0x360];
    void *slots[1];
} SlotWork;
BOOL ov81_0223FC74(u8 *);

BOOL ov81_0223FC74(u8 *p) {
    u8 *q;
    int count;
    int mode;

    ov80_02236DD4(p[9]);
    switch (p[8]) {
    case 0:
        p[0x13] |= 0x20;
        ClearWindowTilemapAndCopyToVram(p + 0x60);
        ClearWindowTilemapAndCopyToVram(p + 0xA0);
        ClearWindowTilemapAndCopyToVram(p + 0xB0);
        FillWindowPixelBuffer(p + 0x50, 0);
        ScheduleWindowCopyToVram(p + 0x50);
        ClearWindowTilemapAndCopyToVram(p + 0x70);
        ClearWindowTilemapAndCopyToVram(p + 0x80);
        ClearWindowTilemapAndCopyToVram(p + 0x90);
        ClearWindowTilemapAndCopyToVram(p + 0xD0);
        sub_0201980C(PTR(p, 0x474), 0);
        ClearFrameAndWindow2(p + 0xC0, 1);
        ClearWindowTilemapAndScheduleTransfer(p + 0xC0);
        ov81_0224300C(p + 0x50);
        Pokepic_StartPaletteFadeAll(PTR(p, 0x1A8), 0, 0x10, 0, 0xFFFF);
        GfGfx_EngineATogglePlanes(4, 0);
        p[0x19] = 0;
        p[8]++;
        break;
    case 1: {
        int i;
        p[0x19]++;
        if (p[0x19] < 2) {
            break;
        }
        for (i = 0; (u32)i < U32(p, 0x47C); i++) {
            Pokepic_Delete(((PicWork *)p)->pics[i]);
        }
        ov81_02242D74(PTR(p, 0x388));
        ov81_02242D74(PTR(p, 0x38C));
        ov81_02242D74(PTR(p, 0x390));
        ov81_02242D74(PTR(p, 0x394));
        ov81_02242D74(PTR(p, 0x398));
        ov81_02241FC0(p);
        ov81_02242170(p);
        ov81_02242DE4(PTR(p, 0x380), 2);
        p[0x19] = 0;
        p[8]++;
        break;
    }
    case 2:
        Pokepic_ResumePaletteFade(PTR(p, 0x1AC));
        if (ov81_02242DD8(PTR(p, 0x380)) == 1) {
            break;
        }
        ov81_02242D88(PTR(p, 0x380), 0);
        ov81_02240728(p, 6);
        ov81_022406E0(p, 2);
        BgSetPosTextAndCommit(PTR(p, 0x4C), 6, 0, U32(p, 0xC));
        PlaySE(0x611);
        p[0x19] = 0;
        p[8]++;
        break;
    case 3:
        if (ov81_02240564(p) != 1) {
            break;
        }
        StopSE(0x611, 0);
        PlaySE(0x678);
        U32(p, 0x14) = 8;
        count = 0;
        if (count < p[0x12]) {
            q = p;
            mode = 0;
            do {
                PTR(q, 0x360) = ov81_02242EA4(PTR(q, 0x360));
                U32(q, 0x360) = mode;
                count++;
                q += 4;
            } while (count < p[0x12]);
        }
        if (ov80_02237254(p[9]) == 1) {
            if (p[0x12] == 2) {
                p[0x12] = 4;
            } else {
                p[0x12] = 2;
            }
        }
        ov81_0223EBE4(p);
        {
            int i;
            i = 0;
            if (i < p[0x12]) {
                do {
                    ov81_02242F60(((SlotWork *)p)->slots[i]);
                    i++;
                } while (i < p[0x12]);
            }
        }
        PlaySE(0x611);
        p[0x19] = 0;
        p[8]++;
        break;
    case 4: {
        int i;
        if (ov81_022404B4(p) != 1) {
            break;
        }
        StopSE(0x611, 0);
        PlaySE(0x678);
        i = 0;
        if (i < p[0x12]) {
            do {
                ov81_02242F94(((SlotWork *)p)->slots[i], 4);
                i++;
            } while (i < p[0x12]);
        }
        U32(p, 0x14) = 8;
        ov81_02242D88(PTR(p, 0x380), 1);
        ov81_02242DE4(PTR(p, 0x380), 1);
        p[0x19] = 0;
        p[8]++;
        break;
    }
    case 5:
        if (ov81_02242DD8(PTR(p, 0x380)) == 1) {
            break;
        }
        p[0x19] = 0;
        p[8]++;
        break;
    case 6:
        if (p[0x19] == 0) {
            GfGfx_EngineATogglePlanes(2, 0);
            if (BIT(p[0x13], 2) == 0) {
                count = 0;
                if ((u32)count < U32(p, 0x47C)) {
                    do {
                        ov81_02241E68(p, count, (void *)count, 1);
                        count++;
                    } while ((u32)count < U32(p, 0x47C));
                }
            } else {
                count = 0;
                if ((u32)count < U32(p, 0x47C)) {
                    do {
                        if (count == U16(p, 0x3C8)) {
                            ov81_02241EDC(p, count, 0, 0);
                        } else {
                            ov81_02241E68(p, count, (void *)count, 1);
                        }
                        count++;
                    } while ((u32)count < U32(p, 0x47C));
                }
            }
            Pokepic_StartPaletteFadeAll(PTR(p, 0x1A8), 0x10, 0, 1, 0xFFFF);
        }
        Pokepic_ResumePaletteFade(PTR(p, 0x1AC));
        p[0x19]++;
        if (p[0x19] < 21) {
            break;
        }
        ov81_02241398(p);
        p[0x13] &= ~0x20;
        if (ov80_02237254(p[9]) == 1) {
            p[0x463] = 1;
            ov81_02241144(p);
        }
        p[0x19] = 0;
        return TRUE;
    }
    return FALSE;
}
