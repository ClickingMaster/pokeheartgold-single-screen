#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u32 word;
    u16 half;
} Object;

typedef struct {
    void *resources[4];
    Object *sprites[10], *extra[2];
    void *special[2], *data[4], *one, *effect;
    u8 flags[12];
} Context;

extern Context *ov13_0224F4C0;
extern const u16 ov13_02245228[];
extern const u16 ov13_02245260[];
extern const u8 ov13_02245214[];
extern const u8 ov13_02245218[];
extern Context *ov13_0224128C(int, int);
extern Object *ov13_02240184(int, int);
extern void *ov13_022401B0(int, int, int);
extern void *ov13_02240798(int, int, int, int, void **, int);
extern void *ov13_0223FC04(int, void *, int);
extern void *ov13_02242528(int, void *, int, int);
extern void ov13_0223FDF4(void *, int, int, int);
extern void ov13_02240008(void *, int, int);
extern void ov13_02240CC0(void *, int, int, int, int, int, int, void *);
extern void ov13_0222AAFC(int, int);
extern void ov13_0222A0DC(void);
void ov13_02229D00(void);
#pragma force_active on

void ov13_02229D00(void) {
    u16 rect[4];
    void *resourceData;
    u16 temp[2];
    int offset;
    int index;
    int i;
    rect[0] = ov13_02245228[16];
    rect[1] = ov13_02245228[17];
    rect[2] = 28;
    rect[3] = 20;
    ov13_0224F4C0 = ov13_0224128C(108, 4);
    ov13_0224F4C0->flags[0] = 31;
    ov13_0224F4C0->flags[3] = 0;
    ov13_0224F4C0->flags[6] = 1;
    ov13_0224F4C0->flags[7] = 1;
    ov13_0224F4C0->flags[8] = 1;
    for (i = 0; i < 10; i++) {
        ov13_0224F4C0->sprites[i] = ov13_02240184(0, 54);
        ov13_0224F4C0->sprites[i]->word = (ov13_0224F4C0->sprites[i]->word & 0xc1fffcff) | 0x200;
        ov13_0224F4C0->sprites[i]->half = (ov13_0224F4C0->sprites[i]->half & ~0xc00) | 0xc00;
    }
    for (i = 0; i < 2; i++) {
        ov13_0224F4C0->extra[i] = ov13_02240184(0, ov13_02245214[i]);
        ov13_0224F4C0->extra[i]->word = (ov13_0224F4C0->extra[i]->word & 0xc1fffcff) | 0x200;
        ov13_0224F4C0->extra[i]->half = (ov13_0224F4C0->extra[i]->half & ~0xc00) | 0xc00;
    }
    for (i = 0; i < 2; i++) {
        ov13_0224F4C0->special[i] = ov13_022401B0(0, ov13_02245218[i], 1);
        ov13_0223FDF4(ov13_0224F4C0->special[i], -1, 512, 0);
        ov13_02240008(ov13_0224F4C0->special[i], -1, 3);
    }
    i = 0;
    offset = i;
    temp[1] = (u16)i;
    for (; i < 4; i++) {
        int j;
        ov13_0224F4C0->resources[i] = ov13_02240798(0, 12, 4, 0, &resourceData, 0);
        index = offset;
        rect[0] = 0;
        for (j = 0; j < 3; j++) {
            temp[0] = ov13_02245260[index];
            ov13_02240CC0(ov13_0224F4C0->resources[i], *(volatile u16 *)&rect[0], *(volatile u16 *)&rect[1], *(volatile u16 *)&rect[2], *(volatile u16 *)&rect[3], 2, 1152, temp);
            rect[0] += 32;
            index++;
        }
        ov13_0224F4C0->data[i] = ov13_0223FC04(0, resourceData, 0);
        offset += 3;
    }
    ov13_0224F4C0->one = ov13_022401B0(0, 68, 1);
    ov13_0223FDF4(ov13_0224F4C0->one, -1, 512, 0);
    ov13_02240008(ov13_0224F4C0->one, -1, 2);
    ov13_0224F4C0->effect = ov13_02242528(0, ov13_0222A0DC, 0, 120);
    ov13_0222AAFC(0, 192);
}
