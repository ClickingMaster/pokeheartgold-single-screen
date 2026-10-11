#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#define W(p, o) (*(u32 *)((p) + (o)))
#define P(p, o) (*(u8 **)((p) + (o)))
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern int ov00_021EC3D8();
extern int ov00_021FF368();
extern int ov45_0224A14C();
extern int ov45_0224A708();
extern char ov45_02255E60[];
extern char ov45_02255EC4[];
extern char ov45_02255FC4[];
extern char ov45_02255FD4[];
extern char ov45_022560B0[];
extern char ov45_022565C0[];
extern char ov45_022565D0[];
extern char ov45_02256644[];
extern char ov45_022566A0[];
void ov45_0224966C(char *p, int n, char *key);
void ov45_02249904(u8 *p);
void ov45_0224A578(u8 *p);
void ov45_0224A5E4(u8 *p);
int ov45_0224A630(u8 *p, int x);
void ov45_0224A6B4(u8 *p);
void ov45_0224A9F8(u8 *p, void *x, void *y, void *z);

int ov45_0224A630(u8 *p, int x) {
    u8 *n;
    if (!p) {
        __msl_assertion_failed(ov45_022565C0, ov45_022565D0, ov45_022560B0, 832);
    }
    if (x <= 0) {
        __msl_assertion_failed(ov45_022566A0, ov45_022565D0, ov45_022560B0, 834);
    }
    for (n = P(p, 0x814); n; n = P(n, 36)) {
        if (W(n, 32) == x) {
            return 1;
        }
    }
    return 0;
}

void ov45_0224A6B4(u8 *p) {
    if (!p) {
        __msl_assertion_failed(ov45_022565C0, ov45_022565D0, ov45_02255FD4, 851);
    }
    while (P(p, 0x814)) {
        ov45_0224A14C(p, P(p, 0x814));
    }
}
