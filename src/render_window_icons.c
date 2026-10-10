#include <string.h>

#include "global.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "pokemon.h"
#include "render_text.h"
#include "render_window_internal.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"
#include "systask_environment.h"
#include "unk_0200A090.h"
#include "unk_02013FDC.h"

// Window decorations: the text printer's down arrow and the waiting icon (NARC a/0/3/8 members 0x16/0x17 drawn
// over frame tiles 10-11), map signposts (NARC a/0/3/6) and the framed Pokemon picture window (a/0/3/8 0x2F-0x32).

struct WaitingIcon {
    Window *window;
    u8 frameTiles[8][0x80];
    u8 savedTiles[0x80];
    u16 baseTile;
    u8 timer;
    u8 frame : 7;
    u8 unused : 1;
    u8 state : 2;
    u8 unused2 : 6;
}; // size: 0x48C

struct PokemonPicWindow {
    FieldSpriteManager spriteManager; // 0x000
    ManagedSprite *sprite;            // 0x164
    BgConfig *bgConfig;               // 0x168
    u8 layer;                         // 0x16C
    u8 x;                             // 0x16D
    u8 y;                             // 0x16E
    u8 state;                         // 0x16F
};

void LoadDownArrowTiles(Window *window, u32 baseTile) {
    LoadWindowFrameIconTiles(window, 0x16, baseTile, 3, 3, 0);
}

// Same tiles as LoadUserFrameGfx2, with the transparent pixels set to color.
void LoadUserFrameGfx2WithFill(BgConfig *bgConfig, u8 bgId, u16 baseTile, u8 color, u8 frame, enum HeapID heapId) {
    NNSG2dCharacterData *charData;
    void *charFile = GfGfxLoader_GetCharData((NarcId)0x26, (s32)GetUserFrameGfx2TilesMember(frame), FALSE, &charData, heapId);
    u8 *tiles = Heap_Alloc(heapId, 0x240);
    u32 i;

    memcpy(tiles, charData->pRawData, 0x240);
    for (i = 0; i < 0x240; i++) {
        u8 high = tiles[i] >> 4;
        u8 low = tiles[i] & 0xF;
        if (high == 0) {
            high = color;
        }
        if (low == 0) {
            low = color;
        }
        tiles[i] = (high << 4) | low;
    }
    BG_LoadCharTilesData(bgConfig, bgId, tiles, 0x240, baseTile);
    Heap_Free(charFile);
    Heap_Free(tiles);
}

void LoadMapSignpostFrameAndGraphic(BgConfig *bgConfig, u8 bgId, u16 baseTile, u8 plttNum, u8 type, u16 map, enum HeapID heapId) {
    void *plttFile;
    NNSG2dPaletteData *pltt;

    GfGfxLoader_LoadCharData((NarcId)0x24, 0, bgConfig, (GFBgLayer)bgId, baseTile, 0x3C0, FALSE, heapId);
    plttFile = AllocAndReadWholeNarcMemberByIdPair((NarcId)0x24, 1, heapId);
    NNS_G2dGetUnpackedPaletteData(plttFile, &pltt);
    BG_LoadPlttData(bgId, (u8 *)pltt->pRawData + type * 0x20, 0x20, plttNum << 5);
    Heap_FreeExplicit(heapId, plttFile);
    {
        u8 typeLocal = *(volatile u8 *)&type;
        if (typeLocal <= 1) {
            LoadMapSignpostGraphic(bgConfig, bgId, baseTile + 0x1E, typeLocal, map, heapId);
        }
    }
}

#pragma force_active on

void LoadMapSignpostGraphic(BgConfig *bgConfig, u32 bgId, u16 baseTile, BOOL type, u16 map, u32 heapId) {
    if (type == 0) {
        map += 33;
    } else {
        map += 2;
    }
    GfGfxLoader_LoadCharData((NarcId)36, map, bgConfig, (GFBgLayer)bgId, baseTile, 768, FALSE, (enum HeapID)heapId);
}

