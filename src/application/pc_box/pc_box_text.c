#include <stddef.h>

#include "global.h"

#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/species.h"

#include "application/pc_box/pc_box_internal.h"
#include "msgdata/msg.naix"

#include "bg_cell_manager.h"
#include "bg_window.h"
#include "dpad_menu_box.h"
#include "font.h"
#include "font_sprite.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "grid_input_handler.h"
#include "heap.h"
#include "item.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "palette.h"
#include "pm_string.h"
#include "pokedex_util.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "render_window.h"
#include "sprite_system.h"
#include "string.h"
#include "text.h"
#include "touchscreen.h"
#include "yes_no_prompt.h"

// Callbacks and layouts of the grid inputs (PCBox_CreateGridInput).
typedef struct PCBoxGridLayout {
    const TouchscreenHitbox *hitboxes;
    const DpadMenuBox *dpadBoxes;
    const GridCallbacks *callbacks;
} PCBoxGridLayout;

extern const u8 ov14_021F8080[];
extern const WindowTemplate sPCBoxWindowTemplates[];
extern const TouchscreenHitbox sPCBoxSlotHitboxes[];
extern const TouchscreenHitbox sPCBoxPartyHitboxes[];
extern const TouchscreenHitbox ov14_021F8630[];
extern const PCBoxGridLayout sPCBoxGridLayouts[];

void PCBox_LoadTrayPageThumbnails(PCBoxWork *work) {
    u32 index;
    u32 offset;
    u32 i;

    offset = work->trayBox / 6 * 6;
    index = 15;
    i = 0;
    offset = offset << 10;
    for (; i < 6; i++) {
        PCBox_LoadSpriteCharsMain(work->view, index, (u8 *)work->view->boxThumbnails + offset, 0x400);
        index++;
        offset += 0x400;
    }
}

void PCBox_RefreshTrayBoxThumbnail(PCBoxWork *work, u32 box) {
    if (work->trayBox / 6 == box / 6) {
        PCBox_LoadSpriteCharsMain(work->view, box % 6 + 15, work->view->boxThumbnails[box], 0x400);
    }
}

typedef union BoxTilePtrSlot {
    void *ptr;
    u32 raw;
} BoxTilePtrSlot;

void PCBox_DrawBoxThumbnailMons(PCBoxWork *work, u32 box, void *dst) {
    BoxTilePtrSlot mon;
    u32 lock;
    u8 row;
    u8 col;
    u8 x;
    u32 rowbase;
    u32 y;
    u16 tile;
    u32 yy;
    u32 type;
    u32 species;
    u16 form;

    tile = 0xb;
    y = (u8)tile;
    for (row = 0; row < 5; row++) {
        x = 0xa;
        col = 0;
        rowbase = row * 6;
        for (; col < 6; col++) {
            mon.ptr = PCStorage_GetMonByIndexPair(work->storage, box, col + rowbase);
            lock = AcquireBoxMonLock(mon.ptr);
            species = GetBoxMonData((void *)mon.raw, MON_DATA_SPECIES, NULL);
            if (GetBoxMonData(mon.ptr, MON_DATA_SPECIES_EXISTS, NULL)) {
                if (!GetBoxMonData((void *)mon.raw, MON_DATA_IS_EGG, NULL)) {
                    form = (u16)GetBoxMonData(mon.ptr, MON_DATA_FORM, NULL);
                    type = (u16)GetMonBaseStatEx_HandleAlternateForm(work->view->personalNarc, species, form, 0x1b);
                } else if (species == SPECIES_MANAPHY) {
                    type = 1;
                } else {
                    type = 8;
                }
                tile = (u16)(ov14_021F8080[type] + 0x20);
                tile = (u16)((tile << 8) | tile);
                for (yy = y; (s32)yy < (s32)(y + 2); yy = (u8)(yy + 1)) {
                    MIi_CpuClear16(tile, (u16 *)((u8 *)dst + ((x & 7) + ((yy & 7) * 8 + (u32)((u8 *)(((s32)yy >> 3) * 4) + (x >> 3)) * 64))), 2);
                }
            }
            ReleaseBoxMonLock((void *)mon.raw, lock);
            x += 2;
        }
        y = (u8)(y + 2);
    }
}

void PCBox_PlaceSpriteOnTop(PCBoxView *view, u32 i, s32 x, s32 y, u32 v) {
    ((void (*)(ManagedSprite *, s32, s32))ManagedSprite_SetPositionXY)(view->sprites[i], x, y);
    PCBox_SetSpriteVisible(view, i, v);
    PCBox_SetSpritePriority(view, i, 0);
}

void PCBox_SetupDepositBoxSprites(PCBoxWork *work) {
    u32 i;
    s16 x;
    s16 y;

    PCBox_PlaceSpriteOnTop(work->view, 4, 12, 84, 1);
    PCBox_PlaceSpriteOnTop(work->view, 5, 244, 84, 1);
    PCBox_PlaceSpriteOnTop(work->view, 6, 43, 84, 1);
    PCBox_PlaceSpriteOnTop(work->view, 7, 128, 65, 1);
    PCBox_PlaceSpriteOnTop(work->view, 8, 128, 77, 1);
    for (i = 0; i < 6; i++) {
        ManagedSprite_GetPositionXY(work->view->sprites[15 + i], &x, &y);
        PCBox_PlaceSpriteOnTop(work->view, 15 + i, x, 84, 1);
    }
    PCBox_LoadTrayPageThumbnails(work);
    PCBox_SetSpriteAnim(work->view, 7, 5);
    PCBox_PlaceTrayTextObjs(work->view);
    TextOBJ_SetSpritesDrawFlag(work->view->textObjs[0].textObj, TRUE);
    TextOBJ_SetSpritesDrawFlag(work->view->textObjs[1].textObj, TRUE);
    TextOBJ_SetPriority(work->view->textObjs[0].textObj, 0);
    TextOBJ_SetPriority(work->view->textObjs[1].textObj, 0);
}

void PCBox_HideBoxTraySprites(PCBoxWork *work) {
    u32 i;

    PCBox_SetSpriteVisible(work->view, 4, 0);
    PCBox_SetSpriteVisible(work->view, 5, 0);
    PCBox_SetSpriteVisible(work->view, 6, 0);
    PCBox_SetSpriteVisible(work->view, 7, 0);
    PCBox_SetSpriteVisible(work->view, 8, 0);
    for (i = 0; i < 6; i++) {
        PCBox_SetSpriteVisible(work->view, 15 + i, 0);
    }
    TextOBJ_SetSpritesDrawFlag(work->view->textObjs[0].textObj, FALSE);
    TextOBJ_SetSpritesDrawFlag(work->view->textObjs[1].textObj, FALSE);
}

