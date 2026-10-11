#include "global.h"
typedef struct Ov93E4B0 {
    u8 pad0[0x1768];
    u32 other;
    u8 pad1[0x2fcc-0x176c];
    u32 prev;
    u32 gap;
    u32 kind;
    u32 arg;
    u8 pad2[0x339c-0x2fdc];
    u32 flag;
} Ov93E4B0;
extern void LinkSync_StartSync(u8);
extern u32 ov93_0225E0F4(void *);
u32 ov93_0225E4B0(Ov93E4B0 *p);
u32 ov93_0225E4B0(Ov93E4B0 *p) {
    u32 ready = 0;
    switch (p->kind) {
    case 1:
        ready = 1;
        break;
    case 2:
        ready = 1;
        break;
    case 3:
        ready = 1;
        break;
    case 4:
        if (p->prev == 1) ready = 1;
        break;
    case 5:
        if (p->flag == 0) ready = 1;
        break;
    case 6:
        if (p->other == 1) ready = 1;
        break;
    default:
        return 0;
    }
    if (ready == 1) {
        LinkSync_StartSync((u8)p->arg);
        if (ov93_0225E0F4(p) == 1) {
            p->kind = 7;
        } else {
            p->kind = 0;
        }
        return 1;
    }
    return 0;
}