void DrawSignpostFrameTilemap(BgConfig *bgConfig, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 paletteNum, u16 baseTile) {
    int top = y - 1;
    FillBgTilemapRect(bgConfig, bgId, baseTile, x - 9, top, 1, 1, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, baseTile + 1, x - 8, top, 1, 1, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, baseTile + 2, x - 7, top, width + 7, 1, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, baseTile + 3, x + width, top, 1, 1, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, baseTile + 4, x + width + 1, top, 1, 1, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, baseTile + 5, x + width + 2, top, 1, 1, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, baseTile + 6, x - 9, y, 1, height, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, baseTile + 7, x - 8, y, 1, height, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, baseTile + 8, x - 1, y, 1, height, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, baseTile + 9, x + width, y, 1, height, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, baseTile + 10, x + width + 1, y, 1, height, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, baseTile + 11, x + width + 2, y, 1, height, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, baseTile + 12, x - 9, y + height, 1, 1, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, baseTile + 13, x - 8, y + height, 1, 1, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, baseTile + 14, x - 7, y + height, width + 7, 1, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, baseTile + 15, x + width, y + height, 1, 1, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, baseTile + 16, x + width + 1, y + height, 1, 1, paletteNum);
    FillBgTilemapRect(bgConfig, bgId, baseTile + 17, x + width + 2, y + height, 1, 1, paletteNum);
}

#pragma force_active off

// The 6x4 tile signpost graphic left of the window.
void DrawSignpostGraphicTilemap(Window *window, u16 baseTile, u8 paletteNum) {
    u16 row;
    u8 bgId = GetWindowBgId(window);
    u16 x = GetWindowX(window) - 7;
    u32 y = GetWindowY(window);

    for (row = 0; row < 4; row++) {
        u16 col;
        for (col = 0; col < 6; col++) {
            FillBgTilemapRect(window->bgConfig, bgId, baseTile + row * 6 + col, (u8)(x + col), y + row, 1, 1, paletteNum);
        }
    }
}

void DrawFrameAndWindow3(Window *window, BOOL dontCopyToVram, u16 baseTile, u8 paletteNum, u8 type) {
    u8 bgId = GetWindowBgId(window);

    if (type <= 1) {
        u8 x = GetWindowX(window);
        u8 y = GetWindowY(window);
        u8 width = GetWindowWidth(window);
        u8 height = GetWindowHeight(window);
        DrawSignpostFrameTilemap(window->bgConfig, bgId, x, y, width, height, paletteNum, baseTile);
        DrawSignpostGraphicTilemap(window, (u16)(baseTile + 0x1E), paletteNum);
    } else {
        u8 x = GetWindowX(window);
        u8 y = GetWindowY(window);
        u8 width = GetWindowWidth(window);
        u8 height = GetWindowHeight(window);
        DrawFrame2Tilemap(window->bgConfig, bgId, x, y, width, height, paletteNum, baseTile);
    }
    if (dontCopyToVram == FALSE) {
        CopyWindowToVram(window);
    }
    TextPrinter_SetDownArrowBaseTile(baseTile);
}

#pragma force_active on

WaitingIcon *WaitingIcon_New(Window *window, int baseTile) {
    int heapId = BgConfig_GetHeapId(window->bgConfig);
    u8 *bgChars = BgGetCharPtr(GetWindowBgId(window));
    WaitingIcon *icon = Heap_Alloc((enum HeapID)heapId, sizeof(WaitingIcon));
    u8 *buffer;
    u32 tile10Offset;
    u32 tile11Offset;
    u8 i;
    NNSG2dCharacterData *charData;
    void *charFile;

    memcpy(icon->savedTiles, bgChars + (baseTile + 18) * 32, 128);
    buffer = Heap_Alloc((enum HeapID)heapId, 128);
    tile10Offset = (baseTile + 10) * 32;
    memcpy(buffer, bgChars + tile10Offset, 32);
    tile11Offset = (baseTile + 11) * 32;
    memcpy(buffer + 32, bgChars + tile11Offset, 32);
    memcpy(buffer + 64, bgChars + tile10Offset, 32);
    memcpy(buffer + 96, bgChars + tile11Offset, 32);
    for (i = 0; i < 8; i++) {
        memcpy(icon->frameTiles[i], buffer, 128);
    }
    Heap_Free(buffer);
    charFile = GfGfxLoader_GetCharData((NarcId)38, 23, FALSE, &charData, (enum HeapID)heapId);
    BlitPixelRect4Bit(charData->pRawData, 0, 0, 16, 128, icon->frameTiles, 16, 128, 0, 0, 16, 128);
    Heap_Free(charFile);
    icon->window = window;
    icon->baseTile = baseTile;
    icon->timer = 0;
    icon->frame = 0;
    icon->state = 0;
    SysTask_CreateOnVBlankQueue((SysTaskFunc)WaitingIcon_Task, icon, 0);
    WaitingIcon_Draw(icon, 1);
    return icon;
}

