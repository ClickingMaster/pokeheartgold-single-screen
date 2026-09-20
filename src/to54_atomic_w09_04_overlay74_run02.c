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

void ov74_02230404(void *arg) {
    ov74_0223144C(U16AT(arg, 8));
    if (U16AT(arg, 2) == 0) {
        ov74_02231100();
        ov74_02231450();
        ov74_02231070(6);
        if (!ov74_0222FE78()) {
            if (ov74_02231094() == 1) {
                u8 *p = ov74_02231154();
                p[0x1C3] = (p[0x1C3] & ~0xF0) | 0x10;
            }
            if (U16AT(arg, 8) == 4) {
                goto perform_30404;
            }
            if (U16AT(arg, 8) == 5) {
                if (ov74_02230320(arg)) {
                    return;
                }
perform_30404:
                ov74_0223030C();
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
