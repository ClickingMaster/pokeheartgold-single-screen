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

void ov14_021F5620(void *p) {
    void *msg;

    msg = PCBox_LoadBoxMessages();
    FillWindowPixelBuffer((u8 *)PTR_AT(p, 0x34) + 0x60, 0);
    PCBox_PrintMsg(PTR_AT(p, 0x34), msg, 3, U32_AT(PTR_AT(p, 0), 8) + 0x32, 0, 8, 0, 0x10200, 0);
    DestroyMsgData(msg);
    CopyWindowToVram((u8 *)PTR_AT(p, 0x34) + 0x60);
}

void PCBox_PrintInfoLabels(void *p) {
    FillWindowPixelBuffer((u8 *)PTR_AT(p, 0x34) + 0x30 + 0x11 * 0x10, 0);
    FillWindowPixelBuffer((u8 *)PTR_AT(p, 0x34) + 0x30 + 0x12 * 0x10, 0);
    FillWindowPixelBuffer((u8 *)PTR_AT(p, 0x34) + 0x30 + 0x13 * 0x10, 0);
    PCBox_PrintMsg(PTR_AT(p, 0x34), PTR_AT(PTR_AT(p, 0x34), 0x20), 0x11, 0x57, 0, 0, 0, 0x10200, 0);
    PCBox_PrintMsg(PTR_AT(p, 0x34), PTR_AT(PTR_AT(p, 0x34), 0x20), 0x12, 0x58, 0, 0, 0, 0x10200, 0);
    PCBox_PrintMsg(PTR_AT(p, 0x34), PTR_AT(PTR_AT(p, 0x34), 0x20), 0x13, 0x59, 0, 0, 0, 0x10200, 0);
    CopyWindowToVram((u8 *)PTR_AT(p, 0x34) + 0x30 + 0x11 * 0x10);
    CopyWindowToVram((u8 *)PTR_AT(p, 0x34) + 0x30 + 0x12 * 0x10);
    CopyWindowToVram((u8 *)PTR_AT(p, 0x34) + 0x30 + 0x13 * 0x10);
}

void PCBox_DrawBoxNameOnWallpaper(void *p, void *src, u32 a2, u32 a3, u32 a5) {
    void *window;
    void *str;

    window = Heap_AllocAtEnd(0xa, 0x10);
    AddWindowParameterized(PTR_AT(PTR_AT(p, 0x34), 0x14), window, 3, 0, 0, (u8)a3, (u8)a5, 0, (u16)a2);
    MIi_CpuCopy32(src, PTR_AT(window, 0xc), a3 * a5 * 32);
    str = String_New(0x14, 0xa);
    PCStorage_GetBoxName(PTR_AT(p, 4), U8_AT(p, 0x1f), str);
    PCBox_PrintAligned(window, str, a3 * 8 / 2, a5 * 8 / 2 - 8, 0, 0x20100, 2);
    String_Delete(str);
    CopyWindowPixelsToVram_TextMode(window);
    RemoveWindow(window);
    Heap_Free(window);
}

void PCBox_DrawTrayBoxNameAndCount(void *p) {
    u8 window[0x10];
    void *str;

    InitWindow(window);
    AddTextWindowTopLeftCorner(PTR_AT(PTR_AT(p, 0x34), 0x14), window, 0xc, 2, 0, 0);
    str = String_New(0x14, 0xa);
    PCStorage_GetBoxName(PTR_AT(p, 4), U8_AT(p, 0x25), str);
    PCBox_PrintAligned(window, str, 0x30, 0, 0, 0x10200, 2);
    String_Delete(str);
    PCBox_CopyWindowToTextObj(PTR_AT(p, 0x34), window, 0);
    RemoveWindow(window);
    InitWindow(window);
    AddTextWindowTopLeftCorner(PTR_AT(PTR_AT(p, 0x34), 0x14), window, 5, 2, 0, 0);
    str = NewString_ReadMsgData(PTR_AT(PTR_AT(p, 0x34), 0x20), 0x18);
    BufferIntegerAsString(PTR_AT(PTR_AT(p, 0x34), 0x24), 0, PCStorage_CountMonsAndEggsInBox(PTR_AT(p, 4), U8_AT(p, 0x25)), 2, 0, 1);
    BufferIntegerAsString(PTR_AT(PTR_AT(p, 0x34), 0x24), 1, 0x1e, 2, 0, 1);
    StringExpandPlaceholders(PTR_AT(PTR_AT(p, 0x34), 0x24), PTR_AT(PTR_AT(p, 0x34), 0x28), str);
    PCBox_PrintAligned(window, PTR_AT(PTR_AT(p, 0x34), 0x28), 0x14, 0, 0, 0x10200, 2);
    String_Delete(str);
    PCBox_CopyWindowToTextObj(PTR_AT(p, 0x34), window, 1);
    RemoveWindow(window);
}

void PCBox_DrawWallpaperName(void *p) {
    u8 window[0x10];
    void *str;

    InitWindow(window);
    AddTextWindowTopLeftCorner(PTR_AT(PTR_AT(p, 0x34), 0x14), window, 0xc, 2, 0, 0);
    if (U8_AT(PTR_AT(p, 0x34), 0x44d) >= 0x10) {
        if (!PCStorage_IsBonusWallpaperUnlocked(PTR_AT(p, 4), U8_AT(PTR_AT(p, 0x34), 0x44d) - 0x10)) {
            str = NewString_ReadMsgData(PTR_AT(PTR_AT(p, 0x34), 0x20), 0x3b);
        } else {
            str = NewString_ReadMsgData(PTR_AT(PTR_AT(p, 0x34), 0x20), U8_AT(PTR_AT(p, 0x34), 0x44d) + 0x23);
        }
    } else {
        str = NewString_ReadMsgData(PTR_AT(PTR_AT(p, 0x34), 0x20), U8_AT(PTR_AT(p, 0x34), 0x44d) + 0x23);
    }
    PCBox_PrintAligned(window, str, 0x30, 0, 0, 0x10200, 2);
    String_Delete(str);
    PCBox_CopyWindowToTextObj(PTR_AT(p, 0x34), window, 0);
    RemoveWindow(window);
}
