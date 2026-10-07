#pragma force_active on
#include "global.h"

typedef struct Owner {
    int heap;
    u8 pad04[0x44];
    union {
        struct {
            void *a;
            u32 unk04[2];
            void *b;
            u32 unk10[4];
        } spr[4];
        struct {
            u8 pad[0x74];
            void *iconBuf[12];
            NNSG2dCharacterData *iconChar[12];
            u8 iconPal[12];
        } icon;
    } u;
} Owner;

void *NARC_New(int narcId, int heapId);
void NARC_Delete(void *narc);
void NARC_ReadWholeMember(void *narc, int member, void *dest);
void *Heap_AllocAtEnd(int heapId, u32 size);
int sub_02074490(void);
int GetMonIconNaixEx(int species, BOOL isEgg, int form);
int GetMonIconPaletteEx(int species, int form, BOOL isEgg);
void GfGfxLoader_GXLoadPal(int narcId, int member, int location, int offset, int size, int heapId);
BOOL NNS_G2dGetUnpackedBGCharacterData(void *pNcgrFile, NNSG2dCharacterData **ppCharData);
void ov96_022004B4(Owner *owner, u8 idx, int arg);
void ov96_021EB52C(void *obj, int a, int b);
void ov96_021EB588(void *obj, VecFx32 *pos);
void ov96_022002F8(Owner *owner, u16 *mons);


// @fn ov96_022002F8
void ov96_022002F8(Owner *owner, u16 *mons) {
    void *narc;
    int i;
    NNSG2dCharacterData **chars;
    int form, species;
    VecFx32 pos;
    Owner *p;

    narc = NARC_New(20, owner->heap);
    GfGfxLoader_GXLoadPal(20, sub_02074490(), 5, 32, 96, owner->heap);
    for (i = 0, p = owner, chars = owner->u.icon.iconChar; i < 12; p = (Owner *)((u8 *)p + 4), mons += 2, chars++, i++) {
        p->u.icon.iconBuf[0] = Heap_AllocAtEnd(owner->heap, 0x1000);
        form = (u8)mons[1];
        species = mons[0];
        NARC_ReadWholeMember(narc, GetMonIconNaixEx(species, FALSE, form), p->u.icon.iconBuf[0]);
        NNS_G2dGetUnpackedBGCharacterData(p->u.icon.iconBuf[0], chars);
        owner->u.icon.iconPal[i] = GetMonIconPaletteEx(species, form, FALSE) + 1;
    }
    NARC_Delete(narc);
    for (i = 0; i < 4; i++) {
        ov96_022004B4(owner, i, 0);
        ov96_021EB52C(owner->u.spr[i].a, 1, 1);
        ov96_021EB52C(owner->u.spr[i].b, 1, 1);
        pos.z = 0;
        pos.x = 10 << 16;
        pos.y = 43 << 16;
        ov96_021EB588(owner->u.spr[i].a, &pos);
        ov96_021EB588(owner->u.spr[i].b, &pos);
    }
}
