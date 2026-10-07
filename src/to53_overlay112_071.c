#include "global.h"
#include "system.h"
#pragma require_prototypes off

typedef struct Ov112State {
    u8 raw[0x1f300];
} Ov112State;

extern int TextPrinterCheckActive(u8);
extern int ManagedSprite_IsAnimated(void *);
extern void BeginNormalPaletteFade(int, int, int, int, int, int, int);
extern int IsPaletteFadeFinished(void);
extern int SaveGameNormal(void *);
extern void Save_ClearStatusFlags(void *);
extern int _s32_div_f(int, int);
extern int ov112_021EA08C(Ov112State *, int, int);
extern void ov112_021EA17C(Ov112State *);
extern void ov112_021EA19C(Ov112State *);
extern void ov112_021EA51C(Ov112State *, int);
extern void ov112_021EA570(Ov112State *);
extern void ov112_021EA584(Ov112State *);
extern void ov112_021EA64C(Ov112State *);
extern void ov112_021EA688(Ov112State *, int);
extern void ov112_021EA864(Ov112State *);
extern void ov112_021E7CC8(Ov112State *);
extern void ov112_021E7484(int);
extern void ov112_021E7CA4(Ov112State *, int, int);
extern void ov112_021E966C(Ov112State *, int);
extern void ov112_021E96D0(Ov112State *, int);
extern void ov112_021E9A78(void *, int);
extern void ov112_021E9C10(Ov112State *, int);
extern void ov112_021E9F40(Ov112State *, int);
extern void ov112_021ED2F4(Ov112State *);
extern void BgClearTilemapBufferAndCommit(void *, int);
void ov112_021EC460(Ov112State *, int, int, int);
extern int (*ov112_021FF730[])(Ov112State *);
extern int (*ov112_021FF740[])(Ov112State *);
extern int (*ov112_021FF7CC[])(Ov112State *);

#define W(p, o) (*(int *)((p)->raw + (o)))
#define H(p, o) (*(u16 *)((p)->raw + (o)))


int ov112_021EBA08(void) { BeginNormalPaletteFade(0, 1, 1, 0, 6, 1, 0x9a); return 2; }

int ov112_021EBA28(Ov112State *p) {
    W(p, 8) = ov112_021FF730[W(p, 8)](p);
    if (W(p, 8) == 4) { W(p, 4) = 11; W(p, 8) = 0; return 3; }
    return 2;
}

int ov112_021EBA50(void) { return 1; }

int ov112_021EBA54(Ov112State *p) { W(p, 0x1e524) = ov112_021EA08C(p, 2, 0x93); return 1; }
