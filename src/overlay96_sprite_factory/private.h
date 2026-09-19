#ifndef TO53_H08_OV96_R23_PRIVATE_H
#define TO53_H08_OV96_R23_PRIVATE_H

#include "global.h"
#include "sprite.h"

typedef struct Ov96R23Entry {
    void *obj;
    u8 pad04[0x24];
    void *sprite;
    u8 pad2C[0x18];
} Ov96R23Entry;

typedef struct Ov96R23Work {
    enum HeapID heapID;
    int count;
    int base;
    int done;
    SpriteList *spriteList;
    void *resource;
    void *resources;
    Ov96R23Entry entries[1];
} Ov96R23Work;

int ov96_021E8B88(void *);
void *ov96_021E8B8C(void *, u8);
void ov96_021EABA8(void *, int);
void *ov96_021E90FC(void *);
void SysTask_Destroy(void *);
void *ov96_021E8BAC(void *);
void *ov96_021E9D08(void *, int);

void ov96_021EAA24(void *, Ov96R23Work *);
Sprite *ov96_021EAA88(Ov96R23Work *, int, NNS_G2D_VRAM_TYPE, int);
void ov96_021EAB38(u8 *, int);
void ov96_021EAB74(u8 *, int);

#endif
