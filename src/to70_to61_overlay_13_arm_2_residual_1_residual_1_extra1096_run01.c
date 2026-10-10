#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 bits;
    u16 flags;
} Object;

typedef struct {
    void *task;
    Object *obj;
    u8 idx, tick;
} Work;

typedef struct {
    void *obj[3];
    void *task;
    u8 pad[6];
    s8 flag;
    u8 idx;
    u8 pad2[4];
} Other;

extern s32 FX_DivS32(s32, s32);
extern Work *ov13_0224F464;
extern Other *ov13_0224F468;
extern u8 ov13_02244EB0[];
extern u8 ov13_02244EE0[];
extern u8 ov13_02244EC4[];
extern void *ov13_0224128C(int, int);
extern Object *ov13_02240184(int, int);
extern void *ov13_02242528(int, void (*)(void *), int, int);
extern void ov13_022425F0(int, void *);
extern void ov13_02241D98(void *);
extern void ov13_022412B0(u32);
extern void ov13_022400F0(int, int, void *);
extern void *ov13_022401B0(int, int, int);
extern void ov13_02240008(void *, int, int);
extern void ov13_022272FC(int);
extern void ov13_02227390(void *);
void ov13_02227064(void *);

void ov13_02226F58(int index);
void ov13_02227028(void);
void ov13_02227130(int index);

void ov13_02227064(void *unused) {
    Object *p;
    u32 value;
    ov13_0224F464->tick = FX_ModS32(ov13_0224F464->tick + 1, 40);
    {
        int n = FX_DivS32(ov13_0224F464->tick, 5) + 71;
        ov13_022400F0(0, n, (*(Work *volatile *)&ov13_0224F464)->obj);
    }
    p = ov13_0224F464->obj;
    p->flags = (p->flags & ~0xc00) | 0x400;
    value = ov13_02244EB0[ov13_0224F464->idx];
    p = ov13_0224F464->obj;
    p->bits &= ~0xc00;
    p->flags = (p->flags & ~0xf000) | (value << 12);
    p = ov13_0224F464->obj;
    p->bits = (p->bits & 0xfe00ff00) | 0xe6008b;
}

void ov13_02227130(int index) {
    int count = ov13_02244EE0[index], i;
    ov13_0224F468 = ov13_0224128C(28, 4);
    ov13_0224F468->flag = -2;
    ov13_0224F468->idx = index;
    for (i = 0; i < count; i++) {
        ov13_0224F468->obj[i] = ov13_022401B0(0, ov13_02244EC4[index * 2 + i], 1);
        ov13_02240008(ov13_0224F468->obj[i], -1, 1);
    }
    ov13_0224F468->obj[2] = ov13_022401B0(0, 1, 1);
    ov13_02240008(ov13_0224F468->obj[2], -1, 1);
    ov13_022272FC(192);
    ov13_0224F468->task = ov13_02242528(0, ov13_02227390, 0, 120);
}