void PCBox_CreateTrayTextObjs(PCBoxView *view) {
    Window window;
    TextOBJTemplate template;
    PCBoxTextObj *textObj;

    view->fontSystem = FontSystem_NewInit(2, HEAP_ID_10);
    textObj = &view->textObjs[0];
    InitWindow(&window);
    AddTextWindowTopLeftCorner(view->bgConfig, &window, 12, 2, 0, 2);
    ObjCharTransfer_ReserveCharSpace(FontOAM_GetCharSize(&window, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_10), TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, &textObj->charTransfer);
    template.fontSystem = view->fontSystem;
    template.window = &window;
    template.spriteList = SpriteManager_GetSpriteList(view->spriteManager);
    template.plttResourceProxy = SpriteManager_FindPlttResourceProxy(view->spriteManager, 0xc101);
    template.sprite = view->sprites[7]->sprite;
    template.offset = textObj->charTransfer.offset;
    template.x = 128;
    template.y = -28;
    template.unk_20 = 1;
    template.unk_24 = 4;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = HEAP_ID_10;
    view->textObjs[0].textObj = FontOAM_Create(&template);
    TextOBJ_SetPalOffset(view->textObjs[0].textObj, 1);
    TextOBJ_SetPalOffsetRespectVramOffset(view->textObjs[0].textObj, 1);
    RemoveWindow(&window);

    textObj = &view->textObjs[1];
    InitWindow(&window);
    AddTextWindowTopLeftCorner(view->bgConfig, &window, 5, 2, 0, 2);
    ObjCharTransfer_ReserveCharSpace(FontOAM_GetCharSize(&window, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_10), TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, &textObj->charTransfer);
    template.fontSystem = view->fontSystem;
    template.window = &window;
    template.spriteList = SpriteManager_GetSpriteList(view->spriteManager);
    template.plttResourceProxy = SpriteManager_FindPlttResourceProxy(view->spriteManager, 0xc101);
    template.sprite = view->sprites[7]->sprite;
    template.offset = textObj->charTransfer.offset;
    template.x = 128;
    template.y = -28;
    template.unk_20 = 1;
    template.unk_24 = 4;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = HEAP_ID_10;
    textObj->textObj = FontOAM_Create(&template);
    TextOBJ_SetPalOffset(textObj->textObj, 1);
    TextOBJ_SetPalOffsetRespectVramOffset(textObj->textObj, 1);
    RemoveWindow(&window);
}

void PCBox_DestroyTrayTextObjs(PCBoxView *view) {
    u32 i;
    u8 *charTransfer; // walks view->textObjs[i].charTransfer
    u8 *slot;         // walks view->textObjs[i]

    i = 0;
    charTransfer = (u8 *)&view->textObjs[0].charTransfer;
    slot = (u8 *)view;
    for (; i < 2; i++) {
        ObjCharTransfer_FreeCharSpace((ObjCharSpaceReservation *)charTransfer);
        FontOAM_Delete(*(TextOBJ **)(slot + offsetof(PCBoxView, textObjs[0].textObj)));
        charTransfer += sizeof(PCBoxTextObj);
        slot += sizeof(PCBoxTextObj);
    }
    FontSystem_Delete(view->fontSystem);
}

void PCBox_CopyWindowToTextObj(PCBoxView *view, Window *window, u32 index) {
    TextOBJLayout *layout;

    layout = TextOBJLayout_New(window, HEAP_ID_10);
    TextOBJ_CopyFromBGWindow(view->textObjs[index].textObj, layout, window, HEAP_ID_10);
    TextOBJLayout_Delete(layout);
}

void PCBox_AddWindows(PCBoxWork *work) {
    u32 i;

    FontID_Alloc(4, HEAP_ID_10);
    for (i = 0; i < 44; i++) {
        AddWindow(work->view->bgConfig, &work->view->windows[i], &sPCBoxWindowTemplates[i]);
    }
}

void PCBox_RemoveWindows(PCBoxWork *work) {
    u16 i;

    for (i = 0; i < 0x2c; i++) {
        RemoveWindow(&work->view->windows[i]);
    }
    FontID_Release(4);
}

void PCBox_PrintAligned(Window *window, String *str, s32 x, s32 y, u32 font, u32 color, u32 align) {
    if (align == 1) {
        x -= FontID_String_GetWidth(font, str, 0);
    } else if (align == 2) {
        x -= FontID_String_GetWidth(font, str, 0) / 2;
    } else if (align == 3) {
        x -= FontID_String_GetWidthMultiline(font, str, 0) / 2;
    }
    // FONT goes on as a full register (the call does not narrow it to a FontID)
    ((u8(*)(Window *, u32, String *, u32, u32, u32, u32, PrinterCallback_t))AddTextPrinterParameterizedWithColor)(window, font, str, x, y, TEXT_SPEED_NOTRANSFER, color, NULL);
}

void PCBox_PrintMsg(PCBoxView *view, void *msgData, u32 win, u32 msgId, s32 x, s32 y, u32 font, u32 color, u32 align) {
    void *str;

    str = NewString_ReadMsgData(msgData, msgId);
    PCBox_PrintAligned(&view->windows[win], str, x, y, font, color, align);
    String_Delete(str);
}

void PCBox_PrintExpandedMsg(PCBoxView *view, void *msgData, u32 win, u32 msgId, s32 x, s32 y, u32 font, u32 color, u32 align) {
    void *str;

    str = NewString_ReadMsgData(msgData, msgId);
    StringExpandPlaceholders(view->msgFormat, view->string, str);
    PCBox_PrintAligned(&view->windows[win], view->string, x, y, font, color, align);
    String_Delete(str);
}

void PCBox_PrintSpeciesName(PCBoxView *view, PCBoxMonInfo *monInfo, u32 win) {
    FillWindowPixelBuffer(&view->windows[win], 0);
    if (!monInfo->isEgg) {
        BufferBoxMonSpeciesName(view->msgFormat, 0, monInfo->boxMon);
        PCBox_PrintExpandedMsg(view, view->msgData, win, 0, 0, 0, 0, 0x10200, 0);
    }
    ScheduleWindowCopyToVram(&view->windows[win]);
}

void PCBox_PrintNickname(PCBoxView *view, PCBoxMonInfo *monInfo, u32 win) {
    FillWindowPixelBuffer(&view->windows[win], 0);
    BufferBoxMonNickname(view->msgFormat, 0, monInfo->boxMon);
    PCBox_PrintExpandedMsg(view, view->msgData, win, 1, 0, 0, 0, 0x10200, 0);
    ScheduleWindowCopyToVram(&view->windows[win]);
}

