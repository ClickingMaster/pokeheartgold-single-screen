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

void ov45_0224A578(u8 *p) {
    u8 *n, *next;
    u32 now;
    if (!p) {
        __msl_assertion_failed(ov45_022565C0, ov45_022565D0, ov45_02255EC4, 794);
    }
    now = ov00_021FF368();
    for (n = P(p, 0x814); n; n = next) {
        next = P(n, 36);
        if (now > W(n, 4)) {
            ov45_0224A14C(p, n);
        }
    }
}
