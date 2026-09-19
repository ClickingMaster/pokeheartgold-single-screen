#ifndef TO52_R2_W17B_PRIVATE_H
#define TO52_R2_W17B_PRIVATE_H
#include "global.h"
#include "bg_window.h"
#include "heap.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "systask_environment.h"

typedef struct Ov27FieldFlagsW17 {
    u8 pad[0xD2];
    u8 mode : 6;
    u8 flag6 : 1;
    u8 flag7 : 1;
} Ov27FieldFlagsW17;

typedef struct Ov27WorkW17 {
    u32 state;
    void *args;
    u8 pad008[0x14 - 0x08];
    u32 selection;
    BgConfig *bgConfig;
    SysTask *task;
    void *arg1;
    Ov27FieldFlagsW17 *fieldSystem;
    u8 pad028[0x34 - 0x28];
    u32 unk34;
    u8 pad038[0x44 - 0x38];
    u32 unk44;
    u32 unk48;
    u8 pad04C[0x218 - 0x4C];
    SpriteList *spriteList;
    u8 pad21C[0x344 - 0x21C];
    void *resManagers[4];
    void *charResource;
    void *plttResource;
    u8 pad35C[0x39C - 0x35C];
    u32 unk39C;
    u32 unk3A0;
} Ov27WorkW17;

extern const BgTemplate ov27_0225D370;
extern const BgTemplate ov27_0225D38C;
extern const BgTemplate ov27_0225D3A8;
void ov27_0225C434(SysTask *, Ov27WorkW17 *);
void ov27_0225C914(void *, u32);
void ov27_0225C930(void *);
void ov27_0225C4AC(void *);
void ov27_0225C72C(void *);
void ov27_0225C80C(void *, int);
void TextFlags_SetCanTouchSpeedUpPrint(BOOL);
void FontID_Alloc(u32, u32);
void FontID_Release(u32);

void *ov27_0225C250(BgConfig *, void *, Ov27FieldFlagsW17 *, void *);
void ov27_0225C398(BgConfig *, SysTask *);
#endif