void PCBox_PrintLevel(PCBoxView *view, PCBoxMonInfo *monInfo, u32 win) {
    FillWindowPixelBuffer(&view->windows[win], 0);
    if (!monInfo->isEgg) {
        MessagePrinter_PrintGlyph(view->msgPrinter, 1, &view->windows[win], 0, 5);
        BufferIntegerAsString(view->msgFormat, 0, monInfo->level, 3, PRINTING_MODE_LEFT_ALIGN, 1);
        PCBox_PrintExpandedMsg(view, view->msgData, win, 0x5a, 0x10, 0, 0, 0x10200, 0);
    }
    ScheduleWindowCopyToVram(&view->windows[win]);
}

void PCBox_PrintGender(PCBoxView *view, PCBoxMonInfo *monInfo, u32 win) {
    FillWindowPixelBuffer(&view->windows[win], 0);
    if (!monInfo->isEgg) {
        if (monInfo->showGender == 1) {
            if (monInfo->gender == 0) {
                PCBox_PrintMsg(view, view->msgData, win, 0x52, 0, 0, 0, 0x70800, 0);
            } else if (monInfo->gender == 1) {
                PCBox_PrintMsg(view, view->msgData, win, 0x53, 0, 0, 0, 0x30400, 0);
            }
        }
    }
    ScheduleWindowCopyToVram(&view->windows[win]);
}

void PCBox_PrintDexNumber(PCBoxWork *work, PCBoxMonInfo *monInfo, u32 win) {
    u32 dexno;

    FillWindowPixelBuffer(&work->view->windows[win], 0);
    if (!monInfo->isEgg) {
        dexno = Pokedex_ConvertToCurrentDexNo(SaveArray_IsNatDexEnabled(work->args->saveData), monInfo->species);
        if (dexno) {
            MessagePrinter_PrintGlyph(work->view->msgPrinter, 2, &work->view->windows[win], 0, 5);
            BufferIntegerAsString(work->view->msgFormat, 0, dexno, 3, PRINTING_MODE_LEADING_ZEROS, 1);
            PCBox_PrintExpandedMsg(work->view, work->view->msgData, win, 0x5b, 0x10, 0, 0, 0x10200, 0);
        }
    }
    ScheduleWindowCopyToVram(&work->view->windows[win]);
}

void PCBox_PrintNature(PCBoxView *view, PCBoxMonInfo *monInfo, u32 win) {
    FillWindowPixelBuffer(&view->windows[win], 0);
    if (!monInfo->isEgg) {
        BufferNatureName(view->msgFormat, 0, monInfo->nature);
        PCBox_PrintExpandedMsg(view, view->msgData, win, 0x55, 0, 0, 0, 0x10200, 0);
    } else {
        PCBox_PrintMsg(view, view->msgData, win, 0x5d, 0, 0, 0, 0x10200, 0);
    }
    ScheduleWindowCopyToVram(&view->windows[win]);
}

void PCBox_PrintAbility(PCBoxView *view, PCBoxMonInfo *monInfo, u32 win) {
    FillWindowPixelBuffer(&view->windows[win], 0);
    if (!monInfo->isEgg) {
        BufferAbilityName(view->msgFormat, 0, monInfo->ability);
        PCBox_PrintExpandedMsg(view, view->msgData, win, 0x54, 0, 0, 0, 0x10200, 0);
    } else {
        PCBox_PrintMsg(view, view->msgData, win, 0x5d, 0, 0, 0, 0x10200, 0);
    }
    ScheduleWindowCopyToVram(&view->windows[win]);
}

void PCBox_PrintHeldItem(PCBoxView *view, PCBoxMonInfo *monInfo, u32 win) {
    FillWindowPixelBuffer(&view->windows[win], 0);
    if (monInfo->heldItem) {
        BufferItemName(view->msgFormat, 0, monInfo->heldItem);
        PCBox_PrintExpandedMsg(view, view->msgData, win, 0x56, 0, 0, 0, 0x10200, 0);
    } else {
        PCBox_PrintMsg(view, view->msgData, win, 0x5c, 0, 0, 0, 0x10200, 0);
    }
    ScheduleWindowCopyToVram(&view->windows[win]);
}

void PCBox_PrintMonInfo(PCBoxWork *work, PCBoxMonInfo *monInfo) {
    PCBox_PrintSpeciesName(work->view, monInfo, 0);
    PCBox_PrintNickname(work->view, monInfo, 1);
    PCBox_PrintLevel(work->view, monInfo, 2);
    PCBox_PrintGender(work->view, monInfo, 4);
    PCBox_PrintDexNumber(work, monInfo, 5);
    PCBox_PrintNature(work->view, monInfo, 6);
    PCBox_PrintAbility(work->view, monInfo, 7);
    PCBox_PrintHeldItem(work->view, monInfo, 8);
}

void PCBox_ClearMonInfoWindows(PCBoxView *view) {
    ClearWindowTilemapAndScheduleTransfer(&view->windows[0]);
    ClearWindowTilemapAndScheduleTransfer(&view->windows[1]);
    ClearWindowTilemapAndScheduleTransfer(&view->windows[2]);
    ClearWindowTilemapAndScheduleTransfer(&view->windows[4]);
    ClearWindowTilemapAndScheduleTransfer(&view->windows[5]);
    ClearWindowTilemapAndScheduleTransfer(&view->windows[6]);
    ClearWindowTilemapAndScheduleTransfer(&view->windows[7]);
    ClearWindowTilemapAndScheduleTransfer(&view->windows[8]);
}

#pragma force_active on
#pragma opt_common_subs off

// Draws the four moves of the mon into the move windows of the back buffer (9-12 or 13-16) and returns the
// first window id.
int PCBox_DrawMoveWindows(PCBoxWork *work, PCBoxMonInfo *monInfo) {
    NNSG2dCharacterData *charData;
    const u8 *second;
    void *allocation, *pixels;
    MsgData *msgData;
    u16 i, j;
    int index, start;
    PCBoxView *view = work->view;
    u8 next;

    if (view->infoWindowToggle == 0) {
        start = 9;
    } else {
        start = 13;
    }
    next = view->infoWindowToggle ^ 1;
    view->infoWindowToggle = next;
    allocation = GfGfxLoader_GetCharData(NARC_a_0_1_9, 6, TRUE, &charData, HEAP_ID_10);
    pixels = charData->pRawData;
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0750_bin, HEAP_ID_10);
    if (!monInfo->isEgg) {
        for (i = 0; i < 4; i++) {
            index = start + i;
            second = pixels;
            second += 32;
            for (j = 0; j < 11; j++) {
                int x = j * 8;
                BlitBitmapRect(&work->view->windows[index], (u8 *)pixels + 0x160, 0, 0, 8, 8, x, 0, 8, 8, 255);
                BlitBitmapRect(&work->view->windows[index], (void *)second, 0, 0, 8, 8, x, 8, 8, 8, 255);
            }
            PCBox_PrintMsg(work->view, msgData, index, monInfo->moves[i], 0, 0, 0, 0x10200, 0);
            CopyWindowPixelsToVram_TextMode(&work->view->windows[index]);
        }
        work->view->movesDrawn = 1;
    } else {
        work->view->movesDrawn = 0;
    }
    DestroyMsgData(msgData);
    Heap_Free(allocation);
    return start;
}

