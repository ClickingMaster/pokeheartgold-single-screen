#pragma force_active on
#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define I(p, o) (*(int *)((u8 *)(p) + (o)))
#define U(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define B(p, o) (*((u8 *)(p) + (o)))
#define V(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct {
    char *key;
    void (*cb)(u8 *, u8 *);
} Dispatch;

extern Dispatch ov45_02256410[];
extern int ov45_02255DA0;
extern char ov45_022565C0[];
extern char ov45_022565D0[];
extern char ov45_022563F0[];
extern char ov45_0225680C[];
extern char ov45_02256998[];
extern char ov45_02256B84[];
extern char ov45_02256B94[];
extern char ov45_02256B24[];
extern void __msl_assertion_failed(char *, char *, char *, int);
extern void ov00_021EC3D8(void *);
extern void ov45_02251B0C(void *, int, void *, int, void *);
extern void *ov00_021EC3A8(int);
extern char *ov00_021FF3D4(char *);
extern int strnicmp(char *, char *);
void ov45_0224F490(u8 *, u8 *);
int ov45_0224F5BC(u8 *, u8 *);
extern long atol(const char *);

int ov45_0224F5BC(u8 *p, u8 *q) {
    int i;
    if (!q) {
        __msl_assertion_failed(ov45_02256B84, ov45_02256B94, ov45_02256B24, 79);
    }
    for (i = 0; i < ov45_02255DA0; i++) {
        if (strnicmp(V(q, 0x14), ov45_02256410[i].key) == 0) {
            void (*cb)(u8 *, u8 *) = ov45_02256410[i].cb;
            if (cb) {
                cb(p, q);
            }
            return 1;
        }
    }
    return 0;
}
