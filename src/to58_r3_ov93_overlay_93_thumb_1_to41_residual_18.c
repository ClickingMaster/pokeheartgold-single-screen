#include "global.h"
typedef struct Ov93InitWork {
    u8 pad0[0x21c];
    u32 a, b;
    u8 pad1[0x234-0x224];
    void *ptr;
    u8 pad2[0x248-0x238];
    u32 c;
    u8 pad3[0x260-0x24c];
    u32 x, y;
    u8 pad4[0x270-0x268];
    u32 kind;
} Ov93InitWork;
typedef struct Ov93InitArg { u8 pad[0x34]; u32 kind; } Ov93InitArg;
extern void *Heap_Alloc(u32, u32);
extern void MI_CpuFill8(void *, u8, u32);
extern void ov93_0225EC98(void *);
extern void ov93_0225EF0C(void *);
extern void ov93_0225EF5C(void *);
extern void ov93_0225F9D8(void *);
extern void *ov93_0225FB00(void *);
extern void ov93_0225FB6C(void *, void *);
Ov93InitWork *ov93_0225E7B0(Ov93InitArg **p);
Ov93InitWork *ov93_0225E7B0(Ov93InitArg **p) {
    Ov93InitWork *w = Heap_Alloc(0x75, 0x278);
    MI_CpuFill8(w, 0, 0x278);
    w->kind = (*p)->kind;
    ov93_0225EC98(w);
    ov93_0225EF0C(w);
    ov93_0225EF5C(w);
    switch ((*p)->kind) {
    case 2:
        w->x = 0x40000;
        w->y = 0x68000;
        break;
    case 1:
        w->x = 0x90000;
        w->y = 0x70000;
        break;
    default:
        w->x = 0x60000;
        w->y = 0x60000;
        break;
    }
    w->a = 0x80;
    w->b = 0x28;
    ov93_0225F9D8(p);
    w->ptr = ov93_0225FB00(p);
    ov93_0225FB6C(w, w->ptr);
    w->c = 0x2000;
    return w;
}
