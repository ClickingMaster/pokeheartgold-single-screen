#include "global.h"

#define U8AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32AT(p, o) (*(u32 *)((u8 *)(p) + (o)))

extern void *ov74_02231154(void);
extern void *ov74_0223115C(void);
extern void *ov74_02231184(void);
extern void *ov74_02231100(void);
extern u8 ov74_02231064(void);
extern u8 ov74_02231094(void);
extern void ov74_02231070(u8);
extern void ov74_0223110C(u16);
extern u32 ov74_02231118(void);
extern void ov74_02231124(u32);
extern void ov74_02231130(u32);
extern void ov74_02231448(u16);
extern void ov74_0223144C(u16);
extern void ov74_02231450(void);
extern void ov74_02231458(void);
extern void ov74_0223145C(void *);
extern BOOL ov74_02231424(void *);
extern BOOL ov74_02231560(void);
extern BOOL ov74_02231584(void);
extern BOOL ov74_0223161C(void);
extern BOOL ov74_02231638(void);
extern BOOL ov74_02231670(void);
extern u8 ov74_022311AC(void);
extern u32 ov74_022311BC(void);
extern void ov74_022313F0(void *);

extern void ov74_0222FE4C(void);
extern BOOL ov74_0222FE78(void);
extern void ov74_0222FEA0(u16);
extern BOOL ov74_0222FF28(void);
extern void ov74_0222FEC8(void);
extern void ov74_0222FF80(void);
extern void ov74_0222FFAC(void);
extern BOOL ov74_022314DC(void);

void ov74_0223030C(void);
BOOL ov74_02230320(void *);
void ov74_02230404(void *);
BOOL ov74_02230478(void);
void ov74_02230520(void *);
void ov74_02230590(void *);
void ov74_022305E4(void *);
void ov74_02230674(void);
void ov74_022306C8(void *);
void ov74_02230714(void *);
void ov74_0223078C(void *);
void ov74_02230818(void *);
void ov74_022308E0(void *);

void ov74_02230520(void *arg) {
    ov74_0223144C(U16AT(arg, 8));
    if (U16AT(arg, 2) == 0) {
        u8 *p = ov74_02231154();
        p[0x1C3] = (p[0x1C3] & ~0xF0) | 0x20;
        ov74_02231070(6);
        if (!ov74_0222FE78()) {
            if (U16AT(arg, 8) == 4) {
                goto perform_30520;
            } else if (U16AT(arg, 8) == 5) {
                if (ov74_02230478()) {
                    return;
                }
perform_30520:
                ov74_0222FFAC();
                if (!ov74_02231584()) {
                    ov74_0222FE4C();
                }
            } else {
                ov74_02231448(U16AT(arg, 2));
                ov74_0222FE4C();
            }
        }
    } else {
        ov74_02231448(U16AT(arg, 2));
        ov74_0222FE4C();
    }
}

void ov74_02230590(void *arg) {
    if (U16AT(arg, 2) == 0) {
        ov74_02231070(7);
        if (!ov74_0222FE78() && ov74_02231094() == 1 && (((u32)U8AT(ov74_02231154(), 0x1C3) << 24) >> 28) == 2) {
            if (!ov74_02231638()) {
                ov74_0222FE4C();
            } else {
                ov74_02231070(3);
            }
        }
    } else {
        ov74_02231448(U16AT(arg, 2));
        ov74_0222FE4C();
    }
}

void ov74_022305E4(void *arg) {
    ov74_0223144C(U16AT(arg, 8));
    if (U16AT(arg, 2) == 0) {
        ov74_02231070(9);
        if (!ov74_0222FE78()) {
            switch (U16AT(arg, 8)) {
            case 6:
            case 8:
                break;
            case 7:
                ov74_0223110C(U16AT(arg, 10));
                ov74_02231124(1);
                ov74_02231130(0);
                if (!ov74_02231670()) {
                    ov74_0222FE4C();
                }
                break;
            case 9:
                if (ov74_02231118()) {
                    ov74_02231130(1);
                }
                ov74_02231124(0);
                break;
            default:
                ov74_02231448(U16AT(arg, 2));
                ov74_0222FE4C();
                break;
            }
        }
    } else {
        ov74_02231448(U16AT(arg, 2));
        ov74_0222FE4C();
    }
}

void ov74_02230674(void) {
    u8 state = ov74_02231094();
    if (state != 1) {
        if (state != 2) {
            return;
        }
        if (ov74_0222FF28()) {
            ov74_0222FEC8();
            if (ov74_022314DC()) {
                U8AT(ov74_0223115C(), 0x60) = 1;
                ov74_02231070(3);
                return;
            }
        }
        ov74_0222FE4C();
    } else {
        ov74_0222FF80();
        if (!ov74_02231560()) {
            ov74_0222FE4C();
        } else {
            ov74_02231070(3);
        }
    }
}

void ov74_022306C8(void *arg) {
    u16 state;
    ov74_0223144C(U16AT(arg, 4));
    state = U16AT(arg, 2);
    if (state == 0) {
        if (U16AT(arg, 0) != 14 || !ov74_0222FE78()) {
            u8 mode = ov74_02231094();
            if (mode != 1) {
                if (mode != 2) {
                    return;
                }
                ov74_02231070(10);
            } else {
                ov74_02231070(11);
            }
        }
    } else if (state != 9 && state != 13 && state != 15) {
        ov74_0222FE4C();
    }
}

void ov74_02230714(void *arg) {
    u8 *work = ov74_02231184();
    if (work[0x19] == 1) {
        u8 *data = (u8 *)U32AT(work, 0xC);
        DC_FlushRange((void *)U32AT(arg, 0xC), U16AT(arg, 0x10));
        MI_CpuCopy8((void *)U32AT(arg, 0xC), (void *)U32AT(work, 0xC), U16AT(arg, 0x10));
        if ((u8)U32AT(data, 0) == ov74_022311AC() && ((U32AT(data, 0) << 20) >> 28) <= ov74_022311BC()) {
            ov74_022313F0((void *)U32AT(work, 0xC));
            if ((u8)U32AT(data, 8) == work[0x1C]) {
                ov74_0223145C((void *)U32AT(work, 0xC));
                if (ov74_02231424((void *)U32AT(work, 0xC)) && ((U32AT(data, 8) << 16) >> 24) != 1) {
                    work[0x19] = 2;
                } else {
                    work[0x19] = 0;
                }
            }
        }
    }
}
