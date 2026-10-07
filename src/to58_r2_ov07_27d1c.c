#include "global.h"

#define FIELD(p, type, off) (*(type *)((u8 *)(p) + (off)))

extern void *Heap_Alloc(int, int);
extern void Heap_Free(void *);
extern void ov07_0221C448(void *, int);
extern int ov07_0221BFD0(int);
extern int ov07_0221C468(int);
extern int ov07_0221C514(int);
extern int ov07_0221FA48(int, int);
extern int ov07_0221FA80(int, int);
extern int ov07_0221FA90(int, int);
extern int ov07_0221FAE8(void *);
extern int ov07_0221FA78(int);
extern int ov07_0221C4E8(int, int);
extern int ov07_0223192C(int, int);
extern void ov07_02231E08(int, int, int);
extern void ov07_0221C410(int, void (*)(int, void *), void *);
extern void ov07_02222268(void *, s16, s16, s16, s16, u16);
extern int ov07_022222B4(void *);
extern void ManagedSprite_SetPositionXY(void *, s16, s16);
extern void ManagedSprite_TickFrame(void *);
extern void SpriteSystem_DrawSprites(void *);
extern void ManagedSprite_SetOamMode(void *, int);
extern void ManagedSprite_SetDrawPriority(void *, int);
extern void ManagedSprite_SetPriority(void *, int);
extern int Sprite_GetPaletteProxy(void *);
extern int ObjPlttTransfer_GetPaletteVramOffset(int, int);
extern void PaletteData_LoadNarc_CustomTint(int, int, int, int, int, int, int, int, int, int);
extern s16 Pokepic_GetAttr(int, int);

extern s16 ov07_02236732[];
extern s16 ov07_02236734[];
extern s16 ov07_02236736[];

void ov07_02227B58(int arg0, void *work);
void ov07_02227D1C(int arg0);

void ov07_02227D1C(int arg0) {
    void *work;
    int i;
    int priority;
    int archive;
    u16 palette;
    int tex;
    int context;
    u8 *slot;
    work = Heap_Alloc(ov07_0221BFD0(arg0), 0xb8);
    FIELD(work, u8, 1) = 0;
    FIELD(work, u8, 0) = 0;
    FIELD(work, int, 0xc) = arg0;
    FIELD(work, int, 0x10) = ov07_0221FA48(FIELD(work, int, 0xc), ov07_0221C468(FIELD(work, int, 0xc)));
    FIELD(work, s16, 8) = Pokepic_GetAttr(FIELD(work, int, 0x10), 0);
    FIELD(work, s16, 10) = Pokepic_GetAttr(FIELD(work, int, 0x10), 1);
    FIELD(work, s16, 10) -= Pokepic_GetAttr(FIELD(work, int, 0x10), 0x29);
    FIELD(work, int, 0x14) = ov07_0221C514(FIELD(work, int, 0xc));
    FIELD(work, u8, 4) = 8;
    FIELD(work, u8, 5) = 6;
    ov07_02231E08(FIELD(work, int, 0xc), -1, -1);
    *(volatile u16 *)0x04000052 = FIELD(work, u8, 4) | (FIELD(work, u8, 5) << 8);
    archive = ov07_0221FA80(FIELD(work, int, 0xc), ov07_0221C468(FIELD(work, int, 0xc)));
    priority = ov07_0221FAE8((void *)(u32)FIELD(work, int, 0xc));
    tex = ov07_0221FA90(FIELD(work, int, 0xc), ov07_0221C468(FIELD(work, int, 0xc)));
    i = 0;
    slot = work;
    do {
        void *sprite = (void *)ov07_0221C4E8(FIELD(work, int, 0xc), i);
        FIELD(slot, void *, 0x18) = sprite;
        ManagedSprite_SetOamMode(sprite, 1);
        slot += 4;
        i++;
    } while (i < 4);
    context = ov07_0223192C(FIELD(work, int, 0xc), ov07_0221C468(FIELD(work, int, 0xc)));
    if (context == 3) {
        ManagedSprite_SetDrawPriority(FIELD(work, void *, 0x18), 10);
        ManagedSprite_SetDrawPriority(FIELD(work, void *, 0x1c), 10);
        ManagedSprite_SetDrawPriority(FIELD(work, void *, 0x20), 20);
        ManagedSprite_SetDrawPriority(FIELD(work, void *, 0x24), 20);
        ManagedSprite_SetPriority(FIELD(work, void *, 0x18), priority);
        ManagedSprite_SetPriority(FIELD(work, void *, 0x1c), priority);
        ManagedSprite_SetPriority(FIELD(work, void *, 0x20), priority);
        ManagedSprite_SetPriority(FIELD(work, void *, 0x24), priority);
        palette = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(*(void **)FIELD(work, void *, 0x18)), 1);
        PaletteData_LoadNarc_CustomTint(ov07_0221FA78(FIELD(work, int, 0xc)), tex, archive, ov07_0221BFD0(arg0), 2, 0x20, (u16)(palette << 4), 0x80, 0x80, 0x80);
        palette = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(*(void **)FIELD(work, void *, 0x20)), 1);
        PaletteData_LoadNarc_CustomTint(ov07_0221FA78(FIELD(work, int, 0xc)), tex, archive, ov07_0221BFD0(arg0), 2, 0x20, (u16)(palette << 4), 0xc4, 0xc4, 0xc4);
    } else {
        ManagedSprite_SetDrawPriority(FIELD(work, void *, 0x18), 20);
        ManagedSprite_SetDrawPriority(FIELD(work, void *, 0x1c), 20);
        ManagedSprite_SetDrawPriority(FIELD(work, void *, 0x20), 10);
        ManagedSprite_SetDrawPriority(FIELD(work, void *, 0x24), 10);
        ManagedSprite_SetPriority(FIELD(work, void *, 0x18), priority + 1);
        ManagedSprite_SetPriority(FIELD(work, void *, 0x1c), priority + 1);
        ManagedSprite_SetPriority(FIELD(work, void *, 0x20), priority + 1);
        ManagedSprite_SetPriority(FIELD(work, void *, 0x24), priority + 1);
        ManagedSprite_SetPriority(FIELD(work, void *, 0x18), priority);
        ManagedSprite_SetPriority(FIELD(work, void *, 0x1c), priority);
        ManagedSprite_SetPriority(FIELD(work, void *, 0x20), priority);
        ManagedSprite_SetPriority(FIELD(work, void *, 0x24), priority);
        palette = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(*(void **)FIELD(work, void *, 0x18)), 1);
        PaletteData_LoadNarc_CustomTint(ov07_0221FA78(FIELD(work, int, 0xc)), tex, archive, ov07_0221BFD0(arg0), 2, 0x20, (u16)(palette << 4), 0xc4, 0xc4, 0xc4);
        palette = ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(*(void **)FIELD(work, void *, 0x20)), 1);
        PaletteData_LoadNarc_CustomTint(ov07_0221FA78(FIELD(work, int, 0xc)), tex, archive, ov07_0221BFD0(arg0), 2, 0x20, (u16)(palette << 4), 0x80, 0x80, 0x80);
    }
    ov07_0221C410(FIELD(work, int, 0xc), ov07_02227B58, work);
}
