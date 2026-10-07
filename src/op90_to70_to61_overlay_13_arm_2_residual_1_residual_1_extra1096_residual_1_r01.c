#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef struct {
    u32 attr01;
    u16 attr2;
} Oam;
typedef struct {
    void *task;
    Oam *oam;
    u8 kind;
} Work6F58;
extern Work6F58 *ov13_0224F464;
extern const u8 ov13_02244EB0[];
void *ov13_0224128C(u32 size, u32 align);
Oam *ov13_02240184(int a, int b);
void *ov13_02242528(int a, void (*fn)(void *), int b, int c);
void ov13_02227064(void *arg);
void ov13_02226F58(u8 kind);

static inline void SetOamPriority(Oam *oam, int prio) {
    oam->attr2 = (oam->attr2 & ~0xc00) | (prio << 10);
}

static inline void SetOamModePalette(Oam *oam, int mode, int pal) {
    oam->attr01 = (oam->attr01 & ~0xc00) | (mode << 10);
    oam->attr2 = (oam->attr2 & ~0xf000) | (pal << 12);
}

static inline void SetOamPos(Oam *oam, int x, int y) {
    oam->attr01 = (oam->attr01 & 0xFE00FF00) | ((u32)(y & 0xff)) | ((u32)(x & 0x1ff) << 16);
}

// @fn ov13_02226F58
void ov13_02226F58(u8 kind) {
    ov13_0224F464 = ov13_0224128C(sizeof(Work6F58), 4);
    ov13_0224F464->kind = kind;
    ov13_0224F464->oam = ov13_02240184(0, 0x47);
    SetOamPriority(ov13_0224F464->oam, 1);
    SetOamModePalette(ov13_0224F464->oam, 0, ov13_02244EB0[kind]);
    SetOamPos(ov13_0224F464->oam, 0xe6, 0x8b);
    ov13_0224F464->task = ov13_02242528(1, ov13_02227064, 0, 0x78);
}