#pragma opt_common_subs reset

#pragma force_active reset

#pragma force_active on

// Prints the name and description of the held item into the back buffer windows (20-21 or 22-23).
u32 PCBox_PrintItemNameAndDesc(PCBoxWork *work, u16 itemId) {
    u32 windowId;
    Window *window;
    PCBoxView *view = work->view;

    if (view->infoWindowToggle == 0) {
        windowId = 20;
    } else {
        windowId = 22;
    }
    window = &view->windows[windowId];
    view->infoWindowToggle ^= 1;
    FillWindowPixelBuffer(window, 13);
    FillWindowPixelBuffer(window + 1, 13);
    if (itemId != ITEM_NONE) {
        GetItemNameIntoString(work->view->string, itemId, HEAP_ID_10);
        PCBox_PrintAligned(window, work->view->string, 0, 0, 0, 0x10200, 0);
        CopyWindowPixelsToVram_TextMode(window);
        GetItemDescIntoString(work->view->string, itemId, HEAP_ID_10);
        PCBox_PrintAligned(window + 1, work->view->string, 0, 0, 0, 0x10200, 0);
        CopyWindowPixelsToVram_TextMode(window + 1);
    }
    return windowId;
}

#pragma force_active reset

void PCBox_PrintModeTitle(PCBoxWork *work) {
    void *msg;

    msg = PCBox_LoadBoxMessages();
    FillWindowPixelBuffer(&work->view->windows[3], 0);
    PCBox_PrintMsg(work->view, msg, 3, work->args->mode + 0x32, 0, 8, 0, 0x10200, 0);
    DestroyMsgData(msg);
    CopyWindowToVram(&work->view->windows[3]);
}

void PCBox_PrintInfoLabels(PCBoxWork *work) {
    FillWindowPixelBuffer(&work->view->windows[0x11], 0);
    FillWindowPixelBuffer(&work->view->windows[0x12], 0);
    FillWindowPixelBuffer(&work->view->windows[0x13], 0);
    PCBox_PrintMsg(work->view, work->view->msgData, 0x11, 0x57, 0, 0, 0, 0x10200, 0);
    PCBox_PrintMsg(work->view, work->view->msgData, 0x12, 0x58, 0, 0, 0, 0x10200, 0);
    PCBox_PrintMsg(work->view, work->view->msgData, 0x13, 0x59, 0, 0, 0, 0x10200, 0);
    CopyWindowToVram(&work->view->windows[0x11]);
    CopyWindowToVram(&work->view->windows[0x12]);
    CopyWindowToVram(&work->view->windows[0x13]);
}

void PCBox_DrawBoxNameOnWallpaper(PCBoxWork *work, void *src, u32 baseTile, u32 width, u32 height) {
    Window *window;
    String *string;

    window = Heap_AllocAtEnd(HEAP_ID_10, sizeof(Window));
    AddWindowParameterized(work->view->bgConfig, window, 3, 0, 0, width, height, 0, baseTile);
    MIi_CpuCopy32(src, window->pixelBuffer, width * height * 32);
    string = String_New(20, HEAP_ID_10);
    PCStorage_GetBoxName(work->storage, work->activeBox, string);
    PCBox_PrintAligned(window, string, width * 8 / 2, height * 8 / 2 - 8, 0, 0x20100, 2);
    String_Delete(string);
    CopyWindowPixelsToVram_TextMode(window);
    RemoveWindow(window);
    Heap_Free(window);
}

void PCBox_DrawTrayBoxNameAndCount(PCBoxWork *work) {
    Window window;
    void *str;

    InitWindow(&window);
    AddTextWindowTopLeftCorner(work->view->bgConfig, &window, 0xc, 2, 0, 0);
    str = String_New(0x14, HEAP_ID_10);
    PCStorage_GetBoxName(work->storage, work->trayBox, str);
    PCBox_PrintAligned(&window, str, 0x30, 0, 0, 0x10200, 2);
    String_Delete(str);
    PCBox_CopyWindowToTextObj(work->view, &window, 0);
    RemoveWindow(&window);
    InitWindow(&window);
    AddTextWindowTopLeftCorner(work->view->bgConfig, &window, 5, 2, 0, 0);
    str = NewString_ReadMsgData(work->view->msgData, 0x18);
    BufferIntegerAsString(work->view->msgFormat, 0, PCStorage_CountMonsAndEggsInBox(work->storage, work->trayBox), 2, PRINTING_MODE_LEFT_ALIGN, 1);
    BufferIntegerAsString(work->view->msgFormat, 1, 0x1e, 2, PRINTING_MODE_LEFT_ALIGN, 1);
    StringExpandPlaceholders(work->view->msgFormat, work->view->string, str);
    PCBox_PrintAligned(&window, work->view->string, 0x14, 0, 0, 0x10200, 2);
    String_Delete(str);
    PCBox_CopyWindowToTextObj(work->view, &window, 1);
    RemoveWindow(&window);
}

void PCBox_DrawWallpaperName(PCBoxWork *work) {
    Window window;
    void *str;

    InitWindow(&window);
    AddTextWindowTopLeftCorner(work->view->bgConfig, &window, 0xc, 2, 0, 0);
    if (work->view->wallpaperSelection >= 0x10) {
        if (!PCStorage_IsBonusWallpaperUnlocked(work->storage, work->view->wallpaperSelection - 0x10)) {
            str = NewString_ReadMsgData(work->view->msgData, 0x3b);
        } else {
            str = NewString_ReadMsgData(work->view->msgData, work->view->wallpaperSelection + 0x23);
        }
    } else {
        str = NewString_ReadMsgData(work->view->msgData, work->view->wallpaperSelection + 0x23);
    }
    PCBox_PrintAligned(&window, str, 0x30, 0, 0, 0x10200, 2);
    String_Delete(str);
    PCBox_CopyWindowToTextObj(work->view, &window, 0);
    RemoveWindow(&window);
}

#pragma force_active on

