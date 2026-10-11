#ifndef W13_PRIVATE_H
#define W13_PRIVATE_H
#include "global.h"
typedef struct W13Work {
    u32 spriteId;
    u32 state;
    void *mdl1;
    void *mdl2;
    void *mdl3;
    u8 pad14[0x10];
    void *unk24;
    u8 pad28[0x28];
    void *obj;
    void *reflection;
} W13Work;
void ov01_021FAB9C(void *, W13Work *);
extern BOOL MapObject_CheckActive(void *);
extern BOOL ov01_021FA2D4(void *);
extern u32 MapObject_GetSpriteID(void *);
extern void sub_02068B48(void);
extern void BillboardObject_Delete(void *);
extern void Heap_Free(void *);
extern void SysTask_Destroy(void *);
#endif
