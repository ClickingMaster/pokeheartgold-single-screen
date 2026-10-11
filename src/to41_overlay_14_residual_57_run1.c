#include "to41_overlay_14_residual_57_private.h"

void PCBox_LoadTrayPageThumbnails(void *p);
void PCBox_RefreshTrayBoxThumbnail(void *p, u32 slot);
void PCBox_DrawBoxThumbnailMons(void *p, u32 box, void *dst);
void ov14_021F4B90(void *q, u32 i, s32 x, s32 y, u32 v);
void ov14_021F4BC0(void *p);
void PCBox_HideBoxTraySprites(void *p);
void PCBox_CreateTrayTextObjs(void *q);
void PCBox_DestroyTrayTextObjs(void *q);
void PCBox_CopyWindowToTextObj(void *q, void *window, u32 i);
void PCBox_AddWindows(void *p);
void PCBox_RemoveWindows(void *p);
void PCBox_PrintAligned(void *window, void *str, s32 x, s32 y, u32 font, u32 color, u32 align);
void PCBox_PrintMsg(void *q, void *msgData, u32 win, u32 msgId, s32 x, s32 y, u32 font, u32 color, u32 align);
void PCBox_PrintExpandedMsg(void *q, void *msgData, u32 win, u32 msgId, s32 x, s32 y, u32 font, u32 color, u32 align);
void PCBox_PrintSpeciesName(void *q, BoxMonView *mon, u32 win);
void PCBox_PrintNickname(void *q, BoxMonView *mon, u32 win);
void PCBox_PrintLevel(void *q, BoxMonView *mon, u32 win);
void PCBox_PrintGender(void *q, BoxMonView *mon, u32 win);
void PCBox_PrintDexNumber(void *p, BoxMonView *mon, u32 win);
void PCBox_PrintNature(void *q, BoxMonView *mon, u32 win);
void PCBox_PrintAbility(void *q, BoxMonView *mon, u32 win);
void PCBox_PrintHeldItem(void *q, BoxMonView *mon, u32 win);
void PCBox_PrintMonInfo(void *p, BoxMonView *mon);
void PCBox_ClearMonInfoWindows(void *q);
u32 ov14_021F5404(void *p, BoxMonView *mon);
u32 PCBox_PrintItemNameAndDesc(void *p, u32 itemId);
void ov14_021F5620(void *p);
void PCBox_PrintInfoLabels(void *p);
void PCBox_DrawBoxNameOnWallpaper(void *p, void *src, u32 a2, u32 a3, u32 a5);
void PCBox_DrawTrayBoxNameAndCount(void *p);
void PCBox_DrawWallpaperName(void *p);
void PCBox_DrawButtonWindow(void *p, u32 win, u32 msgId, u32 style);
void ov14_021F5BD8(void *p, u32 win, u32 msgId, u32 style, u8 align);
void PCBox_DrawPartyButton(void *p, u32 style);
void ov14_021F5C94(void *p, u32 win, u32 msgId);
void ov14_021F5E94(void *p);
void PCBox_DrawReturnButton(void *p, u32 style);
void PCBox_DrawExitButton(void *p, u32 style);
void ov14_021F5ED4(void *p, u32 style);
void PCBox_DrawActionMenu(void *p, BoxMenuEntry *list, u32 count);
void PCBox_DrawBagGiveButton(void *p, u32 which);

#if 0

#endif

typedef struct {
    void *font;
    void *window;
    void *spriteList;
    void *plttProxy;
    u32 unk10;
    u32 unk14;
    s32 x;
    s32 y;
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u32 unk2c;
} FontOamParam;

#if 0





typedef struct {
    void *font;
    void *window;
    void *spriteList;
    void *plttProxy;
    u32 unk10;
    u32 unk14;
    s32 x;
    s32 y;
    u32 unk20;
    u32 unk24;
    u32 unk28;
    u32 unk2c;
} FontOamParam;

#endif

#if 0

#endif

#if 0

#endif

void PCBox_LoadTrayPageThumbnails(void *p) {
    u32 slot;
    u32 off;
    u32 i;

    off = U8_AT(p, 0x25) / 6 * 6;
    slot = 0xf;
    i = 0;
    off = off << 10;
    for (; i < 6; i++) {
        PCBox_LoadSpriteCharsMain(PTR_AT(p, 0x34), slot, (u8 *)PTR_AT(p, 0x34) + 0x40c8 + off, 0x400);
        slot++;
        off += 0x400;
    }
}

void PCBox_RefreshTrayBoxThumbnail(void *p, u32 slot) {
    if (U8_AT(p, 0x25) / 6 == slot / 6) {
        PCBox_LoadSpriteCharsMain(PTR_AT(p, 0x34), slot % 6 + 0xf, (u8 *)PTR_AT(p, 0x34) + 0x40c8 + slot * 0x400, 0x400);
    }
}