// mode 0: load the current frame's tiles, 1: also put them on the tilemap, 2: restore the saved tiles and the frame.
void WaitingIcon_Draw(WaitingIcon *icon, int mode) {
    u8 bgId = GetWindowBgId(icon->window);
    u8 x = GetWindowX(icon->window);
    u8 y = GetWindowY(icon->window);
    u8 width = GetWindowWidth(icon->window);
    if (mode == 2) {
        BG_LoadCharTilesData(icon->window->bgConfig, bgId, icon->savedTiles, 128, icon->baseTile + 18);
        FillBgTilemapRect(icon->window->bgConfig, bgId, icon->baseTile + 10, x + width + 1, y + 2, 1, 1, 16);
        FillBgTilemapRect(icon->window->bgConfig, bgId, icon->baseTile + 11, x + width + 2, y + 2, 1, 1, 16);
        FillBgTilemapRect(icon->window->bgConfig, bgId, icon->baseTile + 10, x + width + 1, y + 3, 1, 1, 16);
        FillBgTilemapRect(icon->window->bgConfig, bgId, icon->baseTile + 11, x + width + 2, y + 3, 1, 1, 16);
        BgCommitTilemapBufferToVram(icon->window->bgConfig, bgId);
        return;
    }
    BG_LoadCharTilesData(icon->window->bgConfig, bgId, icon->frameTiles[icon->frame], 128, icon->baseTile + 18);
    if (mode != 0) {
        FillBgTilemapRect(icon->window->bgConfig, bgId, icon->baseTile + 18, x + width + 1, y + 2, 1, 1, 16);
        FillBgTilemapRect(icon->window->bgConfig, bgId, icon->baseTile + 19, x + width + 2, y + 2, 1, 1, 16);
        FillBgTilemapRect(icon->window->bgConfig, bgId, icon->baseTile + 20, x + width + 1, y + 3, 1, 1, 16);
        FillBgTilemapRect(icon->window->bgConfig, bgId, icon->baseTile + 21, x + width + 2, y + 3, 1, 1, 16);
        BgCommitTilemapBufferToVram(icon->window->bgConfig, bgId);
    }
}

#pragma force_active off

void WaitingIcon_Task(SysTask *task, WaitingIcon *icon) {
    if (icon->state != 0) {
        if (icon->state == 1) {
            WaitingIcon_Draw(icon, 2);
        }
        SysTask_Destroy(task);
        return;
    }
    icon->timer++;
    if (icon->timer == 16) {
        icon->timer = 0;
        icon->frame = (icon->frame + 1) & 7;
        WaitingIcon_Draw(icon, 0);
    }
}

void WaitingIcon_FreeTask(SysTask *task, void *icon) {
    Heap_Free(icon);
    SysTask_Destroy(task);
}

void WaitingIcon_Delete(WaitingIcon *icon) {
    SysTask_CreateOnVWaitQueue(WaitingIcon_FreeTask, icon, 0);
    icon->state = 1;
}

void WaitingIcon_DeleteWithoutRestore(WaitingIcon *icon) {
    SysTask_CreateOnVWaitQueue(WaitingIcon_FreeTask, icon, 0);
    icon->state = 2;
}

// The returned pointer is the picture's state byte: set it to 1 to close the picture, 2 to play its animation.
u8 *DrawPokemonPicFromSpecies(BgConfig *bgConfig, u8 layer, u8 x, u8 y, u8 paletteNum, u16 baseTile, u16 species, u8 gender, u32 heapId) {
    s32 heap = heapId;
    PokemonPicWindow *picWindow = PokemonPicWindow_New(bgConfig, layer, x, y, heap);

    PokemonPicWindow_InitSpriteManager(picWindow, (enum HeapID)heap);
    PokemonPicWindow_LoadSpriteResources(picWindow);
    PokemonPicWindow_CreateSprite(picWindow, x, y);
    PokemonPicWindow_LoadSpeciesGfx(picWindow, species, gender);
    PokemonPicWindow_DrawFrame(picWindow, paletteNum, baseTile);
    BgCommitTilemapBufferToVram(bgConfig, layer);
    return &picWindow->state;
}

u8 *DrawPokemonPicFromMon(BgConfig *bgConfig, u8 layer, u8 x, u8 y, u8 paletteNum, u16 baseTile, Pokemon *mon, u32 heapId) {
    s32 heap = heapId;
    PokemonPicWindow *picWindow = PokemonPicWindow_New(bgConfig, layer, x, y, heap);

    PokemonPicWindow_InitSpriteManager(picWindow, (enum HeapID)heap);
    PokemonPicWindow_LoadSpriteResources(picWindow);
    PokemonPicWindow_CreateSprite(picWindow, x, y);
    PokemonPicWindow_LoadMonGfx(picWindow, mon);
    PokemonPicWindow_DrawFrame(picWindow, paletteNum, baseTile);
    BgCommitTilemapBufferToVram(bgConfig, layer);
    return &picWindow->state;
}