void PCBox_DrawButtonWindow(PCBoxWork *work, u32 win, u32 msgId, u32 style) {
    Window *w = &work->view->windows[win];
    void *buf;
    u32 color;
    u16 offset;
    u8 bg, x, y, width, height;
    NNSG2dCharacterData *chr;
    u8 *pix;
    u8 i;
    String *str;
    bg = GetWindowBgId(w);
    x = GetWindowX(w);
    y = GetWindowY(w);
    width = GetWindowWidth(w);
    height = GetWindowHeight(w);
    if (style == 1) {
        FillWindowPixelBuffer(w, 11);
        color = 0xE0F00;
        offset = 0x180;
    } else {
        FillWindowPixelBuffer(w, 4);
        color = 0xE0F00;
        offset = 0;
    }
    buf = GfGfxLoader_GetCharData(NARC_a_0_1_9, 64, TRUE, &chr, HEAP_ID_10);
    pix = chr->pRawData;
    BlitBitmapRect(w, pix + offset, 0, 0, 8, 8, 0, 0, 8, 8, 0xFF);
    BlitBitmapRect(w, pix + (offset + 0x40), 0, 0, 8, 8, (width - 1) * 8, 0, 8, 8, 0xFF);
    BlitBitmapRect(w, pix + (offset + 0x120), 0, 0, 8, 8, 0, (height - 1) * 8, 8, 8, 0xFF);
    BlitBitmapRect(w, pix + (offset + 0x160), 0, 0, 8, 8, (width - 1) * 8, (height - 1) * 8, 8, 8, 0xFF);
    for (i = 1; i < height - 1; i++) {
        BlitBitmapRect(w, pix + (offset + 0x60), 0, 0, 8, 8, 0, i * 8, 8, 8, 0xFF);
        BlitBitmapRect(w, pix + (offset + 0xA0), 0, 0, 8, 8, (width - 1) * 8, i * 8, 8, 8, 0xFF);
    }
    for (i = 1; i < width - 1; i++) {
        BlitBitmapRect(w, pix + (offset + 0x20), 0, 0, 8, 8, i * 8, 0, 8, 8, 0xFF);
        BlitBitmapRect(w, pix + (offset + 0x140), 0, 0, 8, 8, i * 8, (height - 1) * 8, 8, 8, 0xFF);
    }
    Heap_Free(buf);
    str = NewString_ReadMsgData(work->view->msgData, msgId);
    PCBox_PrintAligned(w, str, width * 8 / 2, 4, 4, color, 2);
    String_Delete(str);
    CopyWindowPixelsToVram_TextMode(w);
}

void PCBox_PrintWindowMessage(PCBoxWork *work, u32 win, u32 msgId, u32 style, u8 align) {
    Window *w = &work->view->windows[win];
    u8 bg, x, y, width, height;
    String *str;
    u32 color;
    bg = GetWindowBgId(w);
    x = GetWindowX(w);
    y = GetWindowY(w);
    width = GetWindowWidth(w);
    height = GetWindowHeight(w);
    if (style == 1) {
        FillWindowPixelBuffer(w, 11);
        color = 0xE0F00;
    } else {
        FillWindowPixelBuffer(w, 4);
        color = 0xE0F00;
    }
    str = NewString_ReadMsgData(work->view->msgData, msgId);
    if (align == 2 || align == 3) {
        PCBox_PrintAligned(w, str, width * 8 / 2, 0, 4, color, align);
    } else {
        PCBox_PrintAligned(w, str, 0, 0, 4, color, align);
    }
    String_Delete(str);
    CopyWindowPixelsToVram_TextMode(w);
}

#pragma force_active reset

void PCBox_DrawPartyButton(PCBoxWork *work, u32 style) {
    PCBox_DrawButtonWindow(work, 0x18, 0x3c, style);
}

#pragma force_active on

void PCBox_DrawTabWindow(PCBoxWork *work, u32 win, u32 msgId) {
    Window *w = &work->view->windows[win];
    u8 width;
    NNSG2dCharacterData *chr;
    void *buf;
    u8 *pix;
    u8 i;
    String *str;
    width = GetWindowWidth(w);
    buf = GfGfxLoader_GetCharData(NARC_a_0_1_9, 14, TRUE, &chr, HEAP_ID_10);
    pix = chr->pRawData;
    BlitBitmapRect(w, pix + 0xAE0, 0, 0, 24, 8, 0, 0, 24, 8, 0xFF);
    BlitBitmapRect(w, pix + 0xB60, 0, 0, 24, 8, 0, 8, 24, 8, 0xFF);
    BlitBitmapRect(w, pix + 0xB20, 0, 0, 24, 8, 0, 16, 24, 8, 0xFF);
    BlitBitmapRect(w, pix + 0xD60, 0, 0, 8, 8, (width - 1) * 8, 0, 8, 8, 0xFF);
    BlitBitmapRect(w, pix + 0xD20, 0, 0, 8, 8, (width - 1) * 8, 8, 8, 8, 0xFF);
    BlitBitmapRect(w, pix + 0xD40, 0, 0, 8, 8, (width - 1) * 8, 16, 8, 8, 0xFF);
    for (i = 1; i < width - 1; i++) {
        BlitBitmapRect(w, pix + 0xB00, 0, 0, 8, 8, i * 8, 0, 8, 8, 0xFF);
        BlitBitmapRect(w, pix + 0xB80, 0, 0, 8, 8, i * 8, 8, 8, 8, 0xFF);
        BlitBitmapRect(w, pix + 0xB40, 0, 0, 8, 8, i * 8, 16, 8, 8, 0xFF);
    }
    Heap_Free(buf);
    str = NewString_ReadMsgData(work->view->msgData, msgId);
    PCBox_PrintAligned(w, str, width * 8 / 2, 4, 4, 0x90A00, 2);
    String_Delete(str);
    CopyWindowPixelsToVram_TextMode(w);
}

#pragma force_active reset

void PCBox_DrawTrayTab(PCBoxWork *work, u32 unused) {
    if (work->args->mode == PC_BOX_MODE_MOVE_ITEMS) {
        PCBox_DrawTabWindow(work, 0x19, 0x40);
    } else {
        PCBox_DrawTabWindow(work, 0x19, 0x3d);
    }
}

void PCBox_DrawReturnButton(PCBoxWork *work, u32 style) {
    PCBox_DrawButtonWindow(work, 0x1a, 0x3e, style);
}

void PCBox_DrawExitButton(PCBoxWork *work, u32 style) {
    PCBox_DrawButtonWindow(work, 0x1b, 0x4c, style);
}

void PCBox_DrawMenuExitButton(PCBoxWork *work, u32 style) {
    PCBox_DrawButtonWindow(work, 0x1c, 0x47, style);
}

