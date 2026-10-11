#include "overlay14_box_item_panel_private.h"

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

void ov14_021F5E94(void *p) {
    if (U32_AT(PTR_AT(p, 0), 8) == 3) {
        ov14_021F5C94(p, 0x19, 0x40);
    } else {
        ov14_021F5C94(p, 0x19, 0x3d);
    }
}

void PCBox_DrawReturnButton(void *p, u32 style) {
    PCBox_DrawButtonWindow(p, 0x1a, 0x3e, style);
}

void PCBox_DrawExitButton(void *p, u32 style) {
    PCBox_DrawButtonWindow(p, 0x1b, 0x4c, style);
}

void ov14_021F5ED4(void *p, u32 style) {
    PCBox_DrawButtonWindow(p, 0x1c, 0x47, style);
}

void PCBox_DrawActionMenu(void *p, BoxMenuEntry *list, u32 count) {
    u16 v[2];
    u32 i;

    BgCellManager_GetCellSize(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 3, &v[1], &v[0]);
    for (i = 0; i < count; i++) {
        BoxMenuEntry *e = &list[count - 1 - i];
        if (e->kind == 0) {
            PCBox_DrawButtonWindow(p, 0x21 - i, e->msgId, 0);
            BgCellManager_SetCellPalette(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 7 - i, 0, 0, (u8)v[1], (u8)v[0], 0xc);
        } else {
            ov14_021F5C94(p, 0x21 - i, e->msgId);
            BgCellManager_SetCellPalette(PTR_AT(PTR_AT(p, 0x34), 0x2f0), 7 - i, 0, 0, (u8)v[1], (u8)v[0], 2);
        }
    }
    for (; count < 5; count++) {
        FillWindowPixelBuffer((u8 *)PTR_AT(p, 0x34) + 0x30 + (0x21 - count) * 0x10, 0);
        CopyWindowPixelsToVram_TextMode((u8 *)PTR_AT(p, 0x34) + 0x30 + (0x21 - count) * 0x10);
    }
}

void PCBox_DrawBagGiveButton(void *p, u32 which) {
    FillWindowPixelBuffer((u8 *)PTR_AT(p, 0x34) + 0x30 + 0x1d * 0x10, 0);
    FillWindowPixelBuffer((u8 *)PTR_AT(p, 0x34) + 0x30 + 0x1e * 0x10, 0);
    FillWindowPixelBuffer((u8 *)PTR_AT(p, 0x34) + 0x30 + 0x1f * 0x10, 0);
    FillWindowPixelBuffer((u8 *)PTR_AT(p, 0x34) + 0x30 + 0x20 * 0x10, 0);
    CopyWindowPixelsToVram_TextMode((u8 *)PTR_AT(p, 0x34) + 0x30 + 0x1d * 0x10);
    CopyWindowPixelsToVram_TextMode((u8 *)PTR_AT(p, 0x34) + 0x30 + 0x1e * 0x10);
    CopyWindowPixelsToVram_TextMode((u8 *)PTR_AT(p, 0x34) + 0x30 + 0x1f * 0x10);
    CopyWindowPixelsToVram_TextMode((u8 *)PTR_AT(p, 0x34) + 0x30 + 0x20 * 0x10);
    if (which == 0) {
        PCBox_DrawButtonWindow(p, 0x21, 0x51, 0);
    } else {
        PCBox_DrawButtonWindow(p, 0x21, 0x50, 0);
    }
}