void PokemonPicWindow_Task(SysTask *task, PokemonPicWindow *picWindow) {
    switch (picWindow->state) {
    case 1:
        PokemonPicWindow_ClearFrame(picWindow);
        Sprite_DeleteAndFreeResources(picWindow->sprite);
        FieldSpriteManager_ReleaseWithoutResDat(&picWindow->spriteManager);
        DestroySysTaskAndEnvironment(task);
        return;
    case 2:
        picWindow->state = 3;
        Sprite_SetAnimCtrlSeq(picWindow->sprite->sprite, 1);
        break;
    case 3:
        if (Sprite_GetAnimationFrame(picWindow->sprite->sprite) == 6) {
            picWindow->state = 0;
        }
        break;
    }

    Sprite_UpdateAnim(picWindow->sprite->sprite, FX32_ONE);
    SpriteList_RenderAndAnimateSprites(picWindow->spriteManager.spriteList);
}

PokemonPicWindow *PokemonPicWindow_New(BgConfig *bgConfig, u8 layer, u8 x, u8 y, u32 heapId) {
    PokemonPicWindow *picWindow = SysTask_GetData(CreateSysTaskAndEnvironment((SysTaskFunc)PokemonPicWindow_Task, sizeof(PokemonPicWindow), 0, (enum HeapID)heapId));
    picWindow->state = 0;
    picWindow->bgConfig = bgConfig;
    picWindow->layer = layer;
    picWindow->x = x;
    picWindow->y = y;
    return picWindow;
}

void PokemonPicWindow_InitSpriteManager(PokemonPicWindow *picWindow, enum HeapID heapId) {
    SpriteResourceCountsListUnion counts = sPokemonPicWindowResourceCounts;

    FieldSpriteManager_InitEmptyResLists(&picWindow->spriteManager, &counts, 1, heapId);
}

void PokemonPicWindow_LoadSpriteResources(PokemonPicWindow *picWindow) {
    FieldSpriteManager_AddPlttRes(&picWindow->spriteManager, (NarcId)0x26, 0x32, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 0x15CD5);
    FieldSpriteManager_AddCellRes(&picWindow->spriteManager, (NarcId)0x26, 0x30, FALSE, 0x15CD5);
    FieldSpriteManager_AddAnimRes(&picWindow->spriteManager, (NarcId)0x26, 0x2F, FALSE, 0x15CD5);
    FieldSpriteManager_AddCharRes(&picWindow->spriteManager, (NarcId)0x26, 0x31, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 0x15CD5);
}