void PCBox_DrawActionMenu(PCBoxWork *work, const PCBoxMenuEntry *list, u32 count) {
    u16 v[2];
    u32 i;

    BgCellManager_GetCellSize(work->view->panels, 3, &v[1], &v[0]);
    for (i = 0; i < count; i++) {
        const PCBoxMenuEntry *e = &list[count - 1 - i];
        if (e->kind == 0) {
            PCBox_DrawButtonWindow(work, 0x21 - i, e->msgId, 0);
            BgCellManager_SetCellPalette(work->view->panels, 7 - i, 0, 0, (u8)v[1], (u8)v[0], 0xc);
        } else {
            PCBox_DrawTabWindow(work, 0x21 - i, e->msgId);
            BgCellManager_SetCellPalette(work->view->panels, 7 - i, 0, 0, (u8)v[1], (u8)v[0], 2);
        }
    }
    for (; count < 5; count++) {
        FillWindowPixelBuffer(&work->view->windows[0x21 - count], 0);
        CopyWindowPixelsToVram_TextMode(&work->view->windows[0x21 - count]);
    }
}

void PCBox_DrawBagGiveButton(PCBoxWork *work, u32 which) {
    FillWindowPixelBuffer(&work->view->windows[0x1d], 0);
    FillWindowPixelBuffer(&work->view->windows[0x1e], 0);
    FillWindowPixelBuffer(&work->view->windows[0x1f], 0);
    FillWindowPixelBuffer(&work->view->windows[0x20], 0);
    CopyWindowPixelsToVram_TextMode(&work->view->windows[0x1d]);
    CopyWindowPixelsToVram_TextMode(&work->view->windows[0x1e]);
    CopyWindowPixelsToVram_TextMode(&work->view->windows[0x1f]);
    CopyWindowPixelsToVram_TextMode(&work->view->windows[0x20]);
    if (which == 0) {
        PCBox_DrawButtonWindow(work, 0x21, 0x51, 0);
    } else {
        PCBox_DrawButtonWindow(work, 0x21, 0x50, 0);
    }
}

void PCBox_DrawChangeBoxButton(PCBoxWork *work) {
    PCBox_DrawButtonWindow(work, 0x2a, 0x3f, 0);
}

void PCBox_DrawChangeBoxTitle(PCBoxWork *work) {
    PCBox_PrintWindowMessage(work, 0x29, 0x4d, 0, 3);
}

void PCBox_DrawBoxMenu(PCBoxWork *work) {
    PCBox_DrawChangeBoxTitle(work);
    PCBox_DrawButtonWindow(work, 0x20, 0x1a, 0);
    PCBox_DrawButtonWindow(work, 0x21, 0x1b, 0);
}

void PCBox_DrawChangeWallpaperTitle(PCBoxWork *work) {
    PCBox_PrintWindowMessage(work, 0x29, 0x4e, 0, 3);
}

void PCBox_DrawDepositTitle(PCBoxWork *work) {
    PCBox_PrintWindowMessage(work, 0x29, 0x4f, 0, 3);
}

#pragma force_active on

void PCBox_SetPanelWindowFrame(BgCellManager *mgr, int id, Window *win) {
    u16 size[2];
    u16 *map;
    u16 i;
    u16 j;
    u16 tile;
    u16 pal;
    u16 x;

    map = BgCellManager_GetCellTilemap(mgr, id);
    BgCellManager_GetCellSize(mgr, id, &size[1], &size[0]);
    tile = GetWindowBaseTile(win);
    pal = win->paletteNum << 12;
    map[0] = pal + 1000;
    map[size[1] - 1] = pal + 1002;
    map[size[1] * (size[0] - 1)] = pal + 1009;
    map[size[1] * size[0] - 1] = pal + 1011;
    for (i = 0; i < size[1] - 2; i++) {
        map[i + 1] = pal + 1001;
        map[i + size[1] * (size[0] - 1) + 1] = pal + 1010;
    }
    for (i = 0; i < size[0] - 2; i++) {
        map[size[1] * (i + 1)] = pal + 1003;
        map[size[1] * (i + 2) - 1] = pal + 1005;
    }
    for (j = 0; j < size[0] - 2; j++) {
        for (x = 0; x < size[1] - 2; x++) {
            map[x + (size[1] * (j + 1) + 1)] = pal + tile;
            tile++;
        }
    }
}

#pragma force_active reset

void PCBox_AttachWindowToPanel(BgCellManager *mgr, int id, Window *window) {
    u32 width;
    u32 height;
    u32 bg = GetWindowBgId(window);
    width = GetWindowWidth(window);
    height = GetWindowHeight(window);
    BgCellManager_InitCell(mgr, id, bg, width, height);
    BgCellManager_PutWindowTiles(mgr, id, window);
}

void PCBox_AttachPanelWindows(PCBoxView *view) {
    PCBox_AttachWindowToPanel(view->panels, 3, (Window *)(&view->windows[29]));
    PCBox_AttachWindowToPanel(view->panels, 4, (Window *)(&view->windows[30]));
    PCBox_AttachWindowToPanel(view->panels, 5, (Window *)(&view->windows[31]));
    PCBox_AttachWindowToPanel(view->panels, 6, (Window *)(&view->windows[32]));
    PCBox_AttachWindowToPanel(view->panels, 7, (Window *)(&view->windows[33]));
    PCBox_SetPanelWindowFrame(view->panels, 0xc, (Window *)(&view->windows[41]));
    PCBox_AttachWindowToPanel(view->panels, 0xe, (Window *)(&view->windows[42]));
    PCBox_AttachWindowToPanel(view->panels, 0xf, (Window *)(&view->windows[43]));
}

void PCBox_AttachPartyButtonWindow(PCBoxView *view) {
    PCBox_AttachWindowToPanel(view->panels, 8, &view->windows[24]);
}

void PCBox_AttachTrayTabWindow(PCBoxView *view) {
    PCBox_AttachWindowToPanel(view->panels, 9, &view->windows[25]);
}

void PCBox_AttachReturnButtonWindow(PCBoxView *view) {
    PCBox_AttachWindowToPanel(view->panels, 0xa, &view->windows[26]);
}

void PCBox_AttachMenuExitButtonWindow(PCBoxWork *work) {
    PCBoxView *view = work->view;

    PCBox_AttachWindowToPanel(view->panels, 11, &view->windows[28]);
    PCBox_DrawMenuExitButton(work, 0);
}

void PCBox_PrintCenteredLabel(PCBoxView *view, u32 windowIndex, u32 messageId, u32 unused) {
    Window *window = &view->windows[windowIndex];
    u8 x;

    FillWindowPixelBuffer(window, 0xD);
    x = GetWindowWidth(window) * 8 / 2;
    PCBox_PrintMsg(view, view->msgData, windowIndex, messageId, x, 0, 4, 0x00090A0D, 2);
    CopyWindowPixelsToVram_TextMode(window);
}

void PCBox_DrawConfirmCancel(PCBoxView *view) {
    PCBox_PrintCenteredLabel(view, 0x22, 0x48, 0);
    PCBox_PrintCenteredLabel(view, 0x23, 0x49, 0);
}

void PCBox_PrintPartyPanelSwitchLabel(PCBoxView *view) {
    PCBox_PrintCenteredLabel(view, 0x24, 0x4a, 0);
}

