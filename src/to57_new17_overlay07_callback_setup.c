#include "global.h"

#include "pokepic.h"
#include "sprite_system.h"
#define F(p, t, o) (*(t *)((u8 *)(p) + (o)))
typedef struct Work Work;
void *ov07_022324D8(void *, u32);
void ov07_02231FE4(void *, void *);

extern u32 ov07_0221C4A8(void *p, int i);
int ov07_0221C468(void *);
int ov07_0221C470(void *);
int ov07_02222004(void *, int);
Pokepic *ov07_0221FA48(void *, int);
void ov07_02231FA0(void *, void *);
void ov07_02222338(void *, void *, int, int, int, int, int, int);
int ov07_0221FA04(void *, int);
void ov07_022223F0(void *, int, int, int);
BOOL ov07_022223CC(void *, void *, ManagedSprite *);
void ov07_0221C410(void *, void (*)(int, void *), void *);
void ov07_0222928C(int, void *);
void ov07_022292E4(void *, int, int, ManagedSprite *);
void ov07_022292E4(void *arg0, int unused1, int unused2, ManagedSprite *arg3) {
    int var_r5;
    void *w;
    w = ov07_022324D8(arg0, 0xE8);
    ov07_02231FE4(arg0, (u8 *)w + 0x10);
    F(w, s16, 0xC) = ((s32)ov07_0221C4A8(arg0, 0));
    F(w, s16, 0xE) = ((s32)ov07_0221C4A8(arg0, 1));
    F(w, int, 4) = ((s32)ov07_0221C4A8(arg0, 2));
    F(w, int, 8) = ((s32)ov07_0221C4A8(arg0, 3));
    var_r5 = ov07_02222004(arg0, ov07_0221C468(arg0));
    F(w, ManagedSprite *, 0x38) = arg3;
    ManagedSprite_SetAffineOverwriteMode(arg3, 2);
    F(w, Pokepic *, 0x44) = ov07_0221FA48(arg0, ov07_0221C468(arg0));
    F(w, Pokepic *, 0x58) = ov07_0221FA48(arg0, ov07_0221C470(arg0));
    ov07_02231FA0(F(w, void *, 0x44), (u8 *)w + 0x40);
    ov07_02231FA0(F(w, void *, 0x58), (u8 *)w + 0x54);
    ov07_02222338((u8 *)w + 0x8C, (u8 *)w + 0xB0, F(w, s16, 0x40), (s16)(F(w, s16, 0x54) + F(w, s16, 0xC) * var_r5), F(w, s16, 0x42), (s16)(F(w, s16, 0x56) + F(w, s16, 0xE) * var_r5), (u16)F(w, int, 4), F(w, int, 8) << 12);
    if (ov07_0221FA04(arg0, ov07_0221C468(arg0)) == 4 && ov07_0221FA04(arg0, ov07_0221C470(arg0)) == 2) {
        var_r5 *= -1;
    }
    if (ov07_0221FA04(arg0, ov07_0221C468(arg0)) == 5 && ov07_0221FA04(arg0, ov07_0221C470(arg0)) == 3) {
        var_r5 *= -1;
    }
    if (var_r5 > 0) {
        ov07_022223F0((u8 *)w + 0xD4, 0xE38 * var_r5, 0x5C71 * var_r5, 10);
    } else {
        ov07_022223F0((u8 *)w + 0xD4, 0x3FFF * var_r5, 0x5C71 * var_r5, 10);
    }
    ManagedSprite_SetAffineZRotation(F(w, ManagedSprite *, 0x38), (u16)F(w, int, 0xD4));
    ov07_022223CC((u8 *)w + 0x8C, (u8 *)w + 0xB0, F(w, ManagedSprite *, 0x38));
    ManagedSprite_TickFrame(F(w, ManagedSprite *, 0x38));
    ov07_0221C410(F(w, void *, 0x14), ov07_0222928C, w);
}
