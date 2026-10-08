#pragma force_active on
#include "global.h"
#include "system.h"
typedef struct { u32 v[2]; s32 x, y; u32 rest[8]; } Template;
typedef struct { void *sprite; int slot; int box; } CopySlot;
typedef struct { void *mon; void *item; void *extra; } SlotSprites;
typedef struct {
    u32 index;
    u8 pad4[0x18];
    u32 frame;
    void *bg;
    u8 pad24[0x184 - 0x24];
    u8 transfer[0x1a8 - 0x184];
    SlotSprites slots[30];
    u8 pad1[0x398 - 0x1a8 - 30 * 12];
    void *special0;
    u8 pad39c[0x3a8 - 0x39c];
    void *special2;
    u8 pad3ac[0x3b8 - 0x3ac];
    void *special1;
    u8 pad3bc[0x3c8 - 0x3bc];
    CopySlot copies[6];
    int count;
    void *icons[6];
    u8 pad42c[0];
    u32 text[19];
} Work;
extern const u8 ov74_0223C960[];
extern void GfGfxLoader_LoadScrnData(int,int,void*,int,int,int,int,int);
extern void BgTilemapRectChangePalette(void*,int,u8,u8,u8,u8,u8);
extern void BgCommitTilemapBufferToVram(void*,int);
extern void LoadUserFrameGfx1(void*,int,int,int,int,int);
extern void LoadUserFrameGfx2(void*,int,int,int,u8,int);
extern void LoadFontPal0(int,int,int);
void ov74_02232B18(Work*);
extern void Sprite_SetDrawFlag(void*,int);
extern void ov74_02231D48(Template*,void*,void*,int);
extern void *Sprite_CreateAffine(Template*);
extern void Sprite_SetAnimActiveFlag(void*,int),Sprite_SetAnimCtrlSeq(void*,int),Sprite_SetPriority(void*,int);
extern int ov74_02231D70(void*,int,int),ov74_02231D94(void*,int,int),ov74_02231DB8(void*,int,int);
extern void ov74_02231EC4(int,int,int,u8,int,void*);
extern void GfGfx_EngineATogglePlanes(int,int);
extern void ov74_02231A1C(void*,void*,int);
void ov74_02232BD4(Work *w);
// @fn ov74_02232BD4
void ov74_02232BD4(Work *w) {
    int i;
    int species, held;
    Template tmpl;
    for (i = 0; i < 30; i++) {
        Sprite_SetDrawFlag(w->slots[i].mon, 0);
        Sprite_SetDrawFlag(w->slots[i].item, 0);
    }
    for (i = 0; i < 6; i++) {
        Sprite_SetDrawFlag(w->copies[i].sprite, 0);
    }
    Sprite_SetDrawFlag(w->special0, 0);
    Sprite_SetDrawFlag(w->special1, 0);
    Sprite_SetDrawFlag(w->special2, 0);
    ov74_02231D48(&tmpl, w, w->transfer, 1);
    for (i = 0; i < 6; i++) {
        tmpl.x = (28 + i * 40) << 12;
        tmpl.y = 142 << 12;
        w->icons[i] = Sprite_CreateAffine(&tmpl);
        Sprite_SetAnimActiveFlag(w->icons[i], 1);
        Sprite_SetAnimCtrlSeq(w->icons[i], i + 10);
        Sprite_SetPriority(w->icons[i], 1);
        Sprite_SetDrawFlag(w->icons[i], 1);
        species = ov74_02231D70(w, w->copies[i].box, w->copies[i].slot);
        held = ov74_02231D94(w, w->copies[i].box, w->copies[i].slot);
        ov74_02231EC4(species, held, ov74_02231DB8(w, w->copies[i].box, w->copies[i].slot), gSystem.unk6A, i, w->icons[i]);
    }
    GfGfxLoader_LoadScrnData(0x71, 25, w->bg, 2, 0, 0x600, 1, 0x4c);
    BgTilemapRectChangePalette(w->bg, 2, 0, 0, 32, 24, ov74_0223C960[w->index]);
    BgCommitTilemapBufferToVram(w->bg, 2);
    GfGfx_EngineATogglePlanes(2, 0);
    LoadFontPal0(0, 0x1c0, 0x4c);
    LoadUserFrameGfx1(w->bg, 0, 0x3f0, 14, 0, 0x4c);
    LoadUserFrameGfx2(w->bg, 0, 0x3d2, 13, (u8)w->frame, 0x4c);
    ov74_02232B18(w);
    w->text[13] = 10;
    ov74_02231A1C(w, w->text, 24);
}