void PCBox_PrintPartyPanelExitLabel(PCBoxView *view) {
    PCBox_PrintCenteredLabel(view, 0x24, 0x4b, 0);
}

#pragma force_active on

void PCBox_AttachMarkingMenuWindows(PCBoxView *view) {
    BgCellManager_PutWindowTiles(view->panels, 2, &view->windows[34]);
    BgCellManager_PutWindowTiles(view->panels, 2, &view->windows[35]);
}

#pragma force_active reset

void PCBox_AttachPartyPanelLabelWindow(PCBoxView *view) {
    BgCellManager_PutWindowTiles(view->panels, 1, &view->windows[36]);
}

#pragma force_active on

void PCBox_DrawSummaryButton(PCBoxWork *work, u32 unused) {
    PCBoxView *view = work->view;
    void *allocation;
    int limit;
    int width = GetWindowWidth(&view->windows[43]);
    NNSG2dCharacterData *chars;
    u8 *pixels;
    int last;
    int edge;
    u8 i;
    String *string;

    allocation = GfGfxLoader_GetCharData(NARC_a_0_1_9, 14, TRUE, &chars, HEAP_ID_10);
    pixels = chars->pRawData;
    BlitBitmapRect(&view->windows[43], pixels + 0x7a0, 0, 0, 24, 8, 0, 0, 24, 8, 255);
    BlitBitmapRect(&view->windows[43], pixels + 0xba0, 0, 0, 24, 8, 0, 8, 24, 8, 255);
    BlitBitmapRect(&view->windows[43], pixels + 0xfa0, 0, 0, 24, 8, 0, 16, 24, 8, 255);
    last = width - 1;
    edge = last * 8;
    BlitBitmapRect(&view->windows[43], pixels + 0xd00, 0, 0, 8, 8, edge, 0, 8, 8, 255);
    BlitBitmapRect(&view->windows[43], pixels + 0xce0, 0, 0, 8, 8, edge, 8, 8, 8, 255);
    BlitBitmapRect(&view->windows[43], pixels + 0xcc0, 0, 0, 8, 8, edge, 16, 8, 8, 255);
    i = 3;
    if (last > 3) {
        limit = width - 1;
        do {
            int x = i * 8;
            BlitBitmapRect(&view->windows[43], pixels + 0xc20, 0, 0, 8, 8, x, 0, 8, 8, 255);
            BlitBitmapRect(&view->windows[43], pixels + 0xca0, 0, 0, 8, 8, x, 8, 8, 8, 255);
            BlitBitmapRect(&view->windows[43], pixels + 0xc80, 0, 0, 8, 8, x, 16, 8, 8, 255);
            i++;
        } while (i < limit);
    }
    Heap_Free(allocation);
    string = NewString_ReadMsgData(work->view->msgData, 65);
    PCBox_PrintAligned(&view->windows[43], string, width * 8 / 2, 4, 4, 0x90a00, 2);
    String_Delete(string);
    CopyWindowPixelsToVram_TextMode(&view->windows[43]);
}

#pragma force_active reset

MsgData *PCBox_LoadBoxMessages(void) {
    return NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0025_bin, HEAP_ID_10);
}

#pragma force_active on

void PCBox_DrawMessageFrame(Window *window) {
    FillWindowPixelBuffer(window, 15);
    DrawFrame2AroundWindow(window, 910, 10);
}

void PCBox_CloseMessageWindow(PCBoxView *view, int windowId) {
    ClearFrameAndWindow2(&view->windows[windowId], TRUE);
    ScheduleBgTilemapBufferTransfer(view->bgConfig, GetWindowBgId(&view->windows[windowId]));
}

#pragma force_active reset

void PCBox_ClearWindowFrameNoTransfer(PCBoxView *view, u32 idx) {
    ClearFrameAndWindow2(&view->windows[idx], 1);
}

void PCBox_ClearWindowFrame(PCBoxView *view, u32 idx) {
    ClearFrameAndWindow2(&view->windows[idx], 0);
}

// Prints box message MSGNO (msg_0025) into window WINDOWID with a frame.
void PCBox_PrintMessage(PCBoxWork *work, int msgNo, int windowId) {
    MsgData *msgData = PCBox_LoadBoxMessages();

    PCBox_DrawMessageFrame(&work->view->windows[windowId]);
    PCBox_PrintExpandedMsg(work->view, msgData, windowId, msgNo, 0, 0, 1, 0x1020f, 0);
    ScheduleWindowCopyToVram(&work->view->windows[windowId]);
    DestroyMsgData(msgData);
}

void PCBox_PrintExitPrompt(PCBoxWork *work, int x, int kind) {
    if (kind == 1) {
        PCBox_PrintMessage(work, 11, x);
    } else {
        PCBox_PrintMessage(work, 12, x);
    }
}

void PCBox_PrintTakeItemPrompt(PCBoxWork *work, int item, int x) {
    PCBoxView *view = work->view;
    BufferItemName(view->msgFormat, 0, item);
    PCBox_PrintMessage(work, 23, x);
}

void PCBox_PrintMailCantBeTaken(PCBoxWork *work, int x) {
    PCBox_PrintMessage(work, 24, x);
}

void PCBox_PrintMailCantBeMoved(PCBoxWork *work, int x) {
    PCBox_PrintMessage(work, 54, x);
}

void PCBox_PrintTookItem(PCBoxWork *work, int item, int x) {
    PCBoxView *view = work->view;
    BufferItemName(view->msgFormat, 0, item);
    PCBox_PrintMessage(work, 15, x);
}

void PCBox_PrintBagFull(PCBoxWork *work, int x) {
    PCBox_PrintMessage(work, 14, x);
}

void PCBox_PrintItemHeld(PCBoxWork *work, int item, int x) {
    if (item == 0) {
        BufferItemName(work->view->msgFormat, 0, ITEM_GRISEOUS_ORB);
        PCBox_PrintMessage(work, 55, x);
    } else {
        BufferItemName(work->view->msgFormat, 0, item);
        PCBox_PrintMessage(work, 16, x);
    }
}

void PCBox_PrintTouchTheMark(PCBoxWork *work, int x) {
    PCBox_PrintMessage(work, 35, x);
}

#pragma force_active on