void PokemonPicWindow_CreateSprite(PokemonPicWindow *picWindow, int x, int y) {
    ManagedSpriteTemplate template = sPokemonPicWindowSpriteTemplate;

    template.x = (x + 5) * 8;
    template.y = (y + 5) * 8;
    picWindow->sprite = FieldSpriteManager_CreateManagedSprite(&picWindow->spriteManager, &template);
    SpriteList_RenderAndAnimateSprites(picWindow->spriteManager.spriteList);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

void PokemonPicWindow_LoadSpeciesGfx(PokemonPicWindow *picWindow, u16 species, u8 gender) {
    PokepicTemplate template;
    PokepicManager *pokepicManager = PokepicManager_Create((enum HeapID)picWindow->spriteManager.heapID);
    GetMonSpriteCharAndPlttNarcIdsEx(&template, species, gender, 2, 0, 0, 0);
    PokemonPicWindow_LoadGfx(picWindow, &template);
    PokepicManager_Delete(pokepicManager);
}

void PokemonPicWindow_LoadMonGfx(PokemonPicWindow *picWindow, Pokemon *mon) {
    PokepicTemplate template;
    PokepicManager *pokepicManager = PokepicManager_Create((enum HeapID)picWindow->spriteManager.heapID);
    GetPokemonSpriteCharAndPlttNarcIds(&template, mon, 2);
    PokemonPicWindow_LoadGfx(picWindow, &template);
    PokepicManager_Delete(pokepicManager);
}

// The sprite has two 10x10 tile frames side by side in the picture graphic.
void PokemonPicWindow_LoadGfx(PokemonPicWindow *picWindow, PokepicTemplate *template) {
    UnkStruct_02014E30 frame0Rect;
    UnkStruct_02014E30 frame1Rect;
    void *buffer;
    SpriteResource *resource;
    NNSG2dImageProxy *imageProxy;
    u32 location;

    buffer = Heap_Alloc((enum HeapID)picWindow->spriteManager.heapID, 0x1900);
    frame0Rect = sPokemonPicFrame0Rect;
    sub_020143E0((NarcId) template->narcID, template->charDataID, (enum HeapID)picWindow->spriteManager.heapID, &frame0Rect, buffer);
    frame1Rect = sPokemonPicFrame1Rect;
    sub_020143E0((NarcId) template->narcID, template->charDataID, (enum HeapID)picWindow->spriteManager.heapID, &frame1Rect, (u8 *)buffer + 0xC80);
    resource = SpriteResourceCollection_Find(picWindow->spriteManager.spriteResManagers[GF_GFX_RES_TYPE_CHAR], 0x15CD5);
    imageProxy = SpriteTransfer_GetCharProxy(resource);
    location = NNS_G2dGetImageLocation(imageProxy, NNS_G2D_VRAM_TYPE_2DMAIN);
    DC_FlushRange(buffer, 0x1900);
    GX_LoadOBJ(buffer, location, 0x1900);
    Heap_Free(buffer);
    buffer = sub_02014450((NarcId) template->narcID, template->palDataID, (enum HeapID)picWindow->spriteManager.heapID);
    resource = SpriteResourceCollection_Find(picWindow->spriteManager.spriteResManagers[GF_GFX_RES_TYPE_PLTT], 0x15CD5);
    imageProxy = (NNSG2dImageProxy *)SpriteTransfer_GetPaletteProxy(resource, imageProxy);
    location = NNS_G2dGetImagePaletteLocation((NNSG2dImagePaletteProxy *)imageProxy, NNS_G2D_VRAM_TYPE_2DMAIN);
    DC_FlushRange(buffer, 0x20);
    GX_LoadOBJPltt(buffer, location, 0x20);
    Heap_Free(buffer);
}

void PokemonPicWindow_DrawFrame(PokemonPicWindow *picWindow, u8 paletteNum, u16 baseTile) {
    FillBgTilemapRect(picWindow->bgConfig, picWindow->layer, baseTile, (u8)(picWindow->x - 1), (u8)(picWindow->y - 1), 1, 1, paletteNum);
    FillBgTilemapRect(picWindow->bgConfig, picWindow->layer, baseTile + 1, picWindow->x, (u8)(picWindow->y - 1), 10, 1, paletteNum);
    FillBgTilemapRect(picWindow->bgConfig, picWindow->layer, baseTile + 2, (u8)(picWindow->x + 10), (u8)(picWindow->y - 1), 1, 1, paletteNum);
    FillBgTilemapRect(picWindow->bgConfig, picWindow->layer, baseTile + 4, picWindow->x, picWindow->y, 10, 10, paletteNum);
    FillBgTilemapRect(picWindow->bgConfig, picWindow->layer, baseTile + 3, (u8)(picWindow->x - 1), picWindow->y, 1, 10, paletteNum);
    FillBgTilemapRect(picWindow->bgConfig, picWindow->layer, baseTile + 5, (u8)(picWindow->x + 10), picWindow->y, 1, 10, paletteNum);
    FillBgTilemapRect(picWindow->bgConfig, picWindow->layer, baseTile + 6, (u8)(picWindow->x - 1), (u8)(picWindow->y + 10), 1, 1, paletteNum);
    FillBgTilemapRect(picWindow->bgConfig, picWindow->layer, baseTile + 7, picWindow->x, (u8)(picWindow->y + 10), 10, 1, paletteNum);
    FillBgTilemapRect(picWindow->bgConfig, picWindow->layer, baseTile + 8, (u8)(picWindow->x + 10), (u8)(picWindow->y + 10), 1, 1, paletteNum);
    ScheduleBgTilemapBufferTransfer(picWindow->bgConfig, picWindow->layer);
}

void PokemonPicWindow_ClearFrame(PokemonPicWindow *picWindow) {
    FillBgTilemapRect(picWindow->bgConfig, picWindow->layer, 0, (u8)(picWindow->x - 1), (u8)(picWindow->y - 1), 12, 12, 0);
    ScheduleBgTilemapBufferTransfer(picWindow->bgConfig, picWindow->layer);
}