void PCBox_PrintReleaseMessage(PCBoxWork *work, int mode, int windowId) {
    int msgNo;

    switch (mode) {
    case 0:
        msgNo = 2;
        break;
    case 1:
        msgNo = 3;
        BufferBoxMonNickname(work->view->msgFormat, 0, PCBox_GetSlotBoxMon(work, work->activeBox, work->selectedSlot));
        break;
    case 2:
        msgNo = 4;
        BufferBoxMonNickname(work->view->msgFormat, 0, PCBox_GetSlotBoxMon(work, work->activeBox, work->selectedSlot));
        break;
    case 3:
        msgNo = 31;
        break;
    case 4:
        msgNo = 32;
        BufferBoxMonNickname(work->view->msgFormat, 0, PCBox_GetSlotBoxMon(work, work->activeBox, work->selectedSlot));
        break;
    case 5:
        msgNo = 33;
        break;
    case 6:
        msgNo = 6;
        break;
    default:
        break;
    }
    PCBox_PrintMessage(work, msgNo, windowId);
}

#pragma force_active reset

void PCBox_PrintMenuPrompt(PCBoxWork *work, u32 which, u32 v) {
    switch (which) {
    case 0:
        which = 7;
        break;
    case 1:
        which = 0xa;
        break;
    }
    PCBox_PrintMessage(work, which, v);
}

void PCBox_PrintMonActionMessage(PCBoxWork *work, u32 a, u32 kind, u32 v) {
    u32 id;

    switch (kind) {
    case 0:
        id = 0x12;
        break;
    case 1:
        id = 0;
        BufferBoxMonNickname(work->view->msgFormat, 0, PCBox_GetSlotBoxMon(work, work->activeBox, work->selectedSlot));
        break;
    case 2:
        id = 5;
        break;
    case 3:
        id = 0x13;
        break;
    case 4:
        id = 0xd;
        break;
    case 5:
        id = 0x1d;
        break;
    case 6:
        id = 0x1e;
        break;
    }

    PCBox_PrintMessage(work, id, v);
}

void PCBox_PrintItemActionMessage(PCBoxWork *work, u32 kind, u32 v) {
    u32 id;

    switch (kind) {
    case 0:
        break;
    case 1:
        break;
    case 2:
        id = 0x1a;
        BufferItemName(work->view->msgFormat, 0, work->view->heldItem);
        break;
    case 3:
        id = 0x1b;
        BufferItemName(work->view->msgFormat, 0, work->view->heldItem);
        break;
    case 4:
        id = 0x18;
        break;
    case 5:
        id = 0x22;
        break;
    case 6:
        id = 0xe;
        break;
    case 7:
        id = 0x3b;
        break;
    }

    PCBox_PrintMessage(work, id, v);
}

void PCBox_PrintPanelHint(PCBoxWork *work, u32 idx, u32 kind) {
    void *msgData = PCBox_LoadBoxMessages();
    void *str;

    switch (kind) {
    case 0:
        kind = 0x24;
        break;
    case 1:
        kind = 0x25;
        break;
    case 2:
        kind = 0x26;
        break;
    case 3:
        kind = 0x27;
        break;
    case 4:
        kind = 0x1c;
        break;
    case 5:
        kind = 0x19;
        BufferItemName(work->view->msgFormat, 0, work->view->heldItem);
        break;
    case 6:
        kind = 0x38;
        break;
    case 7:
        kind = 0x39;
        break;
    case 8:
        kind = 0x3a;
        break;
    case 9:
        kind = 0x3b;
        break;
    case 10:
        kind = 0x3c;
        break;
    }

    str = NewString_ReadMsgData(msgData, kind);
    StringExpandPlaceholders(work->view->msgFormat, work->view->string, str);
    String_Delete(str);
    FillWindowPixelBuffer(&work->view->windows[idx], 0xf);
    AddTextPrinterParameterizedWithColor(&work->view->windows[idx], 1, work->view->string, 0, 0, 0xff, 0x1020f, NULL);
    DestroyMsgData(msgData);
}

void PCBox_DrawWindowFrame(PCBoxWork *work, int windowId) {
    DrawFrame2AroundWindow(&work->view->windows[windowId], 910, 10);
    ScheduleWindowCopyToVram(&work->view->windows[windowId]);
}

s32 PCBox_FindTouchedBoxSlot(void) {
    return TouchscreenHitbox_FindRectAtTouchNew(sPCBoxSlotHitboxes);
}

s32 PCBox_FindTouchedPartySlot(void) {
    return TouchscreenHitbox_FindRectAtTouchNew(sPCBoxPartyHitboxes);
}

s32 PCBox_FindTouchedTrayPartySlot(void) {
    return TouchscreenHitbox_FindRectAtTouchNew(ov14_021F8630);
}

#pragma force_active on

void PCBox_CreateModeGridInput(PCBoxWork *work) {
    int layout;

    switch (work->args->mode) {
    case PC_BOX_MODE_DEPOSIT:
        layout = 0;
        break;
    case PC_BOX_MODE_WITHDRAW:
        layout = 2;
        break;
    case PC_BOX_MODE_MOVE_POKEMON:
        layout = 3;
        break;
    case PC_BOX_MODE_MOVE_ITEMS:
        layout = 6;
        break;
    }
    PCBox_FreeGridInput(work);
    work->view->gridInput = GridInputHandler_Create(sPCBoxGridLayouts[layout].hitboxes, sPCBoxGridLayouts[layout].dpadBoxes, sPCBoxGridLayouts[layout].callbacks, work, TRUE, work->returnCursorPos, HEAP_ID_10);
    PCBox_PlaceCursorOnGridInput(work, work->returnCursorPos);
}

void PCBox_CreateGridInput(PCBoxWork *work, int layout, int initialPos) {
    PCBox_FreeGridInput(work);
    work->view->gridInput = GridInputHandler_Create(sPCBoxGridLayouts[layout].hitboxes, sPCBoxGridLayouts[layout].dpadBoxes, sPCBoxGridLayouts[layout].callbacks, work, TRUE, initialPos, HEAP_ID_10);
    PCBox_PlaceCursorOnGridInput(work, initialPos);
}

#pragma force_active reset

void PCBox_FreeGridInput(PCBoxWork *work) {
    GridInputHandler *gridInput = work->view->gridInput;

    if (gridInput != NULL) {
        GridInputHandler_Free(gridInput);
        work->view->gridInput = NULL;
    }
}

#pragma force_active on

void PCBox_PlaceCursorOnGridInput(PCBoxWork *work, int target) {
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(work->view->gridInput, target);

    struct {
        u8 y, x;
    } pos;

    PCBox_SetSpriteVisible(work->view, 9, 1);
    DpadMenuBox_GetPosition(box, &pos.x, &pos.y);
    ManagedSprite_SetPositionXY(work->view->sprites[9], pos.x, pos.y);
}

void PCBox_GridOnButton(PCBoxWork *work, int target) {
    GridInputHandler_SetNextInput(work->view->gridInput, (u8)target);
    PCBox_PlaceCursorOnGridInput(work, target);
}

#pragma force_active reset

void PCBox_GridOnSwitchToTouchMode(PCBoxWork *work) {
    PCBox_SetSpriteVisible(work->view, 9, 0);
}
